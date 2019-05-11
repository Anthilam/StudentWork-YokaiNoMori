/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * Prolog.java - a class that encapsulate Sicsus Prolog related functions
 *
 */

import se.sics.jasper.*;
import java.io.*;
import java.util.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Prolog {
	// Sicstus Prolog object
	SICStus sp = null;

	// The sides
	String side, opposide;

	// The current board
	ArrayList<String> currentBoard;
	// The current list of pieces captured by north player
	ArrayList<String> currentCaptN;
	// The current list of pieces captured by south player
	ArrayList<String> currentCaptS;

	// The new board
	ArrayList<String> newBoard;
	// The new list of pieces captured by north player
	ArrayList<String> newCaptN;
	// The new list of pieces captured by south player
	ArrayList<String> newCaptS;

	// Constructor
	public Prolog(String side, String opposide) {
		// Init sides
		this.side = side;
		this.opposide = opposide;

		try {
			sp = new SICStus();   // Initialize Sicstus
			sp.load("./IA/ia.pl");   // Load Prolog file
		}
		catch (SPException e) {
			System.out.println("* Error : SPException");
			e.printStackTrace();
			System.exit(-1);
		}

		System.out.println("* AI ready with the following settings : ");
		System.out.println("\tside = " + side);
		System.out.println("\topposide = " + opposide);
	}

	// Initialize the Prolog object
	public void init() {
		currentBoard = new ArrayList();
		currentCaptN = new ArrayList();
		currentCaptS = new ArrayList();

		newBoard = new ArrayList();
		newCaptN = new ArrayList();
		newCaptS = new ArrayList();

		try {
			HashMap<String, Term> results = new HashMap();

			// Create a Sicstus query that initialize the board
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
			System.out.println("* Error : SPException");
			e.printStackTrace();
			System.exit(-1);
		}
		catch (Exception e) {
			System.out.println("* Error : Exception");
			e.printStackTrace();
			System.exit(-1);
		}
	}

	/* forceEnnemyStrike : a function that process an ennemy move,
	 * using put or force_move predicates */
	public void forceEnnemyStrike(Coup coup) {
		// Store the result in an hashmap : PrologVarN -> Value
		HashMap<String, Term> results = new HashMap();

		// The Prolog request
		String request = "";

		// Get the piece in the proper format
		String oppopiece;

		switch (coup.getPiece()) {
			case KODAMA: oppopiece = "kodama"; break;
			case KODAMA_SAMOURAI: oppopiece = "samourai"; break;
			case ONI: oppopiece = "oni"; break;
			case SUPER_ONI: oppopiece = "superoni"; break;
			case KIRIN: oppopiece = "kirin"; break;
			case KOROPOKKURU: oppopiece = "koropokkuru"; break;
			default: oppopiece = "AUCUN";
		}

		int oppox = 0, oppoy = 0;         // Opponent piece base coords
		int opponewx = 0, opponewy = 0;   // Opponent piece new coords

		// If move
		if (coup.getTypeCoup() == EnumCoup.DEPLACER) {
			// Get the coordinates
			DeplPiece depl = (DeplPiece)coup.getParams();
			oppox = depl.getCaseDep().getX();
			oppoy = depl.getCaseDep().getY();
			opponewx = depl.getCaseArr().getX();
			opponewy = depl.getCaseArr().getY();

			// Build the request
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
		// If put
		else {
			// Get the coordinates
			DeposerPiece depo = (DeposerPiece)coup.getParams();
			oppox = depo.getCasePos().getX();
			oppoy = depo.getCasePos().getY();

			// Build the request
			request = "put("
				+ opposide + ", "
				+ oppopiece + ", "
				+ oppox + ", "
				+ oppoy + ", "
				+ currentBoard + ", ";

			if (opposide == "north") {
				request += currentCaptN + ", ";
			}
			else {
				request += currentCaptS + ", ";
			}

			request += "NewBoard, NewCapt).";
		}

		try {
      // Create a Sicstus query
      Query qu = sp.openQuery(request, results);

      // Get a solution
      boolean sol = qu.nextSolution();

      // If there is a solution
      if (sol) {
				// If move
				if (coup.getTypeCoup() == EnumCoup.DEPLACER) {
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

					currentBoard = newBoard;
					currentCaptN = newCaptN;
					currentCaptS = newCaptS;
				}
				// If put
				else {
					// Convert lists into Java arrays

					System.out.println("\n" + results + "\n");

					if (results.get("NewBoard").isList()) {
						Term[] term = results.get("NewBoard").toPrologTermArray();

						for (int i = 0; i < term.length; i++) {
							newBoard.add(term[i].toString());
						}
					}

					if (results.get("NewCapt").isList()) {
						Term[] term = results.get("NewCapt").toPrologTermArray();

						for (int i = 0; i < term.length; i++) {
							newCaptN.add(term[i].toString());
						}
					}

					currentBoard = newBoard;

					if (opposide == "north") {
						currentCaptN = newCaptN;
					}
					else {
						currentCaptS = newCaptN;
					}
				}
      }

      newBoard = new ArrayList();
      newCaptN = new ArrayList();
      newCaptS = new ArrayList();

      // Close the query
      qu.close();
		}
		catch (SPException e) {
			System.out.println("* Error : SPException");
			e.printStackTrace();
			System.exit(-1);
		}
		catch (Exception e) {
			System.out.println("* Error : Exception");
			e.printStackTrace();
			System.exit(-1);
		}
	}

	/* generateMove : a function that creates a move,
	 * using the generate_move predicate */
	public Coup generateMove() {
		String type = "";				// Piece's type

		long x = 0, y = 0;			// Current coordinates
		long n_x = 0, n_y = 0;	// New coordinates
		long capture = 0;				// Does it capture something or not

		// Store the result in an hashmap : PrologVarN -> Value
		HashMap<String, Term> results = new HashMap();

		// Build the request
		String request = "generate_move("
			+ currentBoard + ", "
			+ currentCaptN + ", "
			+ currentCaptS + ", "
			+ side + ", NewBoard, NewCaptN, NewCaptS, "
			+ "Type, X, Y, N_X, N_Y, Capture).";

		try {
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

				// Get non-list datas
        type = results.get("Type").getString();
        x = results.get("X").getInteger();
        y = results.get("Y").getInteger();
        n_x = results.get("N_X").getInteger();
        n_y = results.get("N_Y").getInteger();
        capture = results.get("Capture").getInteger();

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
			System.out.println("* Error : SPException");
			e.printStackTrace();
			System.exit(-1);
		}
		catch (Exception e) {
			System.out.println("* Error : Exception");
			e.printStackTrace();
			System.exit(-1);
		}

		// Get the proper type
		EnumPiece piece;
		switch (type) {
			case "kodama": piece = EnumPiece.KODAMA; break;
			case "samourai": piece = EnumPiece.KODAMA_SAMOURAI; break;
			case "oni": piece = EnumPiece.ONI; break;
			case "superoni": piece = EnumPiece.SUPER_ONI; break;
			case "kirin": piece = EnumPiece.KIRIN; break;
			case "koropokkuru": piece = EnumPiece.KOROPOKKURU; break;
			default: piece = EnumPiece.AUCUN;
		}

		// Get the proper capture
		boolean b;
		if (capture == 0) {
			b = false;
		}
		else {
			b = true;
		}

		// Return the move
		return new Coup(EnumCoup.DEPLACER, piece, new DeplPiece(new Case(x, y), new Case(n_x, n_y), b));
	}

	/* printBoard : print the current board */
	public void printBoard() {
		System.out.println("* Board :");
		System.out.println("\tcurrentBoard : " + currentBoard);
		System.out.println("\tcurrentCaptN : " + currentCaptN);
		System.out.println("\tcurrentCaptS : " + currentCaptS);
	}
}
