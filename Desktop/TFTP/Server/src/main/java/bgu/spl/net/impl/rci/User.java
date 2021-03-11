package bgu.spl.net.impl.rci;
import java.util.concurrent.ConcurrentHashMap;

public class User {
    private int ID;
    private String UserName;
    private String Pass;
    private ConcurrentHashMap<Integer,String> SubscribeTopics;
    private boolean isConnect;

    public User(int id, String userName, String pass)
    {
        ID=id;
        UserName=userName;
        Pass=pass;
        SubscribeTopics=new ConcurrentHashMap<>();
        isConnect=false;
    }

    public void Connected (){isConnect=true; }

    public void Disconnect(){isConnect=false;}

    public boolean isConnect() {return isConnect;}

    public void addTopic(String genre, int SubscriptionID)
    {
        synchronized (SubscribeTopics) {
            SubscribeTopics.putIfAbsent(SubscriptionID, genre);
        }
    }

    public String deleteTopic(int SubscriptionID)
    {
        String genre=null;
        synchronized (SubscribeTopics) {
            if (SubscribeTopics.containsKey(SubscriptionID)) {
                genre = SubscribeTopics.get(SubscriptionID);
                SubscribeTopics.remove(SubscriptionID);
            }
        }
        return genre;
    }

    public int getTopicID(String genre)
    {
        for (Integer key:SubscribeTopics.keySet())
        {
            if(SubscribeTopics.get(key).equals(genre))
            {
                return key;
            }
        }
        return -1;
    }

    public boolean SubscribeTopic (String genre)
    {
        boolean ans=false;
        synchronized (SubscribeTopics)
        {
            ans=SubscribeTopics.containsValue(genre);
        }
        return ans;
    }

    public boolean CheckPass(String pass){return (this.Pass.equals(pass));}

    public int getID() {return ID;}

    public void logout()
    {
        synchronized (this) {
            isConnect = false;
            for (Integer key : SubscribeTopics.keySet()) {
                SubscribeTopics.remove(key);
            }
        }
    }

     public void setID(int Id){ this.ID=Id;}

}
