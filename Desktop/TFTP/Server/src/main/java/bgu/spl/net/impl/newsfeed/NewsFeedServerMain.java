package bgu.spl.net.impl.newsfeed;
import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.impl.echo.StompProtocol;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.Server;
import bgu.spl.net.impl.rci.DataBase;

public class NewsFeedServerMain {

    public static void main(String[] args) {
        NewsFeed feed = new NewsFeed(); //one shared object
        DataBase database=DataBase.getInstance();
// you can use any server... 
//        Server.threadPerClient(
//                7777, //port
//                () -> new RemoteCommandInvocationProtocol<>(feed), //protocol factory
//                ObjectEncoderDecoder::new //message encoder decoder factory
//        ).serve();

        Server.reactor(
                Runtime.getRuntime().availableProcessors(),
                7777, //port
                () ->  new StompProtocol<>(database), //protocol factory   <-feed!!
                LineMessageEncoderDecoder::new //message encoder decoder factory
        ).serve();

    }
}
