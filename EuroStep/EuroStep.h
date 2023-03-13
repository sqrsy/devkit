#include <assert.h>
#include "hardware/hardware_switchel.h"
#include "eurotools/eurotools-v2.h"
#include "chips/mcp4822.h"

class EuroStep{

  public:
  
    bool debug = true;
    int input_is_true_threshold = 500;
    bool input_mode_is_analog[hardware::NUMBER_OF_INPUTS];
    int input_values[hardware::NUMBER_OF_INPUTS]; // the input as averaged over 8 readings
    int input_history[hardware::NUMBER_OF_INPUTS][8]; // the last 8 readings
    int pot_values[hardware::NUMBER_OF_POTS];
    bool switch_values[hardware::NUMBER_OF_SWITCHES];
    int output_values_old[hardware::NUMBER_OF_OUTPUTS];
    bool output_mode_is_analog[hardware::NUMBER_OF_OUTPUTS];
    int output_values[hardware::NUMBER_OF_OUTPUTS];
    bool dac_code[16]; // used if output_mode_is_analog
    int dac_event; // helps track which DAC to write

    ///////////////////////////////////////////////////////////////////////////////
    /// Getters and setters
    ///////////////////////////////////////////////////////////////////////////////

    void send_to_output(int index, int value){
      output_values[index] = value;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// These functions are intended to be written by the derived program
    ///////////////////////////////////////////////////////////////////////////////
    
    virtual void on_clock_rise_do(){}
    virtual void on_clock_fall_do(){}
    virtual void on_step_do(){}

    ///////////////////////////////////////////////////////////////////////////////
    /// This is run at start up to initialise pins based on hardware
    ///////////////////////////////////////////////////////////////////////////////

    void start(){
  
      if(debug) Serial.begin(9600);

      // initialise  inputs
      for(int i = 0; i < hardware::NUMBER_OF_INPUTS; i++){
        pinMode(hardware::PINS_INPUT[i], INPUT);
      }
      for(int i = 0; i < hardware::NUMBER_OF_POTS; i++){
        pinMode(hardware::PINS_POT[i], INPUT);
      }
      for(int i = 0; i < hardware::NUMBER_OF_SWITCHES; i++){
        pinMode(hardware::PINS_SWITCH[i], INPUT_PULLUP);
      }

      // initialise outputs
      for(int i = 0; i < hardware::NUMBER_OF_OUTPUTS; i++){
        pinMode(hardware::PINS_OUTPUT[i], OUTPUT);
      }
      for(int i = 0; i < 4; i++){
        if(hardware::PINS_DAC_A[i] != -1){
          pinMode(hardware::PINS_DAC_A[i], OUTPUT);
        }
        if(hardware::PINS_DAC_B[i] != -1){
          pinMode(hardware::PINS_DAC_B[i], OUTPUT);
        }
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// This is a generic way to read audio jack inputs as analog or digital
    ///////////////////////////////////////////////////////////////////////////////
  
    void read_inputs(){
      
      for(int i = 0; i < hardware::NUMBER_OF_INPUTS; i++){
        if(input_mode_is_analog[i]){
          input_values[i] = read_analog_mV_smooth(
          hardware::PINS_INPUT[i],
          input_history[i],
          hardware::V_DIVIDER_R1,
          hardware::V_DIVIDER_R2,
          debug
          );
        }else{
          input_values[i] = read_analog_bool(
          hardware::PINS_INPUT[i],
          input_is_true_threshold,
          hardware::V_DIVIDER_R1,
          hardware::V_DIVIDER_R2,
          debug
          );
        }
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// This is a generic way to read pots as a percent value
    ///////////////////////////////////////////////////////////////////////////////
    
    void read_pots(){
      
      for(int i = 0; i < hardware::NUMBER_OF_POTS; i++){
        pot_values[i] = read_analog_pct(
        hardware::PINS_POT[i],
        hardware::MAX_POT_VOLTAGE,
        hardware::REVERSE_POT,
        0,
        0,
        debug
        );
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// This is a generic way to read switches as bools
    ///////////////////////////////////////////////////////////////////////////////
    
    void read_switches(){
      
      for(int i = 0; i < hardware::NUMBER_OF_SWITCHES; i++){
        switch_values[i] = digitalRead(hardware::PINS_SWITCH[i]);
      }

      if(debug){
        for(int i = 0; i < hardware::NUMBER_OF_SWITCHES; i++){
          Serial.print (switch_values[i]);
        }
        Serial.println ("");
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Provide helper functions for clock rise and clock fall
    ///////////////////////////////////////////////////////////////////////////////

    // in order to run events on clock rise or fall, we need to know
    //  what input to track as the clock signal
    int clock_input = -1; // -1 disables clock events
    void enable_clock_events(int index){
      clock_input = index;
    }

    bool current_state_is_high;
    bool last_state_is_low = true;
    void run_clock_events(){

      if(clock_input > -1){ // by default program assumes no clock exists
        
        current_state_is_high = input_values[clock_input]; // for legibility

        // only run once per clock cycle
        // run on rising edge of incoming clock signal
        if(current_state_is_high & last_state_is_low){
          if(debug) Serial.println ("Clock has risen.");
          on_clock_rise_do();
          last_state_is_low = false;
        }

        // only run once per clock cycle
        // run on falling edge of incoming clock signal
        if(!current_state_is_high & !last_state_is_low){
          if(debug) Serial.println ("Clock has fallen.");
          on_clock_fall_do();
          last_state_is_low = true;
        }
      }
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// This is a generic way to write digital outputs or use MCP4822 DAC
    ///////////////////////////////////////////////////////////////////////////////
    
    void write_outputs(){
      
      dac_event = 0;
      for(int i = 0; i < hardware::NUMBER_OF_OUTPUTS; i++){

        if(output_mode_is_analog[i]){

          // the program cannot currently handle more than 4 analog
          //  outputs since only 2x DACs are set up in memory
          dac_event++;
          assert(dac_event <= 4);
          
          if(output_values[i] != output_values_old[i]){

            if(debug){
              Serial.print ("Writing analog output ");
              Serial.print (i);
              Serial.print (": ");
              Serial.print (output_values[i]);
              Serial.println ("");
            }

            if(dac_event == 1 | dac_event == 3){ // odd dac events are run on channel A
              update_dac_code(dac_code,
              output_values[i], // what to write
              false, // whether dac channel is odd or even
              debug
              );
            }else{ // even dac events are run on channel B
              update_dac_code(dac_code,
              output_values[i], // what to write
              true, // whether dac channel is odd or even
              debug
              );
            }
            
            if(dac_event == 1 | dac_event == 2){ // the first two dac events run on dac A
              write_dac_code(dac_code,
              hardware::PINS_DAC_A[0], // PIN_CS,
              hardware::PINS_DAC_A[1], // PIN_SCK,
              hardware::PINS_DAC_A[2], // PIN_SDI,
              hardware::PINS_DAC_A[3] // PIN_LDAC
              );
            }else{ // the second two dac events run on dac B
              write_dac_code(dac_code,
              hardware::PINS_DAC_B[0], // PIN_CS,
              hardware::PINS_DAC_B[1], // PIN_SCK,
              hardware::PINS_DAC_B[2], // PIN_SDI,
              hardware::PINS_DAC_B[3] // PIN_LDAC
              );
            }
          }
          
        }else{ // if output mode is digital

          if(debug){
            Serial.print ("Writing digital output ");
            Serial.print (i);
            Serial.print (": ");
            Serial.print (output_values[i]);
            Serial.println ("");
          }
          
          if(output_values[i] != output_values_old[i]){
            digitalWrite(hardware::PINS_OUTPUT[i], output_values[i]);
          }
        }

        // whether output mode is analog or digital,
        //  track history to avoid state changes when unnecessary
        output_values_old[i] = output_values[i];
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// This runs through all sub-routines
    ///////////////////////////////////////////////////////////////////////////////
    
    void step(){
      
      read_inputs();
      read_pots();
      read_switches();
      run_clock_events();
      on_step_do();
      write_outputs();
      if(debug) delay(250);
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Provide helper functions for timers
    ///////////////////////////////////////////////////////////////////////////////

    long int last_timer;
    void reset_timer(){
      last_timer = millis();
    }

    long int get_timer(){
      return(millis() - last_timer);
    }
};
