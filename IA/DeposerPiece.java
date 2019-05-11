/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * DeposerPiece.java - a placement for a move
 *
 */

import java.io.Serializable;
import java.io.*;

public class DeposerPiece implements Action,Serializable {
  private Case casePos; // The square where the piece will be placed

  // Constructor
  public DeposerPiece(Case c){
    this.casePos = c;
  }

  // Getters
  public Case getCasePos() {
    return this.casePos;
  }

  // Setters
  public void setCasePose(Case casePo) {
    this.casePos = casePo;
  }

  // toString
  public String toString() {
    return casePos.toString();
  }

  // Function to send this object over the network
  public void sendToNetwork(DataOutputStream ods) {
    try {
      ods.writeInt(this.casePos.getCol().ordinal());
      ods.writeInt(this.casePos.getLig().ordinal());
    }
    catch (IOException e) {
      System.out.println(e);
    }
  }
}
