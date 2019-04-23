/* jSicstus - A Java class that communicates with Sicstus Prolog
 * using Jasper
 *
 * Compiling rules :
 * javac -classpath /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar:. jSicstus.java
 *
 * java -classpath /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar:. jSicstus.java
 *
 */

import se.sics.jasper.*;
import java.io.*;
import java.util.*;

public class jSicstus {
    public static void main(String[] args) {
		System.out.println("* Starting jSicstus");

		// Sicstus Prolog object
    	SICStus sp = null;

		// Main loop switch
		boolean run = true;

		// Specify turn order
		boolean opponentTurn = false;

		// Prolog query data
		String side = "north";				// Our side (north/south)
		String opposide = "south";			// Opponent side (north/south)
		String oppopiece = "kodama";		// Opponent piece
		int oppox = 3, oppoy = 4;			// Opponent piece coords
		int opponewx = 3, opponewy = 3;		// Opponent piece new coords

		// The current board
		ArrayList<String> currentBoard = new ArrayList();
		// The current list of pieces captured by north player
		ArrayList<String> currentCaptN = new ArrayList();
		// The current list of pieces captured by south player
		ArrayList<String> currentCaptS = new ArrayList();

		// The new board
		ArrayList<String> newBoard = new ArrayList();
		// The new list of pieces captured by north player
		ArrayList<String> newCaptN = new ArrayList();
		// The new list of pieces captured by south player
		ArrayList<String> newCaptS = new ArrayList();

	    try {
	    	sp = new SICStus();	// Initialize Sicstus
	    	sp.load("./ia.pl");	// Load Prolog file
	    }
	    catch (SPException e) {
			e.printStackTrace();
			System.exit(-2);
	    }

		// Initialize the board
		try {
			HashMap<String, Term> results = new HashMap();

			// Create a Sicstus query
			Query qu = sp.openQuery("initial_board(Board).", results);

			// Get a solution
			boolean sol = qu.nextSolution();

			// If there is a solution
			if (sol) {
				// Convert list into Java arraylist
				if (results.get("Board").isList()) {
					Term[] term = results.get("Board").toPrologTermArray();

					for (int i = 0; i < term.length; i++) {
						currentBoard.add(term[i].toString());
					}
				}
			}

			qu.close();
		}
		catch (SPException e) {
			System.out.println("SPException");
			e.printStackTrace();
		}
		catch (Exception e) {
			System.out.println("Exception");
			e.printStackTrace();
		}

    	// boucle pour saisir les informations
	    while (run) {
			System.out.println("* New turn");

			System.out.println("* currentBoard : " + currentBoard);
			System.out.println("* currentCaptN : " + currentCaptN);
			System.out.println("* currentCaptS : " + currentCaptS);

	        // Store the result in an hashmap : PrologVarN -> Value
			HashMap<String, Term> results = new HashMap();

	        try {
				String request = "try_move("
					+ currentBoard + ", "
					+ currentCaptN + ", "
					+ currentCaptS + ", "
					+ side + ", NewBoard, NewCaptN, NewCaptS).";

				if (opponentTurn) {
					request = "force_move("
						+ opposide + ", "
						+ oppopiece + ", "
						+ oppox + ", "
						+ oppoy + ", "
						+ opponewx + ", "
						+ opponewy + ", "
						+ currentBoard + ", "
						+ currentCaptN + ", "
						+ currentCaptS + ", NewBoard, NewCaptN, NewCaptS).";
				}

		        // Create a Sicstus query
		        Query qu = sp.openQuery(request, results);

		        // Get a solution
		        boolean sol = qu.nextSolution();

				// If there is a solution
		        if (sol) {
		            // Convert lists into Java arrays
					if (results.get("NewBoard").isList()) {
						Term[] term = results.get("NewBoard").toPrologTermArray();

						for (int i = 0; i < term.length; i++) {
							newBoard.add(term[i].toString());
						}
					}

					if (results.get("NewCaptN").isList()) {
						Term[] term = results.get("NewCaptN").toPrologTermArray();

						for (int i = 0; i < term.length; i++) {
							newCaptN.add(term[i].toString());
						}
					}

					if (results.get("NewCaptS").isList()) {
						Term[] term = results.get("NewCaptS").toPrologTermArray();

						for (int i = 0; i < term.length; i++) {
							newCaptS.add(term[i].toString());
						}
					}

					// Print
					System.out.println("*");
					System.out.println("* newBoard : " + newBoard);
					System.out.println("* newCaptN : " + newCaptN);
					System.out.println("* newCaptS : " + newCaptS);

					// TODO : attendre validation serveur
					currentBoard = newBoard;
					currentCaptN = newCaptN;
					currentCaptS = newCaptS;
		        }

				newBoard = new ArrayList();
				newCaptN = new ArrayList();
				newCaptS = new ArrayList();

				// Close the query
		        qu.close();
	        }
	        catch (SPException e) {
				System.out.println("SPException");
	        	e.printStackTrace();
	        }
	        catch (Exception e) {
				System.out.println("Exception");
	        	e.printStackTrace();
	        }

	        System.out.print("*\n*");
	        saisieClavier();
	    }

    	System.out.println("* jSicstus is shutting down");
    }


    public static String saisieClavier() {
	    // declaration du buffer clavier
	    BufferedReader buff = new BufferedReader(new InputStreamReader(System.in));

	    try {
	        return buff.readLine();
	    }
	    catch (IOException e) {
	        System.err.println("IOException " + e);
	        e.printStackTrace();
	        System.exit(-1);
	    }

	    return ("halt.");
    }
}
