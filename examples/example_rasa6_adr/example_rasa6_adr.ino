#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa6-ao.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Envelope.h"

class make_envelope : public EuroStep::EuroStep {
public:

  Envelope Env1;
  Envelope Env2;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start decay
  ///////////////////////////////////////////////////////////////////////////////

  // both envelopes triggered by single clock
  void on_clock_rise_do() {
    Env1.turn_on_gate();
    Env2.turn_on_gate();
  }
  void on_clock_fall_do() {
    Env1.turn_off_gate();
    Env2.turn_off_gate();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for attack, decay, and end of decay phases
  ///////////////////////////////////////////////////////////////////////////////

  void on_step_do() {

    // first Envelope is attack-release (100% sustain)
    Env1.set_ADSR_rate(0, pot_values[0]);  // attack rate ranges 1-100
    Env1.set_ADSR_rate(1, pot_values[1]);  // decay rate ranges 1-100
    Env1.set_ADSR_rate(3, pot_values[1]);  // release = decay
    Env1.set_sustain_level(4000);          // sustain level ranges 40-4000
    Env1.advance_envelope();

    // second Envelope is attack-decay (no sustain)
    Env2.set_ADSR_rate(0, pot_values[0]);  // attack rate ranges 1-100
    Env2.set_ADSR_rate(1, pot_values[1]);  // decay rate ranges 1-100
    Env2.set_ADSR_rate(3, pot_values[1]);  // release = decay
    Env2.set_sustain_level(0);             // sustain level ranges 40-4000
    Env2.advance_envelope();

    // write envelope to dac
    output_value_to_dac(0, Env1.get_current_value());
    output_value_to_dac(1, Env2.get_current_value());
  }
};

make_envelope module;

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
