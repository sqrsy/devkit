#include "EuroStep.h"

// DERIVE NEW CLASS WITH USER-DEFINED FUNCTIONALITY
class make_module: public EuroStep::EuroStep{
	public:
	
	// STUFF YOU NEED FOR PROGRAM TO WORK
	int foo = 1;
	int bar = 0;
	
	// STUFF THAT HAPPENS WHEN CLOCK RISES (IF APPLICABLE)
	void on_clock_rise_do(){
		foo = foo + 1;
	}
	
	// STUFF THAT HAPPENS WHEN CLOCK FALLS (IF APPLICABLE)
	void on_clock_fall_do(){
		foo = foo - 1;
	}
	
	// STUFF THAT HAPPENS EVERY STEP
	void on_step_do(){
		bar = foo * 1000;
		send_to_output(0, bar); // send bar to output 0
		send_to_output(1, 4000 - bar); // send 4000 - bar to output 1
	}
};

make_module module; // make the class

// RUNS ONCE
void setup(){
	module.input_mode_is_analog[0] = true; // if input is readAnalog()
	module.enable_clock_events(0); // treat input 0 as a clock signal (optional)
	module.output_mode_is_analog[0] = true; // send output 0 to DAC
	module.output_mode_is_analog[1] = true; // send output 1 to DAC
	module.debug = true; // toggle debug
	module.start(); // required to initialise pins
}

// RUNS EVERY STEP
void loop(){
	module.step(); // runs all user-defined *_do() calls
}
