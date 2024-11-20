#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa3x2.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Envelope.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Interpolate.h"
#include "C:/Users/61436/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Playback.h"
//#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/hardware/rasa3x2.h"
//#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/EuroStep.h"
//#include "C:/Users/Thom/Dropbox/Hobbies/Electronics/GitHub/devkit/EuroStep/add-ons/Playback.h"
#include "wavetable.h"

class make_Envelope_Sampler : public EuroStep::EuroStep {
public:

  // objects for doing the work
  Playback LFO;
  Envelope Env;
  Interpolate Xfade;

  // constants
  int WAVETABLE_SIZE = 32;

  // used for inputs
  int env_rise;
  int env_fall;
  int env_hold;
  int lfo_xfade;
  int lfo_speed;
  int lfo_fm;

  // used for intermediate calculations
  int lfo_playback_rate;
  double env_val_as_percent;
  double lfo_fm_penalty;
  double lfo_cv_as_percent;
  int env_lfo_val;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start release
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS WHEN CLOCK RISES AND FALLS (IF APPLICABLE)
  void on_clock_rise_do() {
    Env.turn_on_gate();
    LFO.restart_playback();
  }
  void on_clock_fall_do() {
    Env.turn_off_gate();
  }

  void on_start_do() {

    // default values for LFO
    LFO.set_playback_step(1);
    LFO.set_start_position(0);
    LFO.loop_playback(true);
    LFO.use_micros();

    // set up Interpolate engine to cross-fade between waves
    Xfade.set_inputs(tri, noise, WAVETABLE_SIZE);
    Xfade.set_output(lfo);
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for ADSR phases
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    env_rise = pot_values[0];
    env_fall = pot_values[1];
    env_hold = max(1, pot_values[2]) * 40;  // sustain max is 4000 mV
    lfo_xfade = pot_values[3];
    lfo_speed = pot_values[4];
    lfo_speed = map_percent_to_range(lfo_speed, 1, 32) * 1000;
    lfo_fm = pot_values[5];

    // Move Envelope along
    Env.set_ADSR_rate(0, env_rise);  // attack rate ranges 1-100
    Env.set_ADSR_rate(1, env_fall);  // decay rate ranges 1-100
    Env.set_ADSR_rate(3, env_fall);  // release = decay
    Env.set_sustain_level(env_hold);
    Env.advance_envelope();
    env_val_as_percent = Env.get_current_value_as_percent();

    // Set LFO shape -- cross-fade between sine vs. tri waves
    Xfade.interpolate(lfo_xfade);
    LFO.set_audio(lfo, WAVETABLE_SIZE);

    // optional: FM the LFO based on Envelope size
    // either way, set the LFO speed
    if (lfo_fm > 4) {
      lfo_fm_penalty = lfo_fm / 100.0 * env_val_as_percent;
      lfo_fm_penalty = 3.0 * lfo_fm_penalty / 4.0;                 // no more than 75% increase in lfo speed
      lfo_playback_rate = lfo_speed - lfo_speed * lfo_fm_penalty;  // subtract will increase speed!!
    } else {
      lfo_playback_rate = lfo_speed;
    }
    lfo_cv_as_percent = input_values[1] / 5000.0;       // normal speed when 0V -- CV will increase speed
    lfo_cv_as_percent = 3.0 * lfo_cv_as_percent / 4.0;  // no more than 75% increase in lfo speed
    lfo_playback_rate = lfo_playback_rate - lfo_playback_rate * lfo_cv_as_percent;
    LFO.set_playback_rate(lfo_playback_rate);

    // Move LFO along -- the output is a function of Envelope and LFO
    LFO.run_playback();
    env_lfo_val = 127 + ((LFO.get_current_value() - 127) * env_val_as_percent);  // mid-point is 127

    // send value to DAC
    send_to_output(0, Env.get_current_value());  //
    send_to_output(1, map_byte_to_range(env_lfo_val, 0, 4095));
  }
};

make_Envelope_Sampler module;  // make the class

// RUNS ONCE
void setup() {
  module.set_input_to_analog(0, false);
  module.set_input_to_analog(1, true);
  module.enable_clock_events(0);         // treat input 0 as a clock signal (optional)
  module.set_output_to_analog(0, true);  // send output 0 to DAC
  module.set_output_to_analog(1, true);  // send output 1 to DAC
  module.set_debug(false);               // toggle debug
  module.start();                        // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
