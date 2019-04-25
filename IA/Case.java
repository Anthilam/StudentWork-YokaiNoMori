import java.io.Serializable;

public class Case implements Serializable {
  private EnumCol c;
  private EnumLig l;

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
}
