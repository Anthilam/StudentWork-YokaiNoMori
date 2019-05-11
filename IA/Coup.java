/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * Coup.java - a move
 *
 */

import java.net.ServerSocket;
import java.net.Socket;
import java.io.*;

public class Coup implements Serializable {
  private static final long serialVersionUID = 59771629617947L;

  private EnumCoup typeCoup;  // Move type
  private EnumPiece piece;    // Move piece
  private boolean finPartie;  // Game end flag
  private Action params;      // Move action (movement/placement)

  // Constructor
  public Coup() {}

  // Constuctor 2
  public Coup(EnumCoup tpCoup, EnumPiece p, Action param) {
    this.typeCoup = tpCoup;
    this.piece = p;
    this.params = param;
  }

  // Getters
  public EnumCoup getTypeCoup() {
    return this.typeCoup;
  }

  public EnumPiece getPiece() {
    return this.piece;
  }

  public Action getParams() {
    return this.params;
  }

  public boolean getFinPartie() {
    return this.finPartie;
  }

  // Setters
  public void setTypeCoup(EnumCoup typeCoup) {
    this.typeCoup = typeCoup;
  }

  public void setEnumPiece(EnumPiece p) {
    this.piece = p;
  }

  public void setParams(Action act) {
    this.params = act;
  }

  // toString
  public String toString() {
    return "* Coup :\n\tTypeCoup: " + typeCoup + "\n\tPiece: " +  piece + "\n" + params.toString();
  }

  // Function to send the move over the network
  public void sendToNetwork(DataOutputStream ods){
    try {
      if (this.finPartie) {
        ods.writeInt(1);
      }
      else{
        ods.writeInt(0);
      }

      ods.writeInt(this.typeCoup.ordinal());
      ods.writeInt(this.piece.ordinal());
    }
    catch(IOException e) {
      System.out.println("this :"+e);
      System.exit(-1);
    }

    this.params.sendToNetwork(ods);
  }

  // Function to read a move from the network
  public void readFromNetwork(DataInputStream ids) {
    try {
      int boolValue = ids.readInt();

      // Get game end flag
      if (boolValue == 0) {
        this.finPartie = false;
      }
      else {
        this.finPartie = true;
      }

      // If the game isn't ended, read values
      if (this.finPartie == false) {
        this.typeCoup = EnumCoup.values()[ids.readInt()];
        this.piece = EnumPiece.values()[ids.readInt()];

        if (this.typeCoup == EnumCoup.DEPLACER) {
          Case cFrom = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);
          Case cTo = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);

          if (ids.readInt() == 0) {
            this.params = new DeplPiece(cFrom,cTo, false);
          }
          else {
            this.params = new DeplPiece(cFrom,cTo, true);
          }
        }
        else {
          Case cOnto = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);
          this.params = new DeposerPiece(cOnto);
        }
      }
    }
    catch(IOException e) {
      System.out.println("* Error : IOException");
      e.printStackTrace();
      System.exit(-1);
    }
  }
}
