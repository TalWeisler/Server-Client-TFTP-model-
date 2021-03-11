package bgu.spl.net.api;

import bgu.spl.net.impl.rci.DataBase;
import bgu.spl.net.impl.rci.Message;
import bgu.spl.net.srv.Connections;

public interface StompMessagingProtocol<T>  {


	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    void start(int connectionId, Connections<T> connections);
    
    void process(Message message);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
