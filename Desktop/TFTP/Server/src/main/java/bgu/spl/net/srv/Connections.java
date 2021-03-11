package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, String msg);

    void send(String channel, String msg);

    void disconnect(int connectionId);

    void addClient(int connId,ConnectionHandler<T> ch);
}
