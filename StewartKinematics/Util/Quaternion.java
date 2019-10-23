package org.pOH.StewartKinematics.Util;

public class Quaternion {

  public static final Quaternion IDENTITY = new Quaternion(1,0,0,0);

  private final double w;
  private final double i;
  private final double j;
  private final double k;

  public Quaternion(double w, double i, double j, double k) {
    this.w = w;
    this.i = i;
    this.j = j;
    this.k = k;
  }

  public static Quaternion getQuaternionForRotation(Vector axis, double angle) {
    axis = axis.normalize();
    double halfAngle = angle / 2;
    double w = Round.round(Math.cos(halfAngle));
    double sinHalfAngle = Math.sin(halfAngle);
    double i = Round.round(axis.getX() * sinHalfAngle);
    double j = Round.round(axis.getY() * sinHalfAngle);
    double k = Round.round(axis.getZ() * sinHalfAngle);
    return new Quaternion(w, i, j, k);
  }

  public static Quaternion toQuaternion(Vector vector) {
    return new Quaternion(0, vector.getX(), vector.getY(), vector.getZ());
  }

  public double getW() {
    return w;
  }

  public double getI() {
    return i;
  }

  public double getJ() {
    return j;
  }

  public double getK() {
    return k;
  }

  public Quaternion konjugate() {
    return new Quaternion(this.w, -this.i, -this.j, -this.k);
  }

  public Quaternion hamiltonProduct(Quaternion other) {
    double w = this.w * other.w - this.i * other.i - this.j * other.j - this.k * other.k;
    double i = this.w * other.i + this.i * other.w + this.j * other.k - this.k * other.j;
    double j = this.w * other.j - this.i * other.k + this.j * other.w + this.k * other.i;
    double k = this.w * other.k + this.i * other.j - this.j * other.i + this.k * other.w;
    return new Quaternion(w, i, j, k);
  }

  @Override
  public String toString() {
    return "w: " + w + ", i:" + i + ", j:" + j + ", k:" + k;
  }
}
