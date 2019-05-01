import java.io.Serializable;
import java.io.*;

public class DeposerPiece implements Action,Serializable {
  private Case casePos;

  public DeposerPiece(Case c){
    this.casePos = c;
  }
  public Case getCasePos() {
    return this.casePos;
  }

  public void setCasePose(Case casePo) {
    this.casePos = casePo;
  }

  public void sendToNetwork(DataOutputStream ods){
    try{
      ods.writeInt(this.casePos.getCol().ordinal());
      ods.writeInt(this.casePos.getLig().ordinal());
    }catch(IOException e){
      System.out.println(e);
    }
  }
}
