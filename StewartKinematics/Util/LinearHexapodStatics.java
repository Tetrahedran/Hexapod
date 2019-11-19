package org.pOH.StewartKinematics.Util;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

@SuppressWarnings("Duplicates")
public class LinearHexapodStatics {
  static double sqrt3 = Math.sqrt(3);

  /**
   * Seitenlänge des zugrundeliegenden gleichseitigen Dreiecks der Bodenplatte b
   */
  public static final double ab = 1;

  /**
   * Seitenlänge des zugrundeliegenden gleichseitigen Dreiecks der Platform p
   */
  public static final double ap = 1;

  /**
   * Länge des Aktuators bei voll eingefahrener Welle
   */
  public static final double staticAktuatorLength = 0.5;

  /**
   * Maximale Länge der Welle des linearen Aktuators
   */
  public static final double welle = 0.5;

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
  public static final double bPosKoeff =0.5 ;

  /**
   * Abstandskoeffizent für die Positionierung der Anckerpunkte auf der Platform
   * 0.5 = Positionierung auf der exakten Mitte der Seiten des zugrundeliegenden Dreiecks
   */
  public static final double pPosKoeff = 0.5;

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

  static {
    berechneOrtsvektorenEckenDreiecke(ab,ap);
    berechneOrtsvektorenMotoren(bPosKoeff);
    berechneOrtsvektorenAncker(pPosKoeff);
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
    LinearHexapodStatics.Ab = Ab;
    LinearHexapodStatics.Bb = Bb;
    LinearHexapodStatics.Cb = Cb;

    // Ecken des Dreiecks der Platform
    Vector Ap, Bp, Cp;
    double innkreisRadiusP = sqrt3 / 6.0 * ap;
    double umkreisRadiusP =  2.0 * innkreisRadiusP;
    Ap = new Vector(0, -umkreisRadiusP);
    Bp = new Vector(ap / 2.0, innkreisRadiusP);
    Cp = new Vector(-ap / 2.0, innkreisRadiusP);
    LinearHexapodStatics.Ap = Ap;
    LinearHexapodStatics.Bp = Bp;
    LinearHexapodStatics.Cp = Cp;

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
}
