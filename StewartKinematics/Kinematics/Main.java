package org.pOH.StewartKinematics.Kinematics;

import org.pOH.StewartKinematics.Util.Quaternion;
import org.pOH.StewartKinematics.Util.Vector;

import java.util.List;

public class Main {

  public static void main(String[] args) {
    Hexapod hex = new Hexapod();
    for(int i = 0; i < 25 ; i++) {
      Vector relTrans = Vector.ONE.multiply(0.01);
      Quaternion rotation = Quaternion.getQuaternionForRotation(Vector.UP, 0 * 2.0 * Math.PI / 360);
      List<Double> sollWinkel = hex.calcMotors(relTrans, rotation);
      String str = i + " ";
      for(int j = 0; j < sollWinkel.size(); j++) {
        double angleDegrees = (360 * sollWinkel.get(j) / (2.0 * Math.PI));
        str += "Motor " + j + ": " + angleDegrees + " ";
      }
      System.out.println(str);
    }
  }
}
