import java.net.ServerSocket;
import java.net.Socket;
import java.io.*;

public class Coup implements Serializable {
  private static final long serialVersionUID = 59771629617947L;

  private EnumCoup typeCoup;
  private EnumPiece piece;
  private boolean finPartie;
  private Action params;

  public Coup() {}

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

  public boolean getFinPartie() {
    return this.finPartie;
  }

  public String toString() {
    return "* Coup :\n\tTypeCoup: " + typeCoup + "\n\tPiece: " +  piece + "\n" + params.toString();
  }

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

  public void readFromNetwork(DataInputStream ids) {
    try {
      int boolValue = ids.readInt();

      if (boolValue == 0) {
        this.finPartie = false;
      }
      else {
        this.finPartie = true;
      }

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
