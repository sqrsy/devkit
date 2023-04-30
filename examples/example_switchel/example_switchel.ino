//#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/switchel-v3.h"
//#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
//#include "/home/thom/Dropbox/Hobbies/Synths/Arduino/devkit/add_ons/quantize_voltage.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/hardware/switchel-v3.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/EuroStep.h"
#include "C:/Users/61436/Dropbox/Hobbies/Synths/Arduino/devkit/EuroStep/add_ons/quantize_voltage.h"

class make_Quantizer : public EuroStep::EuroStep {
public:

  const int glide_step = 8;
  int glideA;
  int glideB;
  int cv_writeA = 0;
  int cv_writeB = 0;
  int cv_targetA = 0;
  int cv_targetB = 0;
  int input_currentA = 0;
  int input_currentB = 0;
  int input_oldA = 0;
  int input_oldB = 0;

  // set up timer that works in microseconds:
  //  this is moved outside of EuroStep module so that
  //  it can be used by any program
  Timer timer;
  Timer timer_2;
  void on_start_do() {  // runs once as part of start()
    timer.use_micros();
    timer_2.use_micros();
  }

  void on_step_do() {

    // the pots set the glide rate
    glideA = pot_values[0];
    glideB = pot_values[1];

    input_currentA = input_values[0];
    if (abs(input_currentA - input_oldA) > 20) {  // check new value against old value to stop wobble
      // quantize operation handled by quantize_votage() function
      cv_targetA = quantize_voltage(input_currentA, switch_values, debug);
      input_oldA = input_currentA;
      timer.reset_timer();
    }

    input_currentB = input_values[1];
    if (abs(input_currentB - input_oldB) > 20) {  // check new value against old value to stop wobble
      cv_targetB = quantize_voltage(input_currentB, switch_values, debug);
      input_oldB = input_currentB;
      timer_2.reset_timer();
    }

    // glide up or down with pot setting speed
    // currently, min speed is ~20 ms
    if (cv_writeA != cv_targetA) {
      if (glideA == 0) {
        cv_writeA = cv_targetA;
      } else {
        if (timer.get_timer() > pow(glideA, 2)) {  // how many micros it takes to get X% way to next note
          if (cv_targetA > cv_writeA) {
            cv_writeA += glide_step;
          } else {
            cv_writeA -= glide_step;
          }
          if (cv_writeA / 10 == cv_targetA / 10) cv_writeA = cv_targetA;
          timer.reset_timer();
        }
      }
    }

    // glide up or down with pot setting speed
    // currently, min speed is ~20 ms
    if (cv_writeB != cv_targetB) {
      if (glideB == 0) {
        cv_writeB = cv_targetB;
      } else {
        if (timer_2.get_timer() > pow(glideB, 2)) {  // how many micros it takes to get X% way to next note
          if (cv_targetB > cv_writeB) {
            cv_writeB += glide_step;
          } else {
            cv_writeB -= glide_step;
          }
          if (cv_writeB / 10 == cv_targetB / 10) cv_writeB = cv_targetB;
          timer_2.reset_timer();
        }
      }
    }

    send_to_output(1, cv_writeA);  // mismatch between input and output jacks
    send_to_output(0, cv_writeB);
  }
};

make_Quantizer module;
void setup() {
  module.input_mode_is_analog[0] = true;
  module.input_mode_is_analog[1] = true;
  module.output_mode_is_analog[0] = true;  // send output 0 to DAC
  module.output_mode_is_analog[1] = true;  // send output 1 to DAC
  module.debug = false;                    // toggle debug
  module.start();                          // required to initialise pins
}

void loop() {
  module.step();  // runs all user-defined *_do() calls
}
