package bgu.spl.net.impl.echo;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.rci.DataBase;
import bgu.spl.net.impl.rci.Message;
import bgu.spl.net.srv.Connections;

import java.util.concurrent.atomic.AtomicInteger;

public class StompProtocol<T> implements StompMessagingProtocol<T> {

    private boolean shouldTerminate = false;
    private Connections<T> Connections;
    private DataBase DB;
    private int ConnectionId;
    private AtomicInteger MessageID;

    public StompProtocol(DataBase db){
        this.DB=db;
    }

    public void start(int connectionId, Connections<T> connections) {
        ConnectionId = connectionId;
        Connections = connections;
        MessageID = new AtomicInteger();
    }

    public void process(Message message) {

        String AnsOfFunction;
        String response="";
        switch (message.topic) {
            case "CONNECT": {
                AnsOfFunction = DB.Login(ConnectionId,message.getValue("login"), message.getValue("passcode"));
                if (AnsOfFunction.equals("Login successful")) {
                    response =  message.ConnectedToString();
                    Connections.send(ConnectionId, response);
                } else {
                    response =message.ErrorToString(AnsOfFunction, MessageID.incrementAndGet());
                    Connections.send(ConnectionId, response);
                }
                break;
            }
            case "SUBSCRIBE": {
                AnsOfFunction = DB.Join(ConnectionId, message.getValue("destination"), Integer.parseInt(message.getValue("id")));
                if (AnsOfFunction.equals("true")) {
                    response =  message.RecieptToString();
                    Connections.send(ConnectionId, response);
                } else {
                    response = message.ErrorToString(AnsOfFunction, Integer.parseInt(message.getValue("receipt")));
                    Connections.send(ConnectionId, response);
                }
                break;
            }
            case "UNSUBSCRIBE": {
                AnsOfFunction = DB.Exit(ConnectionId, Integer.parseInt(message.getValue("id")));
                if (AnsOfFunction.equals("true")) {
                    response =  message.RecieptToString();
                    Connections.send(ConnectionId, response);
                } else {
                    response =  message.ErrorToString(AnsOfFunction, Integer.parseInt(message.getValue("receipt")));
                    Connections.send(ConnectionId, response);
                }
                break;
            }
            case "SEND": {
                int MId = MessageID.incrementAndGet();
                if (message.text.contains("Returning")) {
                    String LenderUN = message.text.substring(message.text.lastIndexOf(" ") + 1);
                    AnsOfFunction = DB.ReturnBook(ConnectionId, message.getValue("destination"), LenderUN);
                    if (AnsOfFunction.equals("true")) {
                        int sub = DB.getSubscriptionID(ConnectionId, message.getValue("destination"));
                        response =  message.MessageToString(sub, MId);
                        Connections.send(message.getValue("destination"), response);
                    } else {
                        response =  message.ErrorToString(AnsOfFunction, MId);
                        Connections.send(ConnectionId, response);
                    }
                }
                else {
                    AnsOfFunction = DB.checkSubsciption(ConnectionId, message.getValue("destination"));
                    if (AnsOfFunction.equals("true")) {
                        int sub = DB.getSubscriptionID(ConnectionId, message.getValue("destination"));
                        response =  message.MessageToString(sub, MId);
                        Connections.send(message.getValue("destination"), response);
                    } else {
                        response =  message.ErrorToString(AnsOfFunction, MId);
                        Connections.send(ConnectionId, response);
                    }
                }
                break;
            }
            case "DISCONNECT": {
                AnsOfFunction = DB.Logout(ConnectionId);
                if (AnsOfFunction.equals("true")) {
                    response = message.RecieptToString();
                    Connections.send(ConnectionId, response);
                    Connections.disconnect(ConnectionId);
                } else {
                    response = message.ErrorToString(AnsOfFunction, Integer.parseInt(message.getValue("receipt")));
                    Connections.send(ConnectionId, response);
                }
                break;
            }
        }
        if (response.substring(0,5).equals("ERROR")) {
            DB.Logout(ConnectionId);
        }
    }

    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}

