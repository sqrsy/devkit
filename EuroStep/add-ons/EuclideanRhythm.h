/*
Class Name: EuclideanRhythm

Purpose: Generate a Euclidean rhythm pattern.

Dependencies: None.

Use: Create an instance of the class and configure settings, then run:
-- set_sequence_length(int s): Set the total number of steps (maximum 32).
-- set_number_of_beats(int b): Set how many beats (pulses) are distributed in the sequence.
-- set_offset(int o): Apply an offset to rotate the rhythm pattern.
-- is_beat(int index): Returns true if the given index has a beat.
-- is_rest(int index): Returns true if the given index has a rest.
-- set_debug(value): sets whether to delay after each step and print to console
-- -- note: false by default

Notes:
-- The internal sequence is recalculated on every call to `is_beat()` or `is_rest()`.
-- Sequence generation distributes rests after each beat and handles remaining rests by interleaving them.
-- Maximum supported sequence length is defined by MAX_EUCLIDEAN_SEQUENCE (default 32).
-- Input values are clamped to MAX_EUCLIDEAN_SEQUENCE.
*/

#define MAX_EUCLIDEAN_SEQUENCE 32

class EuclideanRhythm {

private:

  int sequence_length = 8;  // number of events in a cycle
  int number_of_beats = 3;  // number of beats in a cycle
  int o = 0;                // offset for pattern
  bool debug = false;
  bool current_rule[MAX_EUCLIDEAN_SEQUENCE] = { false };  // 0 is silence, 1 is sound

public:

  ///////////////////////////////////////////////////////////////////////////////
  /// Configure
  ///////////////////////////////////////////////////////////////////////////////

  void set_sequence_length(int value) {
    if (value >= MAX_EUCLIDEAN_SEQUENCE) {
      sequence_length = MAX_EUCLIDEAN_SEQUENCE;
    } else {
      sequence_length = value;
    }
  }

  void set_number_of_beats(int value) {
    if (value >= MAX_EUCLIDEAN_SEQUENCE) {
      number_of_beats = MAX_EUCLIDEAN_SEQUENCE;
    } else {
      number_of_beats = value;
    }
  }

  void set_offset(int value) {
    if (value >= MAX_EUCLIDEAN_SEQUENCE) {
      o = MAX_EUCLIDEAN_SEQUENCE;
    } else {
      o = value;
    }
  }

  void set_debug(bool value) {
    debug = value;
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Main program
  ///////////////////////////////////////////////////////////////////////////////

  void update_euclidean_rhythm() {

    int N_int = number_of_beats;                           // number of sounds
    int N_0 = sequence_length - number_of_beats;           // number of silences
    int N_0_per_int = N_0 / number_of_beats;               // number of rests to allocate after EVERY beat
    int count_extra_rests = N_0 % N_int;                   // number of extra rests to allocate irregularly
    int count_no_extra_rests = N_int - count_extra_rests;  // number of sounds WITHOUT extra rest

    int counter = 0 + o;
    for (int i = 0; i < N_int; i = i + 1) {

      // accomodate offset, by going back to the beginning
      if (counter > sequence_length) {
        counter = 1;
      }

      // Record the beat
      current_rule[counter] = 1;  // add beat
      counter = counter + 1;

      // If there are rests after EVERY beat,
      //  add those rests now.
      if (N_0_per_int > 0) {
        for (int j = 0; j < N_0_per_int; j = j + 1) {
          current_rule[counter] = 0;  // add rest
          counter = counter + 1;
        }
      }

      // If there are any extra beats that need to get added
      //  check whether to add them now.
      if (count_extra_rests > 0) {
        // if beat is even and nothing should be done (i.e., no beat or rest)
        //  do nothing, but decrease the do-nothing counter
        if ((i % 2) == 0 & count_no_extra_rests > 0) {
          count_no_extra_rests = count_no_extra_rests - 1;  // this is a do-nothing counter
        } else {
          // otherwise, add the extra rest then decrease the do-something counter
          current_rule[counter] = 0;  // add rest
          counter = counter + 1;
          count_extra_rests = count_extra_rests - 1;  // this is a do-something counter
        }
      }

      if (debug) {
        Serial.print("Euclid sequence length: ");
        Serial.println(sequence_length);
        Serial.print("Euclid number of beats: ");
        Serial.println(number_of_beats);
        Serial.print("Euclid offset: ");
        Serial.println(o);
        Serial.print("Euclid rule: ");
        for (int z = 0; z < MAX_EUCLIDEAN_SEQUENCE; z = z + 1) {
          Serial.print(current_rule[z]);
        }
        Serial.println("");
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Get result
  ///////////////////////////////////////////////////////////////////////////////

  bool is_beat(int index) {
    update_euclidean_rhythm();
    return current_rule[index];
  }

  bool is_rest(int index) {
    update_euclidean_rhythm();
    return !is_beat(index);
  }
};
