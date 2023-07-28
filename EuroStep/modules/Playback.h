class Playback : public Timer {

private:

  int* audio;
  int audio_size = 0;
  int playback_rate = 100;
  int start_position = 0;
  int current_position = 0;
  bool pause = false;
  bool loop = false;

public:

  ///////////////////////////////////////////////////////////////////////////////
  /// Getters and setters
  ///////////////////////////////////////////////////////////////////////////////

  void set_audio(int* new_audio) {
    audio = new_audio;  // just point to new file, which will already live in RAM
    audio_size = sizeof(new_audio) / sizeof(new_audio[0]);
  }

  void set_playback_rate(int new_rate) {
    playback_rate = new_rate;
  }

  void set_start_position(int new_position) {
    start_position = new_position;
  }

  int get_current_value() {
    return audio[current_position];
  }

  ~Playback() {
    delete[] audio;
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Playback functions
  ///////////////////////////////////////////////////////////////////////////////

  void pause_playback(bool new_value = true) {
    pause = new_value;
  }

  void loop_playback(bool new_value = true) {
    loop = new_value;
  }

  void start_playback() {
    pause = false;
  }

  void reset_playback() {
    pause_playback();
    current_position = start_position;
    reset_timer();
  }

  void restart_playback() {
    reset_playback();
    start_playback();
  }

  void advance_playback() {

    if (get_timer() > playback_rate) {
      current_position += 1;
      reset_timer();
    }

    if (current_position >= audio_size) {  // prevent overflow
      if (loop) {
        restart_playback();
      } else {
        reset_playback();
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Step function called by main program
  ///////////////////////////////////////////////////////////////////////////////

  void step() {

    // update start position if it is changed mid-stream
    if (current_position < start_position) {
      current_position = start_position;
    }

    if (!pause) {
      advance_playback();
    }
  }
};
