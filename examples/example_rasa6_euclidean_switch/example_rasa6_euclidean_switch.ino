#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa6-do.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/EuclideanRhythm.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"

#define MAX_LENGTH_FOR_MODULE 8
#define MS_UNTIL_TIMEOUT 1000
#define DEBUG false

class make_switch : public EuroStep::EuroStep {
public:

  Timer timeout_clock;
  EuclideanRhythm euclid;
  int current_step = 0;
  bool current_step_is_beat = false;
  int sequence_length = 8;
  int number_of_beats = 8;

  void on_clock_rise_do() {

    timeout_clock.reset_timer();
    sequence_length = map_percent_to_range(pot_values[0], 1, MAX_LENGTH_FOR_MODULE);
    number_of_beats = map_percent_to_range(pot_values[1], 1, MAX_LENGTH_FOR_MODULE);

    euclid.set_sequence_length(sequence_length);
    euclid.set_number_of_beats(number_of_beats);
    euclid.set_offset(0);

    current_step_is_beat = euclid.is_beat(current_step);
    output_value_to_digital(0, current_step_is_beat);
    output_value_to_digital(1, !current_step_is_beat);

    current_step += 1;
    if (current_step >= sequence_length) {
      current_step = 0;
    }
  }
  void on_clock_fall_do() {
    output_value_to_digital(0, false);
    output_value_to_digital(1, false);
  }

  void on_step_do() {
    if (timeout_clock.get_timer() >= MS_UNTIL_TIMEOUT) {
      current_step = 0;
    }
  }
};

make_switch module;

// RUNS ONCE
void setup() {
  module.enable_clock_as_jack(0);  // treat input 0 as a clock signal (optional)
  module.set_debug(DEBUG);         // toggle debug
  module.start();                  // required to initialise pins
  module.euclid.set_debug(DEBUG);
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
