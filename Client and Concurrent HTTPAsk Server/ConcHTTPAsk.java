import java.net.*;
import java.io.*;
import tcpclient.TCPClient;

public class ConcHTTPAsk{
    public static void main(String[] args) throws IOException {
        try {
            ServerSocket welcomeSocket = new ServerSocket(Integer.parseInt(args[0]));

            while (true) {
                Socket socket = welcomeSocket.accept();
                new Thread(new RunnableHTTPAsk(socket)).start();
            }
        } catch(Exception e){
            System.out.println("exception");
        }
    }
}
