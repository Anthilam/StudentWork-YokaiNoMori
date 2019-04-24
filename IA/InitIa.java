import java.io.Serializable;
public class InitIa extends Serializable{
  private int sens; // 0 == nord ; 1 == sud

  public int getSens(){
    return this.sens;
  }

  public void setSens(int sensJoueur){
    this.sens = sensJoueur;
  }
}
