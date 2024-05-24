#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa6.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/modules/Playback.h"
#include "example_file.h"

// DERIVE NEW CLASS WITH USER-DEFINED FUNCTIONALITY
class make_module : public EuroStep::EuroStep {
public:

  Playback Samp1;
  Playback Samp2;

  // STUFF THAT HAPPENS WHEN CLOCK RISES (IF APPLICABLE)
  void on_clock_rise_do() {
    Samp1.set_audio(WAV1, 197);
    Samp2.set_audio(WAV1, 197);
    Samp1.restart_playback();
    Samp2.restart_playback();
  }

  // STUFF THAT HAPPENS WHEN CLOCK FALLS (IF APPLICABLE)
  void on_clock_fall_do() {
    // nothing
  }

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    // update
    Samp1.set_playback_rate(pot_values[0]);
    Samp2.set_playback_rate(pot_values[1]);
    Samp1.run_playback();
    Samp2.run_playback();

    // send to output
    send_to_output(0, Samp1.get_current_value());
    send_to_output(1, Samp2.get_current_value());
  }
};

make_module module;  // make the class

// RUNS ONCE
void setup() {
  module.input_mode_is_analog[0] = true;   // if input is readAnalog()
  module.enable_clock_events(0);           // treat input 0 as a clock signal (optional)
  module.output_mode_is_analog[0] = true;  // send output 0 to DAC
  module.output_mode_is_analog[1] = true;  // send output 1 to DAC
  module.set_debug(false);                 // toggle debug
  module.start();                          // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
