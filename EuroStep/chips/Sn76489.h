class Sn76489 {

private:

  int PIN_WE;
  int PIN_CE;
  int PIN_D0;
  int PIN_D1;
  int PIN_D2;
  int PIN_D3;
  int PIN_D4;
  int PIN_D5;
  int PIN_D6;
  int PIN_D7;

  // second byte used only for tone
  bool dac_code[16] = { false };
  bool use_byte_2 = true;

  int master_clock_frequency;
  int previous_volume = 0;
  int previous_tone = 0;

  void enable_byte_2(bool value) {
    use_byte_2 = value;
    dac_code[8] = 0;
    dac_code[9] = 0;
  }

  void use_channel_1() {
    dac_code[0] = 1;
    dac_code[1] = 0;
    dac_code[2] = 0;
  };

  void use_channel_2() {
    dac_code[0] = 1;
    dac_code[1] = 0;
    dac_code[2] = 1;
  };

  void use_channel_3() {
    dac_code[0] = 1;
    dac_code[1] = 1;
    dac_code[2] = 0;
  };

  void use_channel_noise() {
    dac_code[0] = 1;
    dac_code[1] = 1;
    dac_code[2] = 1;
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Backend: Write to Chip
  ///////////////////////////////////////////////////////////////////////////////

  void write_to_chip() {
    // TO DO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (use_byte_2) {
      // TO DO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Backend: Write Volume
  ///////////////////////////////////////////////////////////////////////////////

  void map_volume_to_dac_code() {
    enable_byte_2(false);
    // TO DO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  };

  void write_volume(int new_volume) {
    new_volume = transfer_value_to_range(new_volume, 0, 31);
    new_volume = 31 - new_volume;  // invert volume amount to attenuation amount
    if (new_volume != previous_volume) {
      map_volume_to_dac_code();
      write_to_chip();
      previous_volume = new_volume;
    }
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Backend: Write Tone
  ///////////////////////////////////////////////////////////////////////////////

  void map_tone_to_dac_code() {
    enable_byte_2(true);
    // TO DO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  };

  void write_tone(int new_tone) {
    if (new_tone != previous_tone) {
      map_tone_to_dac_code();
      write_to_chip();
      previous_tone = new_tone;
    }
  };


public:

  void set_pins(int we, int ce, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) {
    PIN_WE = we;
    PIN_CE = ce;
    PIN_D0 = d0;
    PIN_D1 = d1;
    PIN_D2 = d2;
    PIN_D3 = d3;
    PIN_D4 = d4;
    PIN_D5 = d5;
    PIN_D6 = d6;
    PIN_D7 = d7;
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Setters
  ///////////////////////////////////////////////////////////////////////////////

  void set_master_clock(int value) {
    master_clock_frequency = value;
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Volume Front-end
  ///////////////////////////////////////////////////////////////////////////////

  void send_volume_to_channel_1(int value) {
    use_channel_1();
    write_volume(value);
  };

  void send_volume_to_channel_2(int value) {
    use_channel_2();
    write_volume(value);
  };

  void send_volume_to_channel_3(int value) {
    use_channel_3();
    write_volume(value);
  };

  void send_volume_to_channel_noise(int value) {
    use_channel_noise();
    write_volume(value);
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Tone Front-end
  ///////////////////////////////////////////////////////////////////////////////

  void send_tone_to_channel_1(int value) {
    use_channel_1();
    write_volume(value);
  };

  void send_tone_to_channel_2(int value) {
    use_channel_2();
    write_volume(value);
  };

  void send_tone_to_channel_3(int value) {
    use_channel_3();
    write_volume(value);
  };

  void send_tone_to_channel_noise(int value) {
    use_channel_noise();
    write_volume(value);
  };

  ///////////////////////////////////////////////////////////////////////////////
  /// Other Wrappers
  ///////////////////////////////////////////////////////////////////////////////

  void turn_off_channel_1() {
    send_volume_to_channel_1(0);
  }

  void turn_off_channel_2() {
    send_volume_to_channel_2(0);
  }

  void turn_off_channel_3() {
    send_volume_to_channel_3(0);
  }

  void turn_off_channel_noise() {
    send_volume_to_channel_noise(0);
  }
};
