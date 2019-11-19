package org.pOH.StewartKinematics.Util;

import org.pOH.StewartKinematics.Kinematics.Hexapod;

import java.util.ArrayList;
import java.util.List;

import static org.pOH.StewartKinematics.Util.HexapodStatics.*;

public class ForceCalculator {

  public static final Vector MaxTorgueX = new Vector(279,0,0);
  public static final Vector MaxTorgueY = new Vector(0,220.5,61.2);
  public static final Vector MaxTorgueZ = new Vector(0,61.2, 77.4);
  public static final Vector AntiGravity = Vector.UP.multiply(250 * 10);

  public double maxTransZ = Hexapod.transLimit(Vector.UP, 0.0001);
  public double maxTransX = Hexapod.transLimit(Vector.RIGHT, 0.0001);
  public double maxTransY = Hexapod.transLimit(Vector.FORWARD, 0.0001);

  public static double maxRotY = Hexapod.rotLimit(Vector.FORWARD, 0.0001);
  public static double maxRotX = Hexapod.rotLimit(Vector.RIGHT, 0.0001);
  public static double maxRotZ = Hexapod.rotLimit(Vector.UP, 0.0001);

  public static void main(String[] args) {
    Hexapod pod = new Hexapod();
    System.out.println(maxRotX * 57.3);
    System.out.println(maxRotY * 57.3);
    System.out.println(maxRotZ * 57.3);
    for(int i = 0; i < 45;i++) {
      pod.calcMotors(Vector.ZERO.multiply(0.01), Quaternion.getQuaternionForRotation(Vector.RIGHT, 1 / 57.3));
      for (double d : getTorgueForAllMotors(pod)) {
        System.out.print(d + " ");
      }
      System.out.println();
    }
  }

  public static List<Double> getTorgueForAllMotors() {
    return getTorgueForAllMotors(new Hexapod());
  }

  public static List<Double> getTorgueForAllMotors(Hexapod pod) {
    calcForce();
    calcTorgue();
    Vector totalForce = Vector.UP.multiply(10 * 250);
    Vector totalTorgue = new Vector(0,0, 0);
    Hexapod hexapod = pod;
    List<Vector> o = hexapod.calcRealRodVectors(pod.calcMotors());
    double[][] forceMatrix = createForceMatrix(anchorPositions, o, hexapod.getAbsoluteRotation());
    double[] results = new double[]{totalForce.getX(), totalForce.getY(), totalForce.getZ(),
        totalTorgue.getX(), totalTorgue.getY(), totalTorgue.getZ()};
    double[] forcesOnRods = solveForceMatrix(forceMatrix, results);
    List<Vector> hornVectors = hexapod.calcCurrentHornVectors(pod.calcMotors());
    List<Vector> shaftVectors = getShaftVectors();
    List<Double> motorTorgue = new ArrayList<>();
    for(int i = 0;i < 6; i++){
      double forceOnRod = forcesOnRods[i];
      double torgue = getTorgueForMotor(o.get(i).multiply(forceOnRod), hornVectors.get(i), shaftVectors.get(i));
      motorTorgue.add(torgue);
    }
    return motorTorgue;
  }

  private static double getTorgueForMotor(Vector force, Vector hornVector, Vector shaftVector) {
    return hornVector.cross(force).dot(shaftVector.normalize().multiply(-1));
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

  private static void calcForce() {
    //TODO
  }

  private static void calcTorgue() {
    //TODO
  }
}
