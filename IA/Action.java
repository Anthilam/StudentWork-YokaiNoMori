import java.io.Serializable;
import java.io.*;

public interface Action extends Serializable {
  public void sendToNetwork(DataOutputStream ods);
}
