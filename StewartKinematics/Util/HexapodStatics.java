package org.pOH.StewartKinematics.Util;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

/**
 * Hilfsklasse zur Kalkulation wichtiger Werte im Bezug auf den Hexapoden
 */
public class HexapodStatics {
  static double sqrt3 = Math.sqrt(3);

  /**
   * Seitenlänge des zugrundeliegenden gleichseitigen Dreiecks der Bodenplatte b
   */
  public static final double ab = 2.0;

  /**
   * Seitenlänge des zugrundeliegenden gleichseitigen Dreiecks der Platform p
   */
  public static final double ap = 2.0;

  /**
   * Länge des festen Stabs der am Ausleger befestigt ist und zu einem Ancker auf der Platform führt
   */
  public static final double rodLength = 1;

  /**
   * Länge des Auslegers der an einer Motorwelle befestigt ist
   */
  public static final double hornLength = 0.25;

  /**
   * Ortsvektoren der Eckpunkte des zugrundeliegenden Dreiecks für die Basis
   */
  public static Vector Ab,Bb,Cb;

  /**
   * Ortsvektoren der Eckpunkte des zugrundeliegenden Dreiecks für die Platform
   * Die Platform ist dabei um 60 Grad gegen die Basis gedreht
   */
  public static Vector Ap,Bp,Cp;

  /**
   * Abstandskoeffizent für die Positionierung der Motoren auf der Basis
   * 0.5 = Positionierung auf der exakten Mitte der Seiten des zugrundeliegenden Dreiecks
   */
  public static final double bPosKoeff = 2.0 / 5.0;

  /**
   * Abstandskoeffizent für die Positionierung der Anckerpunkte auf der Platform
   * 0.5 = Positionierung auf der exakten Mitte der Seiten des zugrundeliegenden Dreiecks
   */
  public static final double pPosKoeff = 2.0 / 5.0;

  /**
   * Positionen der Endpunkte der Motorwellen auf b im lokalen Koordinatensystem von b
   * mit Ob = Umkreismittelpunkt der Bodenplatte
   */
  public static List<Vector> motorPositions = new LinkedList<>();

  /**
   * Positionen der Anckerpunkte für die Stäbe auf p im lokalen Koordinatensystem von p
   * mit Op = Umkreismittelpunkt der Platform
   */
  public static List<Vector> anchorPositions = new LinkedList<>();

  /**
   * Rotation der Servoausleger um die z-Achse gegen die x-Achse
   */
  public static List<Double> hornRotations = new LinkedList<>();

  static {
    berechneOrtsvektorenEckenDreiecke(ab,ap);
    berechneOrtsvektorenMotoren(bPosKoeff);
    berechneOrtsvektorenAncker(pPosKoeff);
    berechneWinkelAuslegerXAchse();
  }

  /**
   * Berechnet die Vectoren der Eckpunkte der Dreiecke für die Basis und die Platform
   * @param ab Seitenlänge des gleichseitigen Dreiecks der Basis
   * @param ap Seitenlänge des gleichseitigen Dreiecks der Platform
   */
  private static void berechneOrtsvektorenEckenDreiecke(double ab, double ap) {
    // Ecken des Dreiecks der Basis
    Vector Ab, Bb, Cb;
    double innkreisRadiusB = sqrt3 / 6.0 * ab;
    double umkreisRadiusB = 2.0 * innkreisRadiusB;
    Ab = new Vector(-ab / 2.0,-innkreisRadiusB);
    Bb = new Vector(ab / 2.0, -innkreisRadiusB);
    Cb = new Vector(0, umkreisRadiusB);
    HexapodStatics.Ab = Ab;
    HexapodStatics.Bb = Bb;
    HexapodStatics.Cb = Cb;

    // Ecken des Dreiecks der Platform
    Vector Ap, Bp, Cp;
    double innkreisRadiusP = sqrt3 / 6.0 * ap;
    double umkreisRadiusP =  2.0 * innkreisRadiusP;
    Ap = new Vector(0, -umkreisRadiusP);
    Bp = new Vector(ap / 2.0, innkreisRadiusP);
    Cp = new Vector(-ap / 2.0, innkreisRadiusP);
    HexapodStatics.Ap = Ap;
    HexapodStatics.Bp = Bp;
    HexapodStatics.Cp = Cp;

  }

