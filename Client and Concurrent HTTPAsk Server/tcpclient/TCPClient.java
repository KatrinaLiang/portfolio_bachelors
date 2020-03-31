package tcpclient;
import java.net.*;
import java.io.*;

public class TCPClient {

    public static String askServer(String hostname, int port, String ToServer) throws  IOException {
        if (ToServer == null)
            return(askServer(hostname, port));

        Socket clientSocket = new Socket(hostname, port);
        clientSocket.setSoTimeout(3000);

        BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(),"UTF-8"));
        DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
        outToServer.writeBytes(ToServer + '\n');

        StringBuilder modifiedSentence = new StringBuilder();
        String str;
        try {
            while ((str = inFromServer.readLine()) != null) {
                modifiedSentence.append(str + '\n');
                if (modifiedSentence.length() > 1000)
                    return modifiedSentence.toString();
            }
            clientSocket.close();
            return modifiedSentence.toString();
        }
        catch(SocketTimeoutException x) {
            clientSocket.close();
            return modifiedSentence.toString();
        }
    }

    public static String askServer(String hostname, int port) throws  IOException {
        Socket clientSocket = new Socket(hostname, port);
        clientSocket.setSoTimeout(3000);

        BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

        StringBuilder modifiedSentence = new StringBuilder();
        String str;
        try {
            while ((str = inFromServer.readLine()) != null) {
                modifiedSentence.append(str + '\n');
                if (modifiedSentence.length() > 1000)
                    return modifiedSentence.toString();
            }
            clientSocket.close();
            return modifiedSentence.toString();
        }catch(SocketTimeoutException x){
            clientSocket.close();
            return modifiedSentence.toString();
        }
    }
}
