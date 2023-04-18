class Timer {

  public:

    bool time_in_micros = false;
    unsigned long last_timer = 0;

    void use_micros () {
      time_in_micros = true;
    }

    void use_millis () {
      time_in_micros = false;
    }

    unsigned long time_right_now() {
      if (time_in_micros ) {
        return micros();
      } else {
        return millis();
      }
    }

    unsigned long get_timer() {
      if (time_right_now() < last_timer) reset_timer(); // overflow catch
      return (time_right_now() - last_timer);
    }

    void reset_timer() {
      last_timer = time_right_now();
    }
};
