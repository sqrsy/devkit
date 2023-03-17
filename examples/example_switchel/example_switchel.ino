#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/switchel-v2.h"
#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/add_ons/quantize_voltage.h"

class make_Quantizer : public EuroStep::EuroStep {
public:

  int tuneA;
  int tuneB;
  int cv_outA;
  int cv_outB;

  void on_step_do() {
    
    // the pots fine tune incoming CV by 1x-1.25x factor
    tuneA = pot_values[0] / 400.0 + 1;
    tuneB = pot_values[1] / 400.0 + 1;

    // quantize operation handled by round_votage() function
    cv_outA = quantize_voltage(input_values[0], switch_values, debug);
    cv_outA = cv_outA * tuneA;
    cv_outB = quantize_voltage(input_values[1], switch_values, debug);
    cv_outB = cv_outB * tuneB;

    send_to_output(0, cv_outA);
    send_to_output(1, cv_outB);
  }
};

make_Quantizer module;
void setup() {
  module.input_mode_is_analog[0] = true;
  module.input_mode_is_analog[1] = true;
  module.output_mode_is_analog[0] = true;  // send output 0 to DAC
  module.output_mode_is_analog[1] = true;  // send output 1 to DAC
  module.debug = true;                     // toggle debug
  module.start();                          // required to initialise pins
}

void loop() {
  module.step();  // runs all user-defined *_do() calls
}
