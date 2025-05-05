#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa6-do.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"

class make_switch : public EuroStep::EuroStep {
public:

  void on_clock_rise_do() {

    int random_value_1 = random(0, 100);  // MAX VALUE 99
    int random_value_2 = random(0, 100);

    if (pot_values[0] > random_value_1) {
      output_value_to_digital(0, true);
    } else {
      if (pot_values[1] > random_value_2) {
        output_value_to_digital(1, true);
      } else {
        // do nothing
      }
    }
  }
  void on_clock_fall_do() {
    output_value_to_digital(0, false);
    output_value_to_digital(1, false);
  }

  void on_step_do() {
    // do nothing
  }
};

make_switch module;

// RUNS ONCE
void setup() {
  module.enable_clock_as_jack(0);  // treat input 0 as a clock signal (optional)
  module.set_debug(false);         // toggle debug
  module.start();                  // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
