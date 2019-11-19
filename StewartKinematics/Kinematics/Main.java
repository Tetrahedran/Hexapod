package org.pOH.StewartKinematics.Kinematics;

import org.pOH.StewartKinematics.Util.Quaternion;
import org.pOH.StewartKinematics.Util.Vector;

import java.util.List;

public class Main {

  public static void main(String[] args) {
    Hexapod hex = new Hexapod();
    for(int i = 0; i < 1 ; i++) {
      Vector relTrans = Vector.ZERO;
      Quaternion rotation = Quaternion.getQuaternionForRotation(Vector.RIGHT,  Math.PI / 4);
      List<Double> sollWinkel = hex.calcMotors(relTrans, rotation);
      String str = i + " ";
      for(int j = 0; j < sollWinkel.size(); j++) {
        double angleDegrees = (sollWinkel.get(j) * 57.3);
        str += "Motor " + j + ": " + angleDegrees + " ";
      }
      System.out.println(str);
    }
  }
}
