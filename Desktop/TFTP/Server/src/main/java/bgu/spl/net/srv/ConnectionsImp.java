package bgu.spl.net.srv;
import bgu.spl.net.impl.rci.DataBase;
import bgu.spl.net.impl.rci.User;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImp<T> implements Connections<T> {

    private DataBase dataBase;
    private ConcurrentHashMap<Integer,ConnectionHandler<T>> ConnectionHandlerMap;

    /*
    private static ConnectionsImp instance = null;

    private static class SingletonHolder {
        private static ConnectionsImp instance = new ConnectionsImp();
    }

    public static ConnectionsImp getInstance() {
        return SingletonHolder.instance;
    }
*/
    public ConnectionsImp()
    {
        dataBase=DataBase.getInstance();
        ConnectionHandlerMap=new ConcurrentHashMap<>();
    }

    public boolean send(int connectionId, String msg) {
        if (ConnectionHandlerMap.containsKey(connectionId)) {
            ConnectionHandler<T> hendler = ConnectionHandlerMap.get(connectionId);
            hendler.send(msg);
            return true;
        }
        else
            return false;
    }

    public void send(String channel, String msg) {
        List<User> subs = dataBase.getSubs(channel);
        if(subs!=null) {
            for (int i = 0; i < subs.size(); i++) {
                ConnectionHandlerMap.get(subs.get(i).getID()).send(msg);
            }
        }
    }

    public void disconnect(int connectionId){
        ConnectionHandlerMap.remove(connectionId);
    }
    public void addClient(int connId,ConnectionHandler<T> ch){
        ConnectionHandlerMap.put(connId,ch);
    }

}
