/*
Class Name: Playback

Purpose: To manage the playback of a WAV file saved as a C++ array object.

Dependencies: This class inherits from the Timer class.

Use: Create an instance of the class and configure settings, then run:
-- set_audio(audio_array, audio_array_length): associate Playback with another C++ array
-- run_playback(): advance the timer to update the current Playback position
-- -- note: run_playback() will not play if Playback is paused
-- get_current_value(): return the value associated with the current Playback position

You will eventually want to start the Playback:
-- restart_playback(): manually restart the Playback from the start position

You may also want other Playback options:
-- rewind_playback(): rewind Playback but do not restart
-- pause_playback(): pause the Playback
-- start_playback(): unpause the Playback
-- loop_playback(): begin looping the Playback
-- unloop_playback(): stop looping the Playback

You may wish to configure the following settings:
-- set_playback_rate(value): set the rate for each step change for Playback
-- set_playback_step(value): set the size of each step change for Playback
-- set_start_position(value): change the start position for Playback

The following settings are relevant from the Timer class:
-- use_millis(): use milliseconds as the time units for the set_ADSR_rate (default)
-- use_micros(): use microseconds as the time units for the set_ADSR_rate
*/

class Playback : public Timer {

private:

  // how fast to playback array (in ms until advancing current position)
  int playback_rate = 100;

  // how much to speed up playback (optional)
  int playback_step = 1;

  // load a reference "file" to sample
  //signed short audio[256] = { 0 };  // 44100 would be 1 second of 44100 Hz sampled audio
  short* audio;
  int audio_length = 0;

  // the position along the playback reference file
  int current_position = 0;
  int start_position = 0;

  // other args
  bool pause = false;
  bool loop = false;

public:

  ///////////////////////////////////////////////////////////////////////////////
  /// Getters and setters
  ///////////////////////////////////////////////////////////////////////////////

  void set_playback_rate(int value) {
    playback_rate = value;
  }

  void set_playback_step(int value) {
    playback_step = value;
  }

  // void set_audio(int *audio_array, int audio_array_length) {
  //   audio_length = audio_array_length;
  //   for (int i = 0; i < audio_length; i++) {
  //     audio[i] = audio_array[i];
  //   }
  // }

  void set_audio(int* audio_array, int audio_array_length) {
    audio = audio_array;  // just point to new file, which will already live in RAM
    audio_length = audio_array_length;
  }

  void set_start_position(int value) {
    start_position = value;
  }

  int get_current_value() {
    if (current_position == start_position) {
      return 0;  // return 0 after "rewind" until Playback restarts
    } else {
      return audio[current_position];
    }
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

  void unpause_playback() {
    pause = false;
  }

  void loop_playback(bool new_value = true) {
    loop = new_value;
  }

  void unloop_playback() {
    loop = false;
  }

  void rewind_playback() {
    pause_playback();
    current_position = start_position;
    reset_timer();
  }

  void restart_playback() {
    rewind_playback();
    unpause_playback();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// Run the playback program
  ///////////////////////////////////////////////////////////////////////////////

  void run_playback() {

    // update start position if it is changed mid-stream
    if (current_position < start_position) {
      current_position = start_position;
    }

    if (!pause) {

      if (get_timer() > playback_rate) {
        current_position += 1;
        reset_timer();
      }

      if (current_position >= audio_length) {  // prevent overflow
        if (loop) {
          restart_playback();
        } else {
          rewind_playback();
        }
      }
    }
  }
};
