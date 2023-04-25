//#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/rasa3x2.h"
//#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/rasa3x2.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/modules/Envelope.h"

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

    // switches set speed of envelope rise / fall
    Env1.fast_A = switch_values[0];
    Env1.fast_DR = switch_values[1];
    Env2.fast_A = switch_values[2];
    Env2.fast_DR = switch_values[3];

    // pots determine ATTACK, SUSTAIN, and DECAY/RELEASE (shared)
    Env1.rate_A = max(1, pot_values[0]);              // attack rate ranges 1-100
    Env1.sustain_limit = max(1, pot_values[1]) * 40;  // sustain level ranges 40-4000
    Env1.rate_DR = max(1, pot_values[2]);             // decay rate ranges 1-100
    Env2.rate_A = max(1, pot_values[3]);
    Env2.sustain_limit = max(1, pot_values[4]) * 40;
    Env2.rate_DR = max(1, pot_values[5]);

    // this method handles both rise and fall, with timers
    //  set up by turn_on_gate() and turn_off_gate()
    Env1.advance_envelope();
    Env2.advance_envelope();

    send_to_output(0, Env1.current_size);
    send_to_output(1, Env2.current_size);
  }
};

make_Envelope_ADSR module;  // make the class

// RUNS ONCE
void setup() {
  module.input_mode_is_analog[0] = false;  // if input is readAnalog()
  module.input_mode_is_analog[1] = false;
  module.enable_clock_events(0);  // treat input 0 as a clock signal (optional)
  module.enable_clock_events_2(1);
  module.output_mode_is_analog[0] = true;  // send output 0 to DAC
  module.output_mode_is_analog[1] = true;  // send output 1 to DAC
  module.debug = false;                    // toggle debug
  module.start();                          // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
