import java.io.Serializable;
public class Coup implements Serializable {
  private EnumCoup typeCoup;
  private EnumPiece piece;
  private Action params;

  public Coup(EnumCoup tpCoup, EnumPiece p, Action param) {
    this.typeCoup = tpCoup;
    this.piece = p;
    this.params = param;
  }

  public void setTypeCoup(EnumCoup typeCoup) {
    this.typeCoup = typeCoup;
  }

  public void setEnumPiece(EnumPiece p) {
    this.piece = p;
  }

  public void setParams(Action act) {
    this.params = act;
  }

  public EnumCoup getTypeCoup() {
    return this.typeCoup;
  }

  public EnumPiece getPiece() {
    return this.piece;
  }

  public Action getParams() {
    return this.params;
  }
}