  /**
   * Berechnet die Ortsvektoren zu den Endpunkten der Motorwellen
   * @param bPosKoeff Koeffizient zur Positionierung der Motoren
   */
  private static void berechneOrtsvektorenMotoren(double bPosKoeff) {
    double invBPosKoeff = 1 - bPosKoeff;

    Vector AB = Bb.subtract(Ab);
    Vector BC = Cb.subtract(Bb);
    Vector CA = Ab.subtract(Cb);

    Vector m1 = Ab.add(AB.multiply(invBPosKoeff));
    Vector m2 = Ab.add(AB.multiply(bPosKoeff));
    Vector m3 = Cb.add(CA.multiply(invBPosKoeff));
    Vector m4 = Cb.add(CA.multiply(bPosKoeff));
    Vector m5 = Bb.add(BC.multiply(invBPosKoeff));
    Vector m6 = Bb.add(BC.multiply(bPosKoeff));

    motorPositions = Arrays.asList(m1,m2,m3,m4,m5,m6);
  }

  /**
   * Berechnet die Ortsvektoren zu den Anckerpunkten auf der Platform
   * @param pPosKoeff Koeffizient zur Positionierung der Anckerpunkte
   */
  private static void berechneOrtsvektorenAncker(double pPosKoeff) {
    double invPPosKoeff = 1 - pPosKoeff;

    Vector AB = Bp.subtract(Ap);
    Vector BC = Cp.subtract(Bp);
    Vector CA = Ap.subtract(Cp);

    Vector a1 = Ap.add(AB.multiply(pPosKoeff));
    Vector a2 = Cp.add(CA.multiply(invPPosKoeff));
    Vector a3 = Cp.add(CA.multiply(pPosKoeff));
    Vector a4 = Bp.add(BC.multiply(invPPosKoeff));
    Vector a5 = Bp.add(BC.multiply(pPosKoeff));
    Vector a6 = Ap.add(AB.multiply(invPPosKoeff));

    anchorPositions = Arrays.asList(a1,a2,a3,a4,a5,a6);
  }

  /**
   * Berechnet den Drehwinkel um die Z-Achse des Auslegers gegen die X-Achse
   */
  private static void berechneWinkelAuslegerXAchse() {
    //Inkorrekte Winkelberechnung
    /**Vector AB = Bb.subtract(Ab);
    Vector BC = Cb.subtract(Bb);
    Vector CA = Ab.subtract(Cb);

    double beta1 = Math.acos(AB.cosine(Vector.RIGHT));
    double beta2 = Math.acos(AB.cosine(Vector.RIGHT));

    double beta3 = Math.acos(BC.cosine(Vector.RIGHT));
    double beta4 = Math.acos(BC.multiply(-1).cosine(Vector.RIGHT));

    double beta5 = Math.acos(CA.cosine(Vector.RIGHT));
    double beta6 = Math.acos(CA.multiply(-1).cosine(Vector.RIGHT));*/

    double beta1 = Math.PI;
    double beta2 = 0.0;

    double beta3 = 60 / 360.0 * 2 * Math.PI;
    double beta4 = -120 / 360.0 * 2 * Math.PI;

    double beta5 = -60 / 360.0 * 2 * Math.PI;
    double beta6 = 120 / 360.0 * 2 * Math.PI;

    hornRotations = Arrays.asList(beta1,beta2,beta3,beta4,beta5,beta6);
  }

  /**
   * Berechnet die Richtungsvectoren der Motorachsen in Richtung Ursprung
   * @return Ein Array mit den Richtungsvectoren der Motorachsen
   */
  public static List<Vector> getShaftVectors(){
    List<Vector> shaftVectors;
    Vector s1 = Ab.add(Bb.subtract(Ab).multiply(0.5));
    Vector s2 = s1;
    Vector s3 = Ab.add(Cb.subtract(Ab).multiply(0.5));
    Vector s4 = s3;
    Vector s5 = Bb.add(Cb.subtract(Bb).multiply(0.5));
    Vector s6 = s5;

    shaftVectors = Arrays.asList(s1,s2,s3,s4,s5,s6);

    return shaftVectors;
  }

}
