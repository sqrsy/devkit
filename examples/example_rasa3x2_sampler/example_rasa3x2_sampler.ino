#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa3x2.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Playback.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Predelay.h"
#include "sample_bank.h"

class make_Envelope_Sampler : public EuroStep::EuroStep {
public:

  Playback Sample1;
  int sample1_bank;
  int sample1_playback_speed;
  Predelay Predelay1;
  int sample1_predelay;

  Playback Sample2;
  int sample2_bank;
  int sample2_playback_speed;
  Predelay Predelay2;
  int sample2_predelay;

  void on_start_do() {

    // default values for Sample1
    Sample1.set_playback_step(1);
    Sample1.set_start_position(0);
    Sample1.use_micros();

    // default values for Sample2
    Sample2.set_playback_step(1);
    Sample2.set_start_position(0);
    Sample2.use_micros();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start release
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS WHEN CLOCK RISES AND FALLS (IF APPLICABLE)
  void on_clock_rise_do() {

    // use pot #1 to read sample from "wave table"
    sample1_bank = map_percent_to_range(pot_values[0], 0, 15);
    Sample1.set_audio(sample_array[sample1_bank], sample_sizes[sample1_bank]);
    Sample1.restart_playback();

    // use pot #2 to set pre-delay -- the time it takes before sample playback actually starts
    sample1_predelay = map_percent_to_range(pot_values[2], 1, 1000);  // note: uses millis
    Predelay1.restart_predelay(sample1_predelay);
  }

  void on_clock_fall_do() {
    // do nothing
  }

  void on_clock_2_rise_do() {

    // use pot #1 to read sample from "wave table"
    sample2_bank = map_percent_to_range(pot_values[3], 0, 15);
    Sample2.set_audio(sample_array[sample2_bank], sample_sizes[sample2_bank]);
    Sample2.restart_playback();

    // use pot #2 to set pre-delay -- the time it takes before sample playback actually starts
    sample2_predelay = map_percent_to_range(pot_values[5], 1, 1000);  // note: uses millis
    Predelay2.restart_predelay(sample2_predelay);
  }

  void on_clock_2_fall_do() {
    // do nothing
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for ADSR phases
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    // check what speed to playback at
    sample1_playback_speed = map_percent_to_range(pot_values[1], 100, 10000);  // note: uses micros
    Sample1.set_playback_rate(sample1_playback_speed);
    Predelay1.advance_predelay();  // run sample playback as soon as pre-delay finishes
    if (Predelay1.predelay_is_finished() | Sample1.is_restarting_safely()) {
      Sample1.run_playback();
    }

    // check what speed to playback at
    sample2_playback_speed = map_percent_to_range(pot_values[4], 100, 10000);  // note: uses micros
    Sample2.set_playback_rate(sample2_playback_speed);
    Predelay2.advance_predelay();  // run sample playback as soon as pre-delay finishes
    if (Predelay2.predelay_is_finished() | Sample2.is_restarting_safely()) {
      Sample2.run_playback();
    }

    // send value to DAC
    output_value_to_dac(0, map_byte_to_range(Sample1.get_current_value(), 0, 4095));  //
    output_value_to_dac(1, map_byte_to_range(Sample2.get_current_value(), 0, 4095));
  }
};

make_Envelope_Sampler module;  // make the class

// RUNS ONCE
void setup() {
  module.enable_clock_as_jack(0);  // treat input 0 as a clock signal (optional)
  module.enable_clock_2_as_jack(1);
  module.set_debug(false);  // toggle debug
  module.start();           // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
