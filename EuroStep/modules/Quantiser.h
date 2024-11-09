/*
Class Name: Quantiser

Purpose: Quantise CV.

Dependencies: None.

Use: Create an instance of the class and configure settings, then run:
-- enable_note(j): Add j-th note to quantiser scale.
-- disable_note(j): Remove j-th note from quantiser scale.
-- run(int mV): Quantise the voltage.
*/

// Convert voltage to MIDI note number
int convert_mV_to_note_number(int mV, int rounding_bias) {

  float note_to_round = (mV + rounding_bias) / 83.3333f;
  int note_number = round(note_to_round);
  return note_number;
}

// Convert MIDI note number to voltage
int convert_note_number_to_mV(int note_number) {

  int mV = note_number * 83.3333f;
  return mV;
}

// Quantise note number to next nearest note number
int find_nearest_note_in_scale(int note_number, bool scale[12]) {

  int next_note_higher = note_number;
  int next_note_lower = note_number;
  int note_index = note_number % 12;
  if (scale[note_index]) {
    return note_number;
  } else {
    while (true) {

      // try the next note above in scale
      next_note_higher += 1;
      note_index = next_note_higher % 12;
      if (scale[note_index]) {
        return next_note_higher;
      }

      // try the next note below in scale
      next_note_lower -= 1;
      note_index = next_note_lower % 12;
      if (scale[note_index]) {
        return next_note_lower;
      }
    }
  }
}

class Quantizer {

private:

  bool scale[12] = { false };
  int incoming_cv = 0;
  int incoming_tone = 0;
  int outgoing_tone = 0;
  int outgoing_cv = 0;

  // used to prevent rapid note switching due to rounding of unstable input CV
  float rounding_bias = 15;  // in mV

  // used to skip quantiser when no notes are set
  int number_of_active_notes = 0;
  bool nothing_to_quantise = false;

public:

  ///////////////////////////////////////////////////////////////////////////////
  /// Use to update the quantised scale
  ///////////////////////////////////////////////////////////////////////////////

  // Enable a note in the scale
  void enable_note(int note) {
    if (note >= 0 && note < 12) {
      scale[note] = true;
    }
  }

  // Disable a note in the scale
  void disable_note(int note) {
    if (note >= 0 && note < 12) {
      scale[note] = false;
    }

    // do not quantize if there are no notes selected
    //  (this avoids an infinite while loop)
    number_of_active_notes = 0;
    for (int i = 0; i < 12; i++) {
      number_of_active_notes += scale[i];
      if (number_of_active_notes == 0) {
        nothing_to_quantise = true;
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Main steps for quantiser
  ///////////////////////////////////////////////////////////////////////////////

  // Set the incoming CV in millivolts
  void set_incoming_cv(int cv) {
    incoming_cv = cv;
  }

  // Perform the quantization of the input CV
  int do_quantise_cv() {

    incoming_tone = convert_mV_to_note_number(incoming_cv, rounding_bias);
    outgoing_tone = find_nearest_note_in_scale(incoming_tone, scale);
    outgoing_cv = convert_note_number_to_mV(outgoing_tone);
  }

  // Get the quantized CV in millivolts
  int get_quantised_cv() {
    return outgoing_cv;
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Single method to run all quantiser steps
  ///////////////////////////////////////////////////////////////////////////////

  int run(int cv) {
    set_incoming_cv(cv);
    do_quantise_cv();
    get_quantised_cv();
  }
};
