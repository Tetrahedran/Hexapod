package org.pOH.StewartKinematics.Kinematics;

import org.pOH.StewartKinematics.Util.Quaternion;
import org.pOH.StewartKinematics.Util.Vector;

import java.util.LinkedList;
import java.util.List;

import static org.pOH.StewartKinematics.Util.LinearHexapodStatics.*;

public class LinearHexapod {

  private Vector absoluteTransform;
  private Quaternion absoluteRotation;
  private final double minRodLength;
  private final double maxRodLength;

  public LinearHexapod() {
    //TODO
    absoluteTransform = new Vector(0,0, calcStartHeight());
    absoluteRotation = Quaternion.IDENTITY;
    minRodLength = staticAktuatorLength;
    maxRodLength = staticAktuatorLength + welle;
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
    List<Double> sollLängen = new LinkedList<>();
    for(int i = 0; i < motorPositions.size(); i++) {
      Vector effectiveRodLength = calcEffectiveRodLength(absoluteTransform, absoluteRotation, motorPositions.get(i), anchorPositions.get(i));
      if (effectiveRodLength.magnitude() < minRodLength) {
        throw new IllegalArgumentException("Minimale Wellenlänge unterschritten");
      }
      else if(effectiveRodLength.magnitude() > maxRodLength){
        throw new IllegalArgumentException("Maximale Wellenlänge überschriiten");
      }
      sollLängen.add(i, effectiveRodLength.magnitude());
    }
    return sollLängen;
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

  private double calcStartHeight(){
    return Math.sqrt(Math.pow(staticAktuatorLength + welle / 2,2)
        - Math.pow(anchorPositions.get(0).getX() - motorPositions.get(0).getX(),2)
        - Math.pow(anchorPositions.get(0).getY() - motorPositions.get(0).getY(),2));
  }


  //TODO Hilfsmethoden für die Auslegung
  public List<Vector> calcEffectiveRodLength(LinearHexapod pod){
    List<Vector> ret = new LinkedList<>();
    for(int i = 0;i < 6 ;i++){
      ret.add(i, calcEffectiveRodLength(pod.absoluteTransform, pod.absoluteRotation, motorPositions.get(i), anchorPositions.get(i).normalize()));
    }
    return ret;
  }

  public List<Double> calcMotors() {
    return calcMotors(Vector.ZERO, Quaternion.IDENTITY);
  }

  private Vector getCurrentAnchorPosition(int i) {
    return absoluteTransform.add(anchorPositions.get(i).rotate(absoluteRotation));
  }

  public Quaternion getAbsoluteRotation() {
    return absoluteRotation;
  }

  public static double transLimit(Vector axis, double granularity) {
    LinearHexapod pod = new LinearHexapod();
    for(int i = 0;;i++) {
      try {
        pod.calcMotors(axis.normalize().multiply(granularity), Quaternion.IDENTITY);
      } catch (IllegalArgumentException e) {
        return i * granularity;
      }
    }
  }

  public static double rotLimit(Vector axis, double angularGranularity) {
    LinearHexapod pod = new LinearHexapod();
    for(int i = 0;;i++) {
      if(i * angularGranularity > (Math.PI)){
        return Math.PI;
      }
      try {
        pod.calcMotors(Vector.ZERO, Quaternion.getQuaternionForRotation(axis, angularGranularity));
      } catch (IllegalArgumentException e) {
        return i * angularGranularity;
      }
    }
  }
}
