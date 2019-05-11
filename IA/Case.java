/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * Case.java - a square in the Yokai No-Mori game
 *
 */

import java.io.Serializable;

public class Case implements Serializable {
  private EnumCol c;  // Column
  private EnumLig l;  // Line

  // Constructor
  public Case(EnumCol col, EnumLig lig){
    this.c = col;
    this.l = lig;
  }

  // Constructor 2
  public Case(long x, long y) {
    switch ((int)x) {
      case 1: c = EnumCol.A; break;
      case 2: c = EnumCol.B; break;
      case 3: c = EnumCol.C; break;
      case 4: c = EnumCol.D; break;
      case 5: c = EnumCol.E; break;
      default: c = EnumCol.AUCUN;
    }

    switch ((int)y) {
      case 1: l = EnumLig.UN; break;
      case 2: l = EnumLig.DEUX; break;
      case 3: l = EnumLig.TROIS; break;
      case 4: l = EnumLig.QUATRE; break;
      case 5: l = EnumLig.CINQ; break;
      case 6: l = EnumLig.SIX; break;
      default: l = EnumLig.AUCUN;
    }
  }

  // Getters (base format)
  public EnumCol getCol() {
    return this.c;
  }

  public EnumLig getLig() {
    return this.l;
  }

  // Getters (int format)
  public int getX() {
    return this.c.ordinal() + 1;
  }

  public int getY() {
    return this.l.ordinal() + 1;
  }

  // Setters
  public void setCol(EnumCol col) {
    this.c = col;
  }

  public void setLig(EnumLig ligne) {
    this.l = ligne;
  }

  // toString
  public String toString(){
    return "\tCase > c: " + this.c + "; l: " + this.l;
  }
}
