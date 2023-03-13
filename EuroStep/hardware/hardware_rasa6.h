///////////////////////////////////////////////////////////////////////////////
/// DEFINE HARDWARE PARAMETERS FOR MODULE
///////////////////////////////////////////////////////////////////////////////

namespace hardware{
  
  // INPUT DESIGN
  const int NUMBER_OF_INPUTS = 1;
  int V_DIVIDER_R1 = 220;
  int V_DIVIDER_R2 = 150;
  int PINS_INPUT[NUMBER_OF_INPUTS] = {A2};

  // POT DESIGN
  const int NUMBER_OF_POTS = 2;
  int MAX_POT_VOLTAGE = 5000;
  bool REVERSE_POT = true;
  int PINS_POT[NUMBER_OF_POTS] = {A0, A1};

  // SWITCH DESIGN
  const int NUMBER_OF_SWITCHES = 0;
  int PINS_SWITCH[NUMBER_OF_SWITCHES];

  // OUTPUT DESIGN
  const int NUMBER_OF_OUTPUTS = 2;
  int PINS_OUTPUT[NUMBER_OF_OUTPUTS] = {10, 11};
  int PINS_DAC_A[4] = {11, 10, 9, -1}; // must disable pin with -1 if not using!
  int PINS_DAC_B[4] = {-1, -1, -1, -1};
};
