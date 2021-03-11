package bgu.spl.net.impl.rci;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.List;

public class DataBase {
    private ConcurrentHashMap<String,User> UsersListByName;//UserName+User
    private ConcurrentHashMap<Integer,User> UsersListByID;//UserName+User
    private ConcurrentHashMap<String,List<User>> Topics; //topic+User

    private static class SingeltonHolder {
        private static DataBase instance = new DataBase();
    }

    public static DataBase getInstance() {
        return SingeltonHolder.instance;
    }

    private DataBase() {
        UsersListByName=new ConcurrentHashMap<>();
        UsersListByID=new ConcurrentHashMap<>();
        Topics= new ConcurrentHashMap<>();
    }

    public String Login(int id, String UN,String Pass){//change id
        String Answer=null;
        synchronized (UsersListByName) {
            if (UsersListByName.containsKey(UN)){
                if(UsersListByName.get(UN).CheckPass(Pass))
                {
                    if(UsersListByName.get(UN).isConnect()){Answer="User already logged in"; }
                    else
                    {
                        UsersListByName.get(UN).Connected();
                        UsersListByID.remove(UsersListByName.get(UN).getID());
                        UsersListByID.put(id,UsersListByName.get(UN));
                        UsersListByName.get(UN).setID(id);
                        Answer="Login successful";
                    }
                }
                else {Answer="Wrong password";}
            }
            else {
                User U = new User(id, UN, Pass);
                U.Connected();
                synchronized (UsersListByID) {
                    UsersListByID.put(id,U);
                }
                UsersListByName.put(UN, U);
                Answer = "Login successful";
            }
        }
        return Answer;
    }

    public String Join (int ConnectionID,String genre, Integer SubscriptionID) {
        if (UsersListByID.containsKey(ConnectionID)){
            User user=UsersListByID.get(ConnectionID);
            if(user.isConnect())
            {
                if(user.getTopicID(genre)==-1) {
                    user.addTopic(genre, SubscriptionID);
                    synchronized (Topics) {
                        if (Topics.containsKey(genre)) {
                            List<User> top = Topics.get(genre);
                            top.add(user);
                        } else {
                            List<User> top = new LinkedList<>();
                            top.add(user);
                            Topics.put(genre, top);
                        }
                    }
                }
                return ("true");
            }
        }
        return "The User isn't connected";
    }

    public String Exit (int ConnectionID, Integer SubscriptionID) {

        if (UsersListByID.containsKey(ConnectionID)){
            User user=UsersListByID.get(ConnectionID);
            if(user.isConnect())
            {
                String genre= user.deleteTopic(SubscriptionID);
                if (genre!=null) {
                    synchronized (Topics) {
                        List<User> top = Topics.get(genre);
                        top.remove(user);
                    }
                    }
                    return ("true");
                }
            }
        return "The User isn't connected";
    }

    public String checkSubsciption (int ConnectionID,String genre) {
        if (UsersListByID.containsKey(ConnectionID)) {
            User user = UsersListByID.get(ConnectionID);
            if (user.isConnect()) {
                if (user.SubscribeTopic(genre)) {
                    return "true";
                }
                else
                    return "The User doesn't subscribe to this topic";
            }
            else
                return "The User isn't connected";
        }
        return "The user not appear in the UserList";
    }

    public String ReturnBook (int ConnectionID,String genre,String LenderUN) {
        if (UsersListByID.containsKey(ConnectionID)) {
            User user = UsersListByID.get(ConnectionID);
            if (user.isConnect()) {
                if (user.SubscribeTopic(genre)) {
                    if (UsersListByName.containsKey(LenderUN)&&UsersListByName.get(LenderUN).isConnect()) {
                       return "true";
                    }
                    else
                        return "The lender isn't connected";
                }
                else
                    return "The user unsubscribe to this topic";
            }
        }
        return "The User isn't connected";
    }

    public String Logout (int ConnectionID) {
        if (UsersListByID.containsKey(ConnectionID)) {
            User user = UsersListByID.get(ConnectionID);
            if (user.isConnect()) {
                user.logout();
                for (String topics:Topics.keySet())
                {
                    if(Topics.get(topics).contains(user))
                    {
                        Topics.get(topics).remove(user);
                    }
                }
                return "true";
            }
        }
        return "The User isn't connected";
    }

    public int getSubscriptionID(int ConnectionID,String genre) {
        return UsersListByID.get(ConnectionID).getTopicID(genre);
    }

    public List<User> getSubs(String channel) {
        if (Topics.containsKey(channel)) {
            synchronized (Topics) {
                return Topics.get(channel);
            }
        } else
            return null;

    }

    public void changeUserStatus(int ConnectionID)
    {
        UsersListByID.get(ConnectionID).Disconnect();
    }










}
