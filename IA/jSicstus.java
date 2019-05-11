/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * jSicstus.java - A Java class that communicates with :
 *    - Sicstus Prolog using Jasper
 *    - C client using sockets
 *
 * USE THE GIVEN LIBRARY FIRST (folder libSicstus4.4.1) OR USE SICSTUS 4.4.1
 *
 * Compiling rule :
 *   javac -classpath [PATH TO JASPER.JAR]/jasper.jar:. jSicstus.java
 *
 * Launching rule :
 *   java -Djava.library.path="[PATH TO JASPER LIB]" -cp [PATH TO JASPER.JAR]:./IA/:. jSicstus port
 *
 * Default jasper.jar path :
 *   /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar
 *
 * Our jasper.jar path :
 *   ./IA/libSicstus4.4.1/sicstus-4.4.1/bin/jasper.jar
 *
 * Default sicstus lib path :
 *   -Djava.library.path="/usr/local/sicstus4.4.1/lib"
 *
 * Our sisctus lib path :
 *   ./IA/libSicstus4.4.1/
 */

import se.sics.jasper.*;
import java.io.*;
import java.util.*;
import java.net.ServerSocket;
import java.net.Socket;

public class jSicstus {
  public static void main(String[] args) {

    System.out.println("* Starting AI..");

    /*--------------------------------------------------------------------------
     * JAVA <-> C
     *------------------------------------------------------------------------*/

    if (args.length != 1) {
      System.out.println("* Error : missing argument -> port");
      System.exit(-1);
    }

    ServerSocket srv = null;
    Socket s = null;

    //DataInputStream ids = null;
    //DataOutputStream ods = null;

    int port = Integer.parseInt(args[0]);
    DataInputStream ids = null;
    DataOutputStream ods= null;
    boolean sens = false; // false = nord ; true = sud

    // Server creation and waiting the client's connection
    try {
      srv = new ServerSocket(port) ;

      System.out.println("* Waiting client connection");

      s = srv.accept();

      System.out.println("* Client connected");

      ids = new DataInputStream(s.getInputStream());
      ods =  new DataOutputStream(s.getOutputStream());

      // Read the orientation sent by the game-server
      sens = ids.readBoolean();

      System.out.println("* Sens [nord (false) / sud(true)] : " + sens);
    }
    catch (IOException e) {
      System.out.println(e);
      System.exit(-1);
    }

    /*--------------------------------------------------------------------------
     * JAVA <-> Prolog
     *------------------------------------------------------------------------*/

    // Main loop switch
    boolean run = true;
    int nbPartie = 1;

    // Initialize sides
    String side = "north";      // Our side (north/south)
    String opposide = "south";  // Opponent side (north/south)

    // Change sides if sens is false
    if (sens == false) {
			side = "south";
			opposide = "north";
		}

    // Create a new Prolog object
    Prolog prolog = new Prolog(side, opposide);

    // Initialize Prolog
    prolog.init();

    /*-- GAME 1 --------------------------------------------------------------*/
    System.out.println("** STARTING GAME 1 **");

    while (run && nbPartie == 1) {
      if (side == "north") {
        // Create a move
        System.out.println("[N] * Getting move from Prolog");
        Coup oppStrike = prolog.generateMove();
        System.out.println(oppStrike);

        // Send the move
        System.out.println("[N] * Sending move to client");
        oppStrike.sendToNetwork(ods);

        // Read ennemy move
        System.out.println("[N] * Getting ennemy move from client");
        oppStrike.readFromNetwork(ids);

        // If game is ending
        if (oppStrike.getFinPartie()) {
          nbPartie++;

          System.out.println("[N] * Game end received");
        }
        // Otherwise
        else {
          System.out.println(oppStrike);

          // Update Prolog
          prolog.forceEnnemyStrike(oppStrike);
        }
      }
      else {
        // Create a move
        Coup oppStrike = new Coup();

        // Read ennemy move
        System.out.println("[S] * Getting ennemy move from client");
        oppStrike.readFromNetwork(ids);

        // If game is ending
        if (oppStrike.getFinPartie()) {
          nbPartie++;

          System.out.println("[S] * Game end received");
        }
        // Otherwise
        else {
          System.out.println(oppStrike);

          // Update Prolog
          prolog.forceEnnemyStrike(oppStrike);

          // Create a move
          System.out.println("[S] * Getting move from Prolog");
          oppStrike = prolog.generateMove();
          System.out.println(oppStrike);

          // Send the move
          System.out.println("[S] * Sending move to client");
          oppStrike.sendToNetwork(ods);
        }
      }
    }

    // Skip bytes that are still avaiblable
    try {
      ids.skip(ids.available());
    }
    catch(IOException e) {
      e.printStackTrace();
    }

    // Re-initialize Prolog
    prolog.init();

    /*-- GAME 2 --------------------------------------------------------------*/
    System.out.println("** STARTING GAME 2 **");

    while (run && nbPartie == 2) {
      if (side == "south") {
        // Create a move
        System.out.println("[S] * Getting move from Prolog");
        Coup oppStrike = prolog.generateMove();
        System.out.println(oppStrike);

        // Send the move
        System.out.println("[S] * Sending move to client");
        oppStrike.sendToNetwork(ods);

        // Read ennemy move
        System.out.println("[S] * Getting ennemy move from client");
        oppStrike.readFromNetwork(ids);

        // If game is ending
        if (oppStrike.getFinPartie()) {
          nbPartie++;

          System.out.println("[S] * Game end received");
        }
        // Otherwise
        else {
          System.out.println(oppStrike);

          // Update Prolog
          prolog.forceEnnemyStrike(oppStrike);
        }
      }
      else {
        Coup oppStrike = new Coup();

        // Read ennemy move
        System.out.println("[N] * Getting ennemy move from client");
        oppStrike.readFromNetwork(ids);

        // If game is ending
        if (oppStrike.getFinPartie()) {
          nbPartie++;

          System.out.println("[N] * Game end received");
        }
        // Otherwise
        else {
          System.out.println(oppStrike);

          // Update Prolog
          prolog.forceEnnemyStrike(oppStrike);

          // Create a move
          System.out.println("[N] * Getting move from Prolog");
          oppStrike = prolog.generateMove();
          System.out.println(oppStrike);

          // Send the move
          System.out.println("[N] * Sending move to client");
          oppStrike.sendToNetwork(ods);
        }
      }
    }

    System.out.println("* AI shutting down..");
    try{
      srv.close();
    }catch(IOException e){
      System.out.println(e);
    }

  }
}
