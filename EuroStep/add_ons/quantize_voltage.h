int quantize_voltage(int mV, bool SCALE[12], bool debug = false) {

  // do not quantize if there are no notes selected
  //  (this avoids an infinite while loop)
  int number_of_switches = 0;
  for (int i = 0; i < 12; i++) {
    number_of_switches += SCALE[i];
  }
  if (number_of_switches == 0) {
    return (mV);
  }

  int octave_shift_f = 0;
  int octave_shift_c = 0;
  int octave_shift = 0;  // whether quantized tone is an octave above / below
  int outTone = 99;      // dummy value

  // convert incoming voltage to 'remainder' that needs rounding
  //	(the result is a decimal ranges from 0 to 12)
  int residVolts = mV % 1000;
  float residTones = residVolts / 83.3333;

  // get the whole tones below and above deimal (e.g., 4.6 => {4, 5})
  int floortone = floor(residTones);
  int ceiltone = floortone + 1;

  // find the nearest whole tone that belongs to the scale
  while (outTone == 99) {

    // if only lower note is in scale, select lower note
    if (SCALE[floortone] == 1 & SCALE[ceiltone] == 0) {

      outTone = floortone;
      octave_shift = octave_shift_f;
      break;

      // if only higher note is in scale, select higher note
    } else if (SCALE[floortone] == 0 & SCALE[ceiltone] == 1) {

      outTone = ceiltone;
      octave_shift = octave_shift_c;
      break;

      // if both notes are in scale, select the one that is closest
    } else if (SCALE[floortone] == 1 & SCALE[ceiltone] == 1) {

      if (round(residTones) == floor(residTones)) {
        outTone = floortone;
        octave_shift = octave_shift_f;
        break;
      } else {
        outTone = ceiltone;
        octave_shift = octave_shift_c;
        break;
      }

      // if neither note is in scale, look beyond to +/- 1 semi-tone
    } else {

      floortone -= 1;
      if (floortone == -1) {  // if out-of-bounds on low end
        floortone = 11;       // so move down to 11 which is 12th note, i.e., is B
        octave_shift_f -= 1;  // track you have moved down an octave
      }

      ceiltone += 1;
      if (ceiltone == 12) {   // if out-of-bounds on high end
        ceiltone = 0;         // so move up to 0 which is 1st note, i.e., is C
        octave_shift_c += 1;  // track you have moved up an octive
      }
    }
  }

  // convert rounded tone into outgoing voltage
  int base_volts = mV / 1000;
  int new_mV = base_volts * 1000 + octave_shift * 1000 + outTone * 83.3333;

  if (debug) {
    Serial.print("Original (mV): ");
    Serial.println(mV);
    Serial.print("Residue (mV): ");
    Serial.println(residVolts);
    Serial.print("Current tone: ");
    Serial.println(residTones);
    Serial.print("Trying tone at low end: ");
    Serial.println(floortone);
    Serial.print("Trying tone at high end: ");
    Serial.println(ceiltone);
    Serial.print("Choosing tone: ");
    Serial.println(outTone);
    Serial.print("New voltage: ");
    Serial.println(new_mV);
  }

  return new_mV;
}
