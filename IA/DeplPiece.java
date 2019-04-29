import java.io.Serializable;

public class DeplPiece implements Action,Serializable{
  private Case caseDep;
  private Case caseArr;
  private boolean estCapt;
  
  public boolean getEstCapt() {
    return this.estCapt;
  }

  public Case getCaseDep() {
    return this.caseDep;
  }

  public Case getCaseArr() {
    return this.caseArr;
  }

  public void setEstCapt(boolean cpt) {
    this.estCapt = cpt;
  }

  public void setCaseDep(Case caseDepart) {
    this.caseDep = caseDepart;
  }

  public void setCaseArr(Case caseArrive) {
    this.caseArr = caseArrive;
  }
}
