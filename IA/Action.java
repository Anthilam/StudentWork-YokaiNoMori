/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * Action.java - An interface that specifies an action allowed to be send
 * over the network
 *
 */

import java.io.Serializable;
import java.io.*;

public interface Action extends Serializable {
  public void sendToNetwork(DataOutputStream ods);
}
