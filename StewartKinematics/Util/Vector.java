package org.pOH.StewartKinematics.Util;

/**
 * Repräsentation eines dreidimensionalen Vectors mit x, y und z-Koordinate
 */
public class Vector {

  public static final Vector ZERO = new Vector(0,0,0);
  public static final Vector ONE = new Vector(1,1,1);
  public static final Vector RIGHT = new Vector(1,0,0);
  public static final Vector LEFT = new Vector(-1,0,0);
  public static final Vector FORWARD = new Vector(0,1,0);
  public static final Vector BACKWARD = new Vector(0,-1,0);
  public static final Vector UP = new Vector(0,0,1);
  public static final Vector DOWN = new Vector(0,0,-1);

  private final double x;
  private final double y;
  private final double z;

  /**
   * Konstructor für einen Vector
   * @param x Wert auf der x-Achse
   * @param y Wert auf der y-Achse
   * @param z Wert auf der z-Achse
   */
  public Vector(double x, double y, double z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  /**
   * Konstructor für einen Vector
   * z-Achse automatisch 0
   * @param x Wert auf der x-Achse
   * @param y Wert auf der y-Achse
   */
  public Vector(double x, double y) {
    this.x = x;
    this.y = y;
    this.z = 0;
  }

  public static Vector toVector(Quaternion quaternion) {
    if(Math.abs(quaternion.getW()) < 1e-15) {
      return new Vector(quaternion.getI(), quaternion.getJ(), quaternion.getK());
    }
    throw new IllegalArgumentException("Nur Quaternionen mit einem Skalar = 0 können in Vectoren umgewandelt werden, " + quaternion.toString());
  }

  public double getX() {
    return x;
  }

  public double getY() {
    return y;
  }

  public double getZ() {
    return z;
  }

  /**
   * Multipliziert diesen Vector mit einem Skalar
   * @param scalar Skalar mit dem Multipliziert werden soll
   * @return Vector der mit dem Skalar multipliziert wurde
   */
  public Vector multiply(double scalar) {
    double x = Round.round(this.x * scalar);
    double y = Round.round(this.y * scalar);
    double z = Round.round(this.z * scalar);
    return new Vector(x,y,z);
  }

  /**
   * Addiert einen anderen Vector zu diesem
   * @param other Anderer Vector welcher addiert werden soll
   * @return Dieser Vector + other
   */
  public Vector add(Vector other) {
    return new Vector(this.x + other.x, this.y + other.y, this.z + other.z);
  }

  /**
   * Subtrahiert einen Vector von diesem
   * @param other Vector der subtrahiert werden soll
   * @return Dieser Vector - other
   */
  public Vector subtract(Vector other) {
    return add(other.multiply(-1));
  }

  /**
   * Normalilisiert diesen Vector
   * @return Vector der dieselbe Richtung wie dieser und die Länge 1 hat
   */
  public Vector normalize() {
    return this.multiply(1 / this.magnitude());
  }

  /**
   * Bildet das Kreuzprodukt zwischen diesem Vector und einem anderen
   * @param other Anderer Vector mit dem das Kreuzprodukt gebildet werden soll
   * @return Vector der orthogonal auf diesem und dem anderen Vector steht
   * und dessen Länge dem Flächeninhalt des eingeschlossenen Parallelogramms entspricht
   */
  public Vector cross(Vector other) {
    double x = this.y * other.z - this.z * other.y;
    double y = this.z * other.x - this.x * other.z;
    double z = this.x * other.y - this.y * other.x;
    return new Vector(x,y,z);
  }

  /**
   * @return Die Länge dieses Vectors
   */
  public double magnitude() {
    return Math.sqrt(this.dot(this));
  }

  /**
   * Bildet das Skalarprodukt zwischen diesem Vector und einem anderen
   * @param other Vector mit dem das Skalarprodukt gebildet werden soll
   */
  public double dot(Vector other) {
    return this.x * other.x + this.y * other.y + this.z * other.z;
  }

  /**
   * Berechnet den Winkel zwischen diesem Vector und einem anderen
   * @param other Anderer Vector
   * @return Der Cosinus des eingeschlossenen Winkels
   */
  public double cosine(Vector other) {
    return (this.dot(other)) / (this.magnitude() * other.magnitude());
  }

  /**
   * Konvertiert einen Vector in ein Array
   * @return Ein Array mit [0] = x, [1] = y und [2] = z
   */
  public double[] toArray() {
    return new double[] {x,y,z};
  }

  /**
   * Rotiert einen Vector
   * @param rotation Quaternion welches die Rotation beschreibz
   * @return Den rotierten Vector
   */
  public Vector rotate(Quaternion rotation) {
    return Vector.toVector(rotation.hamiltonProduct(Quaternion.toQuaternion(this)).hamiltonProduct(rotation.konjugate()));
  }

  @Override
  public String toString() {
    return "x:" + x + ", y:" + y + ", z:" + z;
  }
}
