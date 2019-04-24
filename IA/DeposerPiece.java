import java.io.Serializable;

public class DeposerPiece implements Action,Serializable {
  private Case casePos;

  public Case getCasePos(){
    return this.casePos;
  }

  public void setCasePose(Case casePo){
    this.casePos = casePo;
  }
}
