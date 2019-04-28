import java.io.Serializable;

public class Case implements Serializable {
  private EnumCol c;
  private EnumLig l;

  public Case(EnumCol col, EnumLig lig){
    this.c = col;
    this.l = lig;
  }
  public EnumCol getCol() {
    return this.c;
  }

  public EnumLig getLig() {
    return this.l;
  }

  public void setCol(EnumCol col) {
    this.c = col;
  }

  public void setLig(EnumLig ligne) {
    this.l = ligne;
  }

  public String toString(){
    return "Case : colonne : "+this.c+"; ligne : "+this.l;
  }
}
