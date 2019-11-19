package org.pOH.StewartKinematics.Util;

import org.pOH.StewartKinematics.Kinematics.LinearHexapod;

import java.util.List;

import static org.pOH.StewartKinematics.Util.HexapodStatics.*;

public class LinearForceCalculator {
  public static final Vector MaxTorgueX = new Vector(279,0,0);
  public static final Vector MaxTorgueY = new Vector(0,220.5,61.2);
  public static final Vector MaxTorgueZ = new Vector(0,61.2, 77.4);
  public static final Vector AntiGravity = Vector.UP.multiply(250 * 10);

  public double maxTransZ = LinearHexapod.transLimit(Vector.UP, 0.0001);
  public double maxTransX = LinearHexapod.transLimit(Vector.RIGHT, 0.0001);
  public double maxTransY = LinearHexapod.transLimit(Vector.FORWARD, 0.0001);

  public static double maxRotY = LinearHexapod.rotLimit(Vector.FORWARD, 0.0001);
  public static double maxRotX = LinearHexapod.rotLimit(Vector.RIGHT, 0.0001);
  public static double maxRotZ = LinearHexapod.rotLimit(Vector.UP, 0.0001);

  public static void main(String[] args) {
    LinearHexapod pod = new LinearHexapod();
    System.out.println(maxRotX * 57.3);
    System.out.println(maxRotY * 57.3);
    System.out.println(maxRotZ * 57.3);
    for(int i = 0; i < 45;i++) {
      pod.calcMotors(Vector.ZERO.multiply(0.01), Quaternion.getQuaternionForRotation(Vector.UP, 1 / 57.3));
      for (double d : getForceForAllMotors(pod)) {
        System.out.print(d + " N  ");
      }
      System.out.println();
    }
  }

  public static double[] getTorgueForAllMotors() {
    return getForceForAllMotors(new LinearHexapod());
  }

  public static double[] getForceForAllMotors(LinearHexapod pod) {
    Vector totalForce = Vector.UP.multiply(10 * 250);
    Vector totalTorgue = MaxTorgueZ;
    LinearHexapod hexapod = pod;
    List<Vector> o = hexapod.calcEffectiveRodLength(pod);
    double[][] forceMatrix = createForceMatrix(anchorPositions, o, hexapod.getAbsoluteRotation());
    double[] results = new double[]{totalForce.getX(), totalForce.getY(), totalForce.getZ(),
        totalTorgue.getX(), totalTorgue.getY(), totalTorgue.getZ()};
    return solveForceMatrix(forceMatrix, results);
  }

  private static double[] solveForceMatrix(double[][] forceMatrix, double[] results) {
    return GaussianElimination.lsolve(forceMatrix, results);
  }

  private static double[][] createForceMatrix(List<Vector> anchorPositions, List<Vector> o, Quaternion absoluteRotation) {
    double[][] matrix = new double[6][6];
    //Eintragung der Kräfte
    for(int row = 0; row < 3; row++) {
      for(int column = 0; column < 6; column++) {
        matrix[row][column] = o.get(column).toArray()[row];
      }
    }
    //Eintragung der Momente
    for(int row = 0; row < 3; row++) {
      for(int column = 0; column < 6; column++) {
        matrix[row + 3][column] = anchorPositions.get(column).rotate(absoluteRotation).cross(o.get(column)).toArray()[row];
      }
    }
    return matrix;
  }
}
