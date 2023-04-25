int get_bit(char byte, int pos) {

  int result = byte >> pos & 1;
  return (result);
}

void write_ym2612(char byte, bool debug) {

  // pass data to chip
  digitalWrite(PIN_CS, 0);
  digitalWrite(PIN_DA0, get_bit(byte, 0));
  digitalWrite(PIN_DA1, get_bit(byte, 1));
  digitalWrite(PIN_DA2, get_bit(byte, 2));
  digitalWrite(PIN_DA3, get_bit(byte, 3));
  digitalWrite(PIN_DA4, get_bit(byte, 4));
  digitalWrite(PIN_DA5, get_bit(byte, 5));
  digitalWrite(PIN_DA6, get_bit(byte, 6));
  digitalWrite(PIN_DA7, get_bit(byte, 7));
  delayMicroseconds(1);

  // enable write mode
  digitalWrite(PIN_WR, 0);
  digitalWrite(PIN_RD, 1);
  delayMicroseconds(5);

  // stop write
  digitalWrite(PIN_WR, 1);
  digitalWrite(PIN_RD, 0);
  delayMicroseconds(5);

  // finish
  digitalWrite(PIN_CS, 1);
}

void set_reg_to_val(char reg_byte, char val_byte, bool debug) {

  // write to register
  digitalWrite(PIN_A0, 0);
  write_ym2612(reg_byte, debug);

  // write value
  digitalWrite(PIN_A1, 1);
  write_ym2612(val_byte, debug);
}
