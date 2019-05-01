import java.io.Serializable;

public class DeposerPiece implements Action,Serializable {
  private Case casePos;

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
