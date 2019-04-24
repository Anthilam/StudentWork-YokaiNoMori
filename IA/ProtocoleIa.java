import java.io.Serializable;
public class ProtocoleIa implements Serializable {
  private EnumCoup typeCoup;
  private EnumPiece piece;
  private Action params;

  public void setTypeCoup(EnumCoup tCoup){
    this.typeCoup = typeCoup;
  }

  public void setEnumPiece(EnumPiece p){
    this.piece = p;
  }

  public void setParams(Action act){
    this.params = act;
  }

  public EnumCoup getTypeCoup(){
    return this.typeCoup;
  }

  public EnumPiece getPiece(){
    return this.piece;
  }

  public Action getParams(){
    return this.params;
  }
}
