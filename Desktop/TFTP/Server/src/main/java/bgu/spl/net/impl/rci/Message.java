package bgu.spl.net.impl.rci;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Message<T> {
    public String topic;
    public Map<String,String> map;
    public String text;



    public Message ()
    {
        map=new ConcurrentHashMap<>();
    }

    public String getTopic(){return topic;}

    public String getText(){return text;}

    public String getValue(String sub)
    {
        if(map.containsKey(sub))
            return map.get(sub);
        else
            return null;
    }

    public void TranslateStringToMessage(String m)
    {
        int place=m.indexOf('\n');
        topic=m.substring(0,place);
        String headers=m.substring(place+1);
        headers=headers.substring(0,headers.indexOf("\n\n")+1);
        if(m.indexOf("\n\n")+2!=m.length()) {
            m = m.substring(m.indexOf("\n\n")+2);
            text=m.substring(0,m.indexOf("\n"));
        }
        place=headers.indexOf(':');
        while(place!=-1)
        {
            int place2= headers.indexOf('\n');
            String header= headers.substring(0,place);
            String t=headers.substring(place+1,place2);
            map.put(header,t);
            headers=headers.substring(place2+1);
            place=headers.indexOf(':');
        }
    }

    public String getMessage()
    {
        String ans=topic+"\n";
        for (String key : map.keySet()) {
            ans=ans+key+":"+map.get(key)+"\n";
        }
        ans=ans+"\n";
        if (text!=null && text.length()!=0)
            ans=ans+text+"\n";
        return ans;
    }

    public String ConnectedToString()
    {
        String Ans="CONNECTED\nversion:"+getValue("accept-version")+"\n\n"+'\u0000';
        return Ans;
    }

    public String ErrorToString(String response,int ID)
    {
        String Ans="ERROR\nreceipt-id:message-"+ID+"\nmessage:malformed frame received\n\nThe message:\n-----\n"+getMessage()+"-----\n"+response+'\n'+'\u0000';
        return Ans;
    }

    public String RecieptToString()
    {
        String Ans;
        if (getValue("receipt")!=null) {
            Ans = "RECEIPT\nreceipt-id:" + getValue("receipt") + "\n\n" + '\u0000';
        }
        else
            Ans = "RECEIPT\nreceipt-id:0\n\n" + '\u0000';
        return Ans;
    }

    public String MessageToString(int Sub, int ID)
    {
        String Ans="MESSAGE\nsubscription:"+Sub+"\nMessage-id:"+ID+"\ndestination:"+getValue("destination")+"\n\n"+text+"\n"+'\u0000';
        return Ans;
    }


}

