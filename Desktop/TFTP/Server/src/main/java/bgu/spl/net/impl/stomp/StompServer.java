package bgu.spl.net.impl.stomp;
import bgu.spl.net.impl.rci.DataBase;
import bgu.spl.net.impl.echo.StompProtocol;
import bgu.spl.net.srv.Server;
import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;


public class StompServer  {

    public static void main(String[] args) {

           DataBase database=DataBase.getInstance();
           String s=String.format(args[0]);
            int port=Integer.parseInt(s.substring(0,s.lastIndexOf(" ")));
            if(s.substring(s.length()-7,s.length()).equals("Reactor")){
            Server server = Server.reactor(3,port,()->new StompProtocol(database),()-> new LineMessageEncoderDecoder());
            server.serve();
            }
            else{
             Server server = Server.threadPerClient(port,()->new StompProtocol(database),()-> new LineMessageEncoderDecoder());
             server.serve();
                }
            }
    }




