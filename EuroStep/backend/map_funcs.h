/**
 * Maps a percentage value to an integer within a specified range [min, max].
 *
 * @param pct An integer representing a percentage (expected range: 0 to 100).
 * @param min The minimum integer value of the desired range.
 * @param max The maximum integer value of the desired range.
 * @return An integer between min and max, based on the percentage input.
 */
int map_percent_to_range(int pct, int min, int max) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  int result = min + (pct * (max - min) / 100);
  return result;
}

/**
 * Maps a percentage value to a float within a centered range [-plus_or_minus, plus_or_minus].
 *
 * This function transforms a percentage value (0 to 100) into a float that is 
 * centered around zero and falls within the range [-plus_or_minus, plus_or_minus]. 
 * A percentage of 50 corresponds to 0, while 0 and 100 map to -plus_or_minus 
 * and plus_or_minus, respectively.
 *
 * @param pct An integer representing the percentage (expected range: 0 to 100).
 * @param plus_or_minus The maximum absolute value of the mapped range. The range 
 *                      is symmetrical around zero and extends from 
 *                      -plus_or_minus to plus_or_minus.
 * @return A float value within the range [-plus_or_minus, plus_or_minus], mapped 
 *         from the given percentage.
 */
float map_percent_to_centred_range(int pct, int plus_or_minus) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  float new_val = (pct - 50) / (50.0 / plus_or_minus);
  if (new_val < 0) {
    new_val = max(new_val, -1 * plus_or_minus);
  } else {
    new_val = min(new_val, plus_or_minus);
  }
  return (new_val);
}

/**
 * Maps a percentage value to a float within an exponential range centered around a given exponent.
 *
 * This function transforms a percentage (0 to 100) into a float value within an 
 * exponential range. It uses a centered exponent offset to calculate the result. 
 * The percentage value is first mapped to a centered range using 
 * `map_percent_to_centred_range`, then adjusted by an exponential offset 
 * centered at `exp_mid`, and finally converted into a float using a power 
 * of 2 calculation.
 *
 * @param pct An integer representing the percentage (expected range: 0 to 100).
 * @param exp_mid The central exponent value around which the exponential range is centered.
 * @param plus_or_minus The maximum absolute offset for the exponent from `exp_mid`. 
 *                      The exponent will vary within the range 
 *                      [exp_mid - plus_or_minus, exp_mid + plus_or_minus].
 * @return A float value representing the mapped exponential value.
 */
float map_percent_to_centred_exp_range(int pct, int exp_mid, int plus_or_minus) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  float exp_offset = map_percent_to_centred_range(pct, plus_or_minus);
  float exp = exp_mid + exp_offset;
  float new_val = power_float(2, exp);
  return (new_val);
}

/**
 * @brief Maps a millivolt (mV) value to a percentage based on a maximum millivolt value.
 * 
 * This function calculates the percentage representation of a given millivolt (mV) value 
 * relative to a maximum millivolt value (default is 5000mV). The result is returned as an 
 * integer percentage.
 * 
 * @param mV The millivolt value to be converted to a percentage.
 * @param max_mV The maximum millivolt value to which the mV is compared. Default is 5000 mV.
 * @return The percentage of mV relative to max_mV (0 to 100).
 */
long map_mV_to_percent(int mV, int max_mV = 5000) {
  int pct = (100 * mV) / (max_mV);
  return (pct);
}

/**
 * @brief Maps a millivolt (mV) value to a frequency (Hz) based on a reference frequency at zero volts.
 * 
 * This function calculates the frequency in Hertz (Hz) corresponding to a given millivolt value (mV),
 * using a reference frequency (`Hz_at_zero_volts`) that represents the frequency when the voltage is 0V.
 * The relationship assumes the frequency follows a logarithmic scale relative to the voltage.
 * 
 * @param mV The millivolt value that affects the frequency.
 * @param Hz_at_zero_volts The reference frequency in Hertz at 0V, used to determine the frequency at other voltages.
 * @return The resulting frequency in Hertz, calculated based on the provided mV value and reference frequency.
 */
float map_mV_to_Hz(int mV, int Hz_at_zero_volts) {
  int reference_voltage = log(Hz_at_zero_volts) / log(2) * 1000;
  float Hz = power_float(2, (reference_voltage + mV) / 1000.0);
  return (Hz);
}

/**
 * Maps a millivolt (mV) value to a musical note number.
 *
 * This function converts an input voltage in millivolts (mV) to a corresponding 
 * musical note number. The conversion is based on the assumption that a change 
 * of 83.3333 mV corresponds to a change of one note. The resulting note number 
 * is rounded to the nearest integer.
 *
 * @param mV The input voltage in millivolts.
 * @return An integer representing the rounded musical note number.
 */
int map_mV_to_note_number(int mV) {

  float note_to_round = mV / 83.3333f;
  int note_number = round(note_to_round);
  return note_number;
}

/**
 * Maps a musical note number to a corresponding millivolt (mV) value.
 *
 * This function converts a musical note number to a voltage value in millivolts (mV).
 * The conversion assumes that each note number corresponds to an increment of 
 * 83.3333 mV. The result is calculated using a simple multiplication.
 *
 * @param note_number The musical note number to be converted.
 * @return An integer representing the equivalent voltage in millivolts.
 */
int map_note_number_to_mV(int note_number) {

  int mV = note_number * 83.3333f;
  return mV;
}
