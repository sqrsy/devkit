/**
 * Reads an analog input pin and calculates the voltage in millivolts (mV), 
 * optionally accounting for a voltage divider.
 *
 * This function reads the analog input from the specified pin and converts 
 * it to a voltage value in millivolts. If resistor values for a voltage divider 
 * (r1 and r2) are provided, the function back-calculates the real voltage before 
 * division. If no resistor values are given, a default conversion factor of 4.9 
 * mV per unit is applied based on a 10-bit ADC (analog-to-digital converter) 
 * with a 5V reference.
 *
 * @param pin_in The analog input pin to read from.
 * @param r1 The resistance value (in ohms) of the resistor connected between 
 *           the voltage source and the analog pin. Defaults to 0 (no voltage divider).
 * @param r2 The resistance value (in ohms) of the resistor connected between 
 *           the analog pin and ground. Defaults to 0 (no voltage divider).
 * @param debug A boolean flag that, when true, prints the calculated mV value 
 *              to the serial monitor for debugging purposes. Defaults to false.
 * @return The calculated voltage in millivolts (mV).
 */
float read_analog_mV(int pin_in, int r1 = 0, int r2 = 0, bool debug = false) {

  int x = analogRead(pin_in);
  x = analogRead(pin_in);
  float mV = 0;
  if (r1 == 0 & r2 == 0) {
    mV = x * 4.9;
  } else {
    mV = x * 4.9 * (r1 + r2) / r2;
  }

  if (debug) {
    Serial.print("Current value (read_analog_mV): ");
    Serial.print(mV);
    Serial.println("");
  }

  return (mV);
}

/**
 * Reads an analog input pin, applies a smoothing filter using a history buffer, 
 * and returns the average voltage in millivolts (mV).
 *
 * This function smooths the voltage readings from the specified analog pin using 
 * an averaging filter. It maintains a history of the last 8 readings, updates the 
 * history with the latest reading, and calculates the average to reduce noise 
 * in the signal. The voltage is adjusted based on the provided resistor values 
 * if a voltage divider is used.
 *
 * @param pin_in The analog input pin to read from.
 * @param read_history An array of 8 integers to store the history of recent readings.
 *                     This array should be maintained between function calls to 
 *                     preserve the smoothing effect.
 * @param r1 The resistance value (in ohms) of the resistor connected between 
 *           the voltage source and the analog pin. Defaults to 0 (no voltage divider).
 * @param r2 The resistance value (in ohms) of the resistor connected between 
 *           the analog pin and ground. Defaults to 0 (no voltage divider).
 * @param debug A boolean flag that, when true, prints the smoothed mV value 
 *              to the serial monitor for debugging purposes. Defaults to false.
 * @return The smoothed voltage in millivolts (mV) as an integer.
 */
int read_analog_mV_smooth(int pin_in, int read_history[8], int r1 = 0, int r2 = 0, bool debug = false) {

  // track history of input
  for (int i = 0; i < 8; i++) {  // move history back one step
    read_history[i] = read_history[i + 1];
  }
  read_history[7] = read_analog_mV(pin_in, r1, r2);  // update history with new value

  // calculate average of history
  long incoming_cv = 0;
  for (int i = 0; i < 8; i++) {  // calculate grand sum of history
    incoming_cv = incoming_cv + read_history[i];
  }
  incoming_cv = incoming_cv / 8;

  if (debug) {
    Serial.print("Current value (read_analog_mV_smooth): ");
    Serial.print(incoming_cv);
    Serial.println("");
  }

  return (incoming_cv);
}

/**
 * Reads an analog input pin, converts the voltage to millivolts (mV), and 
 * compares it to a specified cutoff value to return a boolean result.
 *
 * This function reads the voltage from the specified analog pin, adjusts for 
 * a voltage divider if resistor values are provided, and checks whether the 
 * voltage exceeds a given cutoff threshold. The result is returned as a boolean, 
 * where `true` indicates that the voltage is above the cutoff and `false` 
 * indicates it is below. Optionally, the function can print the boolean output 
 * for debugging purposes.
 *
 * @param pin_in The analog input pin to read from.
 * @param cutoff_mV The cutoff voltage in millivolts used for comparison. Defaults to 500 mV.
 * @param r1 The resistance value (in ohms) of the resistor connected between 
 *           the voltage source and the analog pin. Defaults to 0 (no voltage divider).
 * @param r2 The resistance value (in ohms) of the resistor connected between 
 *           the analog pin and ground. Defaults to 0 (no voltage divider).
 * @param debug A boolean flag that, when true, prints the boolean result 
 *              to the serial monitor for debugging purposes. Defaults to false.
 * @return `true` if the voltage is greater than the cutoff value, `false` otherwise.
 */
bool read_analog_bool(int pin_in, int cutoff_mV = 500, int r1 = 0, int r2 = 0, bool debug = false) {

  int current_mV = read_analog_mV(pin_in, r1, r2);
  bool out = current_mV > cutoff_mV;

  if (debug) {
    Serial.print("Current value (read_analog_bool): ");
    Serial.print(out);
    Serial.println("");
  }

  return (out);
}

/**
 * Reads an analog input pin, calculates the percentage of the maximum voltage, 
 * and optionally adjusts the value based on tolerance and direction.
 *
 * This function converts an analog input voltage into a percentage of a specified 
 * maximum voltage (`max_mV`). The calculated percentage is constrained between 
 * 0 and 100, and can be adjusted for input variability using a tolerance value. 
 * If the `reverse` parameter is set to `true`, the percentage is inverted 
 * (e.g., 0 becomes 100 and 100 becomes 0). The function also provides an option 
 * for debugging by printing the percentage to the serial monitor.
 *
 * @param pin_in The analog input pin to read from.
 * @param max_mV The maximum voltage in millivolts used to calculate the percentage.
 * @param reverse A boolean flag that, when true, inverts the percentage. Defaults to false.
 * @param r1 The resistance value (in ohms) of the resistor connected between 
 *           the voltage source and the analog pin. Defaults to 0 (no voltage divider).
 * @param r2 The resistance value (in ohms) of the resistor connected between 
 *           the analog pin and ground. Defaults to 0 (no voltage divider).
 * @param debug A boolean flag that, when true, prints the calculated percentage 
 *              to the serial monitor for debugging purposes. Defaults to false.
 * @param tolerance An integer used to adjust the percentage to a multiple of the 
 *                  tolerance value, helping to reduce input variability. Defaults to 4.
 * @return The calculated and adjusted percentage as an integer between 0 and 100.
 */
int read_analog_pct(int pin_in, int max_mV, bool reverse = false, int r1 = 0, int r2 = 0, bool debug = false, int tolerance = 4) {

  float mV = read_analog_mV(pin_in, r1, r2);
  float pct_flt = 100 * mV / max_mV;
  int pct = min(pct_flt, 100);
  pct = pct / tolerance * tolerance;  // tolerate input variability
  if (reverse) pct = 100 - pct;

  if (debug) {
    Serial.print("Current value (read_analog_pct): ");
    Serial.print(pct);
    Serial.println("");
  }

  return (pct);
}
