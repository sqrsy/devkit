#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa6-do.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"

class make_switch : public EuroStep::EuroStep {
public:

  Timer timeout_clock;
  int timeout_limit;
  int current_step;
  int max_step;

  void on_clock_rise_do() {
    max_step = map_percent_to_range(pot_values[0], 0, 15);
    timeout_clock.reset_timer();

    if (current_step >= max_step) {
      output_value_to_digital(1, true);
      current_step = 0;
    } else {
      output_value_to_digital(0, true);
      current_step += 1;
    }
  }
  void on_clock_fall_do() {
    output_value_to_digital(0, false);
    output_value_to_digital(1, false);
  }

  void on_step_do() {

    timeout_limit = map_percent_to_range(pot_values[0], 0, 3000);
    if (timeout_clock.get_timer() >= timeout_limit) {
      current_step = 0;
    }
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
