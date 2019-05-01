import java.io.Serializable;
import java.io.*;
public class DeplPiece implements Action,Serializable{
  private Case caseDep;
  private Case caseArr;
  private boolean estCapt;

  public DeplPiece(Case caseD,Case caseA,boolean cap){
    this.caseDep = caseD;
    this.caseArr = caseA;
    this.estCapt = cap;
  }

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

  public void sendToNetwork(DataOutputStream ods){
    try{
      ods.writeInt(this.caseDep.getCol().ordinal());
      ods.writeInt(this.caseDep.getLig().ordinal());
      ods.writeInt(this.caseArr.getCol().ordinal());
      ods.writeInt(this.caseArr.getLig().ordinal());
      ods.writeBoolean(this.estCapt);
    }catch(IOException e){
      System.out.println(e);
    }
  }
}
