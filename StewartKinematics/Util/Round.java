package org.pOH.StewartKinematics.Util;

import java.math.BigDecimal;
import java.math.RoundingMode;

public class Round {
  public static double round(double number, int positions) {
    BigDecimal bd = BigDecimal.valueOf(number);
    bd = bd.setScale(positions, RoundingMode.HALF_DOWN);
    return bd.doubleValue();
  }

  public static double round(double number) {
    return round(number, 15);
  }
}
