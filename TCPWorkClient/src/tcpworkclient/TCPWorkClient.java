package tcpworkclient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class TCPWorkClient {

    public static void main(String[] args) {

        try {
            Scanner sc = new Scanner(System.in);
            
            Socket s = new Socket("localhost", 1234);
            
            PrintWriter out = new PrintWriter(s.getOutputStream());
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(
                            s.getInputStream()));

            
            // login nachricht schicken
            // wenn username = "abcd" und pw = "hallo" ist, 
            // schickt der server "login_successful
            
            
            // wenn der login erfolgreich war, dann 
            // String von der Tastatur einlesen und zum Server schicken.
            // Dann Antwort des Servers ausgeben, ausloggen und 
            // die Verbindung schließen
            
            for(int i=0;i<3;i++){
            String msg = sc.nextLine();          
            out.println(msg);
            out.flush();
            
            System.out.println(in.readLine());
            }

            
            in.close();
            out.close();
            s.close();

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
