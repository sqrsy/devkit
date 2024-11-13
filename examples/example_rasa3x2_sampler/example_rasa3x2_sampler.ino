#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa3x2.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Playback.h"
#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Predelay.h"
#include "example_sample_bank.h"

class make_Envelope_Sampler : public EuroStep::EuroStep {
public:

  Playback Sample1;
  Predelay Predelay1;
  Playback Sample2;
  Predelay Predelay2;

  int sample1_bank;
  int sample1_playback_speed;
  int sample1_predelay;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start release
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS WHEN CLOCK RISES AND FALLS (IF APPLICABLE)
  void on_clock_rise_do() {

    sample1_bank = map_percent_to_range(pot_values[0], 0, 11);
    Sample1.set_audio(sample_array[sample1_bank], sample_sizes[sample1_bank]);
    Sample1.restart_playback();

    sample1_predelay = pot_values[2];
    Predelay1.restart_predelay(sample1_predelay);
    //Sample1.set_playback_step(1);
    //Sample1.set_start_position(0);
  }
  void on_clock_fall_do() {
  }
  void on_clock_rise_2_do() {
  }
  void on_clock_fall_2_do() {
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for ADSR phases
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    Predelay1.advance_predelay();
    if (Predelay1.predelay_is_finished()) {
    }
    sample1_playback_speed = pot_values[1];
    Sample1.set_playback_rate(sample1_playback_speed);

    send_to_output(0, Sample1.get_current_value());
    send_to_output(1, 1);
  }
};

make_Envelope_Sampler module;  // make the class

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
