package org.pOH.StewartKinematics.Kinematics;

import org.pOH.StewartKinematics.Util.Quaternion;
import org.pOH.StewartKinematics.Util.Vector;

import java.util.LinkedList;
import java.util.List;

import static org.pOH.StewartKinematics.Util.HexapodStatics.*;

public class Hexapod {

  private Vector absoluteTransform;
  private Quaternion absoluteRotation;

  public Hexapod() {
    Vector a1 = anchorPositions.get(0);
    Vector m1 = motorPositions.get(0);
    double z_square = Math.pow(rodLength, 2) + Math.pow(hornLength,2) - Math.pow(a1.getX() - m1.getX(), 2) - Math.pow(a1.getY() - m1.getY(), 2);
    double z = Math.sqrt(z_square);
    //TODO
    absoluteTransform = new Vector(0,0, z - 0.013868);
    absoluteRotation = Quaternion.IDENTITY;
  }

  /**
   * Berechnet die Winkel der Motoren für eine Translation und eine Rotation
   * @param relativeTranslation Vector mit relativer Translationsbewegung
   * @param rotation Quaternion mit relativer Rotation
   * @return Liste mit allen Motorwinkeln
   */
  public List<Double> calcMotors(Vector relativeTranslation, Quaternion rotation) {
    absoluteTransform = absoluteTransform.add(relativeTranslation);
    absoluteRotation = absoluteRotation.hamiltonProduct(rotation);
    List<Double> sollWinkel = new LinkedList<>();
    for(int i = 0; i < motorPositions.size(); i++) {
      Vector effectiveRodLength = calcEffectiveRodLength(absoluteTransform, absoluteRotation, motorPositions.get(i), anchorPositions.get(i));
      sollWinkel.add(alphaK(effectiveRodLength, rodLength, hornLength, hornRotations.get(i)));
    }
    return sollWinkel;
  }

  /**
   * Berechnet die effektive Länge eines fiktiven Stabs zwischen einem Ankerpunkt und einer Motorwelle
   * @param absoluteTranslation Absolute Translation der Platform
   * @param rotation Absolute Rotation der Platform
   * @param motorPosition Position einer Motorwelle
   * @param anchorPosition Position des zugehörigen Ankers
   * @return
   */
  private Vector calcEffectiveRodLength(
      Vector absoluteTranslation,
      Quaternion rotation,
      Vector motorPosition,
      Vector anchorPosition) {
    Vector rotatedAnchor = anchorPosition.rotate(rotation);
    return absoluteTranslation.add(rotatedAnchor).subtract(motorPosition);
  }

  private double ek(double hornLengthK, Vector effectiveRodVectorK) {
    return 2.0 * hornLengthK * effectiveRodVectorK.getZ();
  }

  private double fk(double hornLengthK, Vector effectiveRodVectorK, double hornRotationK){
    return 2.0 * hornLengthK * (Math.cos(hornRotationK) * effectiveRodVectorK.getX() + Math.sin(hornRotationK) * effectiveRodVectorK.getY());
  }

  private double gk(Vector effectiveRodVectorK, double rodLength, double hornLength) {
    return Math.pow(effectiveRodVectorK.magnitude(), 2) - (Math.pow(rodLength, 2) - Math.pow(hornLength, 2));
  }

  protected double alphaK(double eK, double fK, double gK) {
    double angle = Math.asin(gK / Math.sqrt(Math.pow(eK,2) + Math.pow(fK, 2))) - Math.atan2(fK,eK);
    if(((Double)angle).isNaN() || angle == Double.POSITIVE_INFINITY || angle == Double.NEGATIVE_INFINITY) {
      throw new IllegalArgumentException("Ungültiger Winkel");
    }
    return angle;
  }

  /**
   * Berechnet den Motorwinkel eines Servos
   * @param effectiveRodVectorK Länge eines fiktiven Stabs zwischen Motorwelle und zugehörigem Anckerpunkt
   * @param rodLengthK Länge des realen Stabs zwischen dem Ausleger und dem zugehörigen Anckerpunkt
   * @param hornLengthK Länge des Auslegers
   * @param hornRotationK Rotation um die Z-Achse des Aulegers gegen die X-Achse
   * @return Rotationswinkel des Servos
   */
  private double alphaK(Vector effectiveRodVectorK, double rodLengthK, double hornLengthK, double hornRotationK) {
    double eK = ek(hornLengthK, effectiveRodVectorK);
    double fK = fk(hornLengthK, effectiveRodVectorK, hornRotationK);
    double gK = gk(effectiveRodVectorK, rodLengthK, hornLengthK);
    return alphaK(eK, fK, gK);
  }

  //TODO Hilfsmethoden für die Auslegung
  public List<Double> calcMotors() {
    return calcMotors(Vector.ZERO, Quaternion.IDENTITY);
  }

  public List<Vector> calcRealRodVectors(List<Double> motorRotations) {
    List<Vector> o = new LinkedList<>();
    for(int i = 0; i < 6; i++) {
      Vector a = getCurrentAnchorPosition(i);
      Vector h = motorPositions.get(i).add(getCurrentHornVector(i, motorRotations.get(i)));
      Vector rod = a.subtract(h);
      o.add(rod);
    }
    return o;
  }

  public List<Vector> calcCurrentHornVectors(List<Double> motorRotations) {
    List<Vector> hornRotations = new LinkedList<>();
    for(int i = 0; i < 6; i++) {
      hornRotations.add(getCurrentHornVector(i, motorRotations.get(i)));
    }
    return hornRotations;
  }

  private Vector getCurrentHornVector(int i, double motorRotation) {
    double x = Math.cos(motorRotation) * Math.cos(hornRotations.get(i));
    double y = Math.cos(motorRotation) * Math.sin(hornRotations.get(i));
    double z = Math.sin(motorRotation);
    return new Vector(x,y,z).multiply(hornLength);
  }

  private Vector getCurrentAnchorPosition(int i) {
    return absoluteTransform.add(anchorPositions.get(i).rotate(absoluteRotation));
  }

  public Quaternion getAbsoluteRotation() {
    return absoluteRotation;
  }

  public static double transLimit(Vector axis, double granularity) {
    Hexapod pod = new Hexapod();
    for(int i = 0;;i++) {
     try {
       pod.calcMotors(axis.normalize().multiply(granularity), Quaternion.IDENTITY);
     } catch (IllegalArgumentException e) {
       return i * granularity;
     }
}
  }

  public static double rotLimit(Vector axis, double angularGranularity) {
    Hexapod pod = new Hexapod();
    for(int i = 0;;i++) {
      try {
        pod.calcMotors(Vector.ZERO, Quaternion.getQuaternionForRotation(axis, angularGranularity));
      } catch (IllegalArgumentException e) {
        return i * angularGranularity;
      }
    }
  }
}
