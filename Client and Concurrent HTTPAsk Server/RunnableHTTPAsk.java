import tcpclient.TCPClient;
import java.net.*;
import java.io.*;

public class RunnableHTTPAsk implements Runnable{
    Socket socket;
    public RunnableHTTPAsk(Socket s){
        this.socket = s;
    }

    @Override
    public void run(){
        //while(true){
            try{
                String hostname = null;
                String port = null;
                String str = null;

                BufferedReader inFromClient = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                DataOutputStream outToClient = new DataOutputStream(socket.getOutputStream());
                String clientSentence = inFromClient.readLine();

                String[] request = clientSentence.split("[ /?=&]");

                if (request[2].equals("ask")) {
                    try{
                        for (int i = 0; i < request.length-1; i++) {
                            if (request[i].equals("hostname")) {
                                hostname = request[i + 1];
                            }
                            if (request[i].equals("port")) {
                                port = request[i + 1];
                            }
                            if (request[i].equals("string")) {
                                str = request[i + 1];
                            }
                        }
                        String response = null;
                        response = TCPClient.askServer(hostname, Integer.parseInt(port), str);
                        outToClient.writeBytes("HTTP/1.1 200 OK\r\n\r\n" + response +"\r\n");
                        socket.close();
                    } catch (UnknownHostException e1) {
                        outToClient.writeBytes("HTTP/1.1 404 Not Found\r\n\r\n");
                        socket.close();
                        }
                } else {
                    outToClient.writeBytes("HTTP/1.1 400 Bad Request\r\n\r\n");
                    socket.close();
                }

            }catch(IOException e){

            }
        //}
    }

}

