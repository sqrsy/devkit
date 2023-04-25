class Envelope {

public:

  // gate used to manage sustain
  int gate_on = false;
  bool pause_DR = false;

  // size, min, max, and sustain in mV
  int current_size;  // the current size of envelope to send to analog out
  int current_limit;
  int max_limit = 4000;
  int sustain_limit;
  int min_limit = 0;

  // how many milliseconds before changing voltage
  int rate_A;
  int rate_DR;  // assume decay / release have same rate

  bool fast_A = false;          // whether to use fast attack
  bool fast_DR = false;         // whether to use fast decay / release
  bool in_progress_A = false;   // if attack is in progress
  bool in_progress_DR = false;  // if decay / release is in progress

  void turn_on_gate() {
    gate_on = true;
    in_progress_A = true;
    in_progress_DR = false;
    current_size = min_limit;  // reset envelope at bottom
    pause_DR = false;
    reset_timer();
  }

  void turn_off_gate() {
    gate_on = false;
    pause_DR = false;  // now that gate is off, unpause envelope to start release
    reset_timer();
  }

  long int last_timer = 0;
  void reset_timer() {
    last_timer = millis();
  }

  long int get_timer() {
    return (millis() - last_timer);
  }

  void rise() {

    if (rate_A == 0) {
      current_size = max_limit;
    } else {
      if (get_timer() > rate_A) {
        if (fast_A) {
          current_size += .05 * max_limit;
        } else {
          current_size += .01 * max_limit;
        }
        reset_timer();
      }
    }
    if (current_size >= max_limit) {
      current_size = max_limit;
      in_progress_A = false;
      in_progress_DR = true;
    }
  }

  void fall() {

    if (gate_on) {
      current_limit = sustain_limit;
    } else {
      current_limit = min_limit;
    }

    if (!pause_DR) {
      if (rate_DR == 0) {
        current_size = current_limit;
      } else {
        if (get_timer() > rate_DR) {
          if (fast_DR) {
            current_size -= .05 * max_limit;
          } else {
            current_size -= .01 * max_limit;
          }
          reset_timer();
        }
      }
    }

    if (current_size <= current_limit) {
      current_size = current_limit;
      if (gate_on) pause_DR = true;  // if limit reached with gate on, sustain envelope
    }

    if (current_size == min_limit) {
      in_progress_DR = false;  // only stop once min limit is reached
    }
  }

  void advance_envelope() {
    if (in_progress_A) {
      rise();
    } else if (in_progress_DR) {
      fall();
    }
  }
};
