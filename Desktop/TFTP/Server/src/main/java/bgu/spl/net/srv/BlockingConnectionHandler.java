package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.rci.Message;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<String> {

    private final StompMessagingProtocol<String> protocol;
    private final MessageEncoderDecoder<String> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private Connections<String> Connections;
    private int ConnectionId;

    public BlockingConnectionHandler(int ConID,Connections connections,Socket sock, MessageEncoderDecoder<String> reader, StompMessagingProtocol<String> protocol ) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.ConnectionId=ConID;
        this.Connections=connections;
    }

    @Override
    public void run() {
        protocol.start(ConnectionId,Connections);
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                String nextMessage = (String)encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    Message m=new Message();
                    m.TranslateStringToMessage(nextMessage);
                    protocol.process(m);
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(String msg) {
        if (msg != null) {
            try {
                out.write(encdec.encode(msg));
                out.flush();
            }catch (IOException e){
                e.printStackTrace();
            }
        }
    }
}
