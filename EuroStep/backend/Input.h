class Input : public Timer {

private:

  bool debug = false;

  // what input to read
  int input_pin = 0;

  // how often to read input
  int read_frequency = 0;
  int read_frequency_offset = 0;

  // store & convert read value
  int last_value_mV = 0;  // used to add "clock rise" and "clock fall" logic
  int current_value_mV_history[8] = { 0 };
  int current_value_mV = 0;
  int current_value_percent = 0;
  int round_percent_to = 4;
  int input_as_bool_threshold = 500;
  int max_input_mV = 5000;

  // used to back-calculate mV from voltage divider network
  int r1_value;
  int r2_value;

  // used to smooth input using moving average (i.e., for "jacks")
  bool smooth_input = false;

  // used to swap the order in which percent gets mapped (i.e., for "pots")
  bool reverse_input = false;

  // used to read digital pins instead of analog pins (i.e., for "switches")
  bool input_is_digital = false;
  bool input_from_digital_read = 0;

  int round_percent_to_nearest(int value) {
    return current_value_percent / value * value;
  }

public:

  void read_input_immediately() {
    last_value_mV = current_value_mV;  // save last value -- used for clock signals
    if (input_is_digital) {
      current_value_mV = max_input_mV * digitalRead(input_pin);  // convert digital to mV to unify getters()
    } else {
      if (smooth_input) {
        current_value_mV = read_analog_mV_smooth(input_pin, current_value_mV_history, r1_value, r2_value, debug);
      } else {
        current_value_mV = read_analog_mV(input_pin, r1_value, r2_value, debug);
      }
    }
    if (reverse_input) current_value_mV = max_input_mV - current_value_mV;
  }

  void read_input_if_ready() {

    // read once Timer has elapsed
    if (get_timer() > read_frequency) {
      read_input_immediately();
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Configure how to read the Input
  ///////////////////////////////////////////////////////////////////////////////

  void set_read_frequency(int value) {
    read_frequency = value;
  }

  void set_read_frequency_offset(int value) {
    read_frequency_offset = value;
  }

  void set_max_input_mV(int value) {
    max_input_mV = value;
  }

  void set_reverse_input(bool value) {
    reverse_input = value;
  }

  void set_debug(bool value) {
    debug = value;
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Wrappers for Input setup
  ///////////////////////////////////////////////////////////////////////////////

  void setup_as_jack(int pin, int r1, int r2) {
    input_pin = pin;
    r1_value = r1;
    r2_value = r2;
    set_read_frequency(2);
    set_read_frequency_offset(input_pin);
    smooth_input = true;
  }

  void setup_as_pot(int pin) {
    input_pin = pin;
    r1_value = 0;
    r2_value = 0;
    set_read_frequency(10);
    set_read_frequency_offset(input_pin);
    smooth_input = false;
  }

  void setup_as_switch(int pin) {
    input_pin = pin;
    set_read_frequency(10);
    set_read_frequency_offset(input_pin);
    smooth_input = false;
    input_is_digital = true;
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Actually get the Input
  ///////////////////////////////////////////////////////////////////////////////

  int get_input_as_mV() {
    read_input_if_ready();
    return current_value_mV;
  }

  int get_input_as_percent() {
    read_input_if_ready();
    current_value_percent = map_mV_to_percent(current_value_mV, max_input_mV);
    return round_percent_to_nearest(round_percent_to);
  }

  bool get_input_as_bool() {
    read_input_if_ready();
    return current_value_mV >= input_as_bool_threshold;
  }

  bool check_if_input_went_low_to_high() {
    // read_input_if_ready(); -- don't need since read_jacks() is already run
    return (last_value_mV < input_as_bool_threshold) & (current_value_mV >= input_as_bool_threshold);
  }

  bool check_if_input_went_high_to_low() {
    // read_input_if_ready(); -- don't need since read_jacks() is already run
    return (last_value_mV >= input_as_bool_threshold) & (current_value_mV < input_as_bool_threshold);
  }
};
