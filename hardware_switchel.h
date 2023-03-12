///////////////////////////////////////////////////////////////////////////////
/// DEFINE HARDWARE PARAMETERS FOR MODULE
///////////////////////////////////////////////////////////////////////////////

namespace hardware{
  
  // INPUT DESIGN
  const int NUMBER_OF_INPUTS = 2;
  int V_DIVIDER_R1 = 220;
  int V_DIVIDER_R2 = 150;
  int PINS_INPUT[NUMBER_OF_INPUTS] = {A5, A4};

  // POT DESIGN
  const int NUMBER_OF_POTS = 2;
  int MAX_POT_VOLTAGE = 4900;
  bool REVERSE_POT = true;
  int PINS_POT[NUMBER_OF_POTS] = {A6, A7};

  // SWITCH DESIGN
  const int NUMBER_OF_SWITCHES = 12;
  int PINS_SWITCH[13, A0, A1, A3, A2, 12, 11, 10, 9, 8, 7, 6];

  // OUTPUT DESIGN
  const int NUMBER_OF_OUTPUTS = 2;
  int PINS_OUTPUT[NUMBER_OF_OUTPUTS] = {4, 3};
  int PINS_DAC_A[4] = {2, 3, 4, 5};
  int PINS_DAC_B[4] = {-1, -1, -1, -1};
};
