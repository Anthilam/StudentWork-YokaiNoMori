/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * DeplPiece.java - a movement for a move
 *
 */

import java.io.Serializable;
import java.io.*;

public class DeplPiece implements Action,Serializable {
  private Case caseDep;     // The starting square
  private Case caseArr;     // The ending square
  private boolean estCapt;  // Capture flag

  // Constructor
  public DeplPiece(Case caseD,Case caseA,boolean cap){
    this.caseDep = caseD;
    this.caseArr = caseA;
    this.estCapt = cap;
  }

  // Getters
  public boolean getEstCapt() {
    return this.estCapt;
  }

  public Case getCaseDep() {
    return this.caseDep;
  }

  public Case getCaseArr() {
    return this.caseArr;
  }

  // Setters
  public void setEstCapt(boolean cpt) {
    this.estCapt = cpt;
  }

  public void setCaseDep(Case caseDepart) {
    this.caseDep = caseDepart;
  }

  public void setCaseArr(Case caseArrive) {
    this.caseArr = caseArrive;
  }

  // toString
  public String toString() {
    return caseDep.toString() + "\n" + caseArr.toString() + "\n\tCapture:" + estCapt;
  }

  // Function to send this object over the network
  public void sendToNetwork(DataOutputStream ods) {
    try {
      ods.writeInt(this.caseDep.getCol().ordinal());
      ods.writeInt(this.caseDep.getLig().ordinal());
      ods.writeInt(this.caseArr.getCol().ordinal());
      ods.writeInt(this.caseArr.getLig().ordinal());
      ods.writeBoolean(this.estCapt);
    }
    catch (IOException e) {
      System.out.println(e);
    }
  }
}
