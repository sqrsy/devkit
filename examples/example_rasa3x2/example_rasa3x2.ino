#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/rasa3x2.h"
#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
#include "Envelope.h"

class make_Envelope_ADSR : public EuroStep::EuroStep {
public:

  int attack_rate;
  int decay_rate;
  int sustain_level;
  int release_rate;
  bool A_in_progress;
  bool D_in_progress;
  bool S_in_progress;
  bool R_in_progress;
  bool R_end;
  int envelope_size = 0;

  // used for switches
  bool fast_A;
  bool fast_DR;
  bool fast_A_2;
  bool fast_DR_2;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope; on clock fall, start release
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS WHEN CLOCK RISES (IF APPLICABLE)
  void on_clock_rise_do() {
    envelope_size = 0; // reset the envelope
    A_in_progress = true; // re-start attack
    D_in_progress = false;
    S_in_progress = false;
    R_in_progress = false;
    reset_timer();
  }

  // STUFF THAT HAPPENS WHEN CLOCK FALLS (IF APPLICABLE)
  void on_clock_fall_do() {
    A_in_progress = false;
    D_in_progress = false;
    S_in_progress = false;
    R_in_progress = true; // start release
    reset_timer();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for ADSR phases
  ///////////////////////////////////////////////////////////////////////////////

  // STUFF THAT HAPPENS EVERY STEP
  void on_step_do() {

    fast_A = switch_values[0];
    fast_DR = switch_values[1];
    fast_A_2 = switch_values[2];
    fast_DR_2 = switch_values[3];

    attack_rate = pot_values[0]; // attack rate ranges 0-100
    if(A_in_progress){
      if(debug) Serial.println ("Attack stage.");
      if(attack_rate == 0){
        envelope_size = 4000; // if attack is 0, spike up right away
      }else{
        if(get_timer() > attack_rate){
          if(fast_A){
            envelope_size += .05 * 4000;
          }else{
            envelope_size += .01 * 4000;
          }
          reset_timer();
        }
      }
      if(envelope_size >= 4000){
        envelope_size = 4000; // cap envelope size at 4 volts
        A_in_progress = false; // stop attack
        D_in_progress = true;
      }
    }

    sustain_level = pot_values[1] * 4000; // sustain level ranges 0-4000
    decay_rate = pot_values[2]; // decay rate ranges 0-100
    if(D_in_progress){
      if(debug) Serial.println ("Decay stage.");
      if(decay_rate == 0){
        envelope_size = sustain_level; // if no decay, drop to sustain
      }else{
        if(get_timer() > decay_rate){
          if(fast_DR){
            envelope_size -= .05 * 4000;
          }else{
            envelope_size -= .01 * 4000;
          }
          reset_timer();
        }
      }
      if(envelope_size <= sustain_level){
        envelope_size = sustain_level;
        D_in_progress = false;
        S_in_progress = true;
      }
    }

    release_rate = decay_rate;
    if(R_in_progress){
      if(debug) Serial.println ("Release stage.");
      if(release_rate = 0){
        envelope_size = 0; // if no release, drop to 0 right away
      }else{
        if(get_timer() > release_rate){
          if(fast_DR){
            envelope_size -= .05 * 4000;
          }else{
            envelope_size -= .01 * 4000;
          }
          reset_timer();
        }
      }
      if(envelope_size <= 0){
        envelope_size = 0;
        R_in_progress = false;
        R_end = true;
      }
    }
  }
};

make_Envelope_ADSR module;  // make the class

// RUNS ONCE
void setup() {
//   module.input_mode_is_analog[0] = true;   // if input is readAnalog()
//   module.enable_clock_events(0);           // treat input 0 as a clock signal (optional)
//   module.output_mode_is_analog[0] = true;  // send output 0 to DAC
//   module.output_mode_is_analog[1] = true;  // send output 1 to DAC
//   module.debug = true;                     // toggle debug
//   module.start();                          // required to initialise pins
}

// RUNS EVERY STEP
void loop() {
  module.step();  // runs all user-defined *_do() calls
}
