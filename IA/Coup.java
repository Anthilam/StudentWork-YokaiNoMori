import java.net.ServerSocket;
import java.net.Socket;
import java.io.*;

public class Coup implements Serializable {
  private EnumCoup typeCoup;
  private EnumPiece piece;
  private Action params;

  public Coup(){

  }
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

  public void sendToNetwork(DataOutputStream ods){
    try{
      ods.writeInt(this.typeCoup.ordinal());
      ods.writeInt(this.piece.ordinal());
    }catch(IOException e){
      System.out.println(e);
    }
    this.params.sendToNetwork(ods);
  }

  public void readFromNetwork(DataInputStream ids){
    try{
      this.typeCoup = EnumCoup.values()[ids.readInt()];
      this.piece = EnumPiece.values()[ids.readInt()];
      if(this.typeCoup == EnumCoup.DEPLACER){
        Case cFrom = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);
        Case cTo = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);
        boolean captured = ids.readBoolean();
        this.params = new DeplPiece(cFrom,cTo,captured);
      }else{
        Case cOnto = new Case(EnumCol.values()[ids.readInt()],EnumLig.values()[ids.readInt()]);
        this.params = new DeposerPiece(cOnto);
      }
    }catch(IOException e){
      System.out.println("* Error : IOException");
      e.printStackTrace();
      System.exit(-1);
    }
  }
}
