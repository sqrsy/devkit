#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa3x2.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Envelope.h"

class make_Envelope_ADSR : public EuroStep::EuroStep {
public:

  Envelope Env1;
  Envelope Env2;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start release
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS WHEN CLOCK RISES AND FALLS (IF APPLICABLE)
  void on_clock_rise_do() {
    Env1.turn_on_gate();
  }
  void on_clock_fall_do() {
    Env1.turn_off_gate();
  }
  void on_clock_rise_2_do() {
    Env2.turn_on_gate();
  }
  void on_clock_fall_2_do() {
    Env2.turn_off_gate();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for ADSR phases
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    // Set speed up for Envelopes based on switch state
    // -- note: by computing (bool*4 + 1), step pre-factor is either 1 or 5
    // -- note: decay and release share same values
    Env1.set_ADSR_step(0, switch_values[0] * 4 + 1);
    Env1.set_ADSR_step(1, switch_values[1] * 4 + 1);
    Env1.set_ADSR_step(3, switch_values[1] * 4 + 1);
    Env2.set_ADSR_step(0, switch_values[2] * 4 + 1);
    Env2.set_ADSR_step(1, switch_values[3] * 4 + 1);
    Env2.set_ADSR_step(3, switch_values[3] * 4 + 1);

    // pots determine ATTACK, SUSTAIN, and DECAY/RELEASE (shared)
    Env1.set_ADSR_rate(0, pot_values[0]);                // attack rate ranges 1-100
    Env1.set_ADSR_rate(1, pot_values[1]);                // decay rate ranges 1-100
    Env1.set_ADSR_rate(3, pot_values[1]);                // release = decay
    Env1.set_sustain_level(max(1, pot_values[2]) * 40);  // sustain level ranges 40-4000
    Env2.set_ADSR_rate(0, pot_values[3]);                // attack rate ranges 1-100
    Env2.set_ADSR_rate(1, pot_values[4]);                // decay rate ranges 1-100
    Env2.set_ADSR_rate(3, pot_values[4]);                // release = decay
    Env2.set_sustain_level(max(1, pot_values[5]) * 40);  // sustain level ranges 40-4000

    // this method handles both rise and fall, with timers
    //  set up by turn_on_gate() and turn_off_gate()
    Env1.advance_envelope();
    Env2.advance_envelope();

    send_to_output(0, Env1.get_current_value());
    send_to_output(1, Env2.get_current_value());
  }
};

make_Envelope_ADSR module;  // make the class

// RUNS ONCE
void setup() {
  module.set_input_to_analog(0, false);
  module.set_input_to_analog(1, false);
  module.enable_clock_events(0);         // treat input 0 as a clock signal (optional)
  module.enable_clock_events_2(1);       // treat input 1 as second clock signal (optional)
  module.set_output_to_analog(0, true);  // send output 0 to DAC
  module.set_output_to_analog(1, true);  // send output 1 to DAC
  module.set_debug(false);               // toggle debug
  module.start();                        // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
