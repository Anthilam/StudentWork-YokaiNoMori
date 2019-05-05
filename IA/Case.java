import java.io.Serializable;

public class Case implements Serializable {
  private EnumCol c;
  private EnumLig l;

  public Case(EnumCol col, EnumLig lig){
    this.c = col;
    this.l = lig;
  }

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

  public EnumCol getCol() {
    return this.c;
  }

  public EnumLig getLig() {
    return this.l;
  }

  // Get column in int format
  public int getX() {
    return this.c.ordinal() + 1;
  }

  // Get line in int format
  public int getY() {
    return this.l.ordinal() + 1;
  }

  public void setCol(EnumCol col) {
    this.c = col;
  }

  public void setLig(EnumLig ligne) {
    this.l = ligne;
  }

  public String toString(){
    return "\tCase > c: " + this.c + "; l: " + this.l;
  }
}
