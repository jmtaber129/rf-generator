#ifndef RF_GENERATOR_TUNING_CONTROLLER_H_
#define RF_GENERATOR_TUNING_CONTROLLER_H_

// Keeps track of tuning knob and pushbutton input.
class TuningController {
  public:
    // Initializes necessary registers and variables.  Initializes the local
    // frequency value with 'initial_freq', and the local active digit with
    // 'initial_digit'.
    static void Init(double initial_freq, int initial_digit);

    // Updates the frequency and active digit position based on the encoder and
    // pushbutton position.  Should be placed in the port interrupt function.
    static void UpdateTuning();

    // Returns the current updated frequency.
    static double get_curr_freq();

    // Returns the current active digit position.
    static int get_curr_digit();

    // Returns true if 'freq' and 'digit' are equal to the most recent
    // frequency and active digit values.  Should be used to check whether the
    // LCD and Si570 need to be updated.
    static bool CheckUpdate(double freq, int digit);

  private:
    // Pin for the pushbutton.
    static const unsigned char kPushButton;

    // Pins for the rotary encoder's signal A and B.
    static const unsigned char kEncoderA;
    static const unsigned char kEncoderB;

    // Minimum and maximum values for the frequency range.
    static const double kMaxFreq;
    static const double kMinFreq;

    // Lookup table for mapping active digit position to increment/decrement
    // amount.
    static const double kDeltaFrequencyLookup[5];

    // The most recently updated frequency value.
    static double curr_freq_;

    // The most recently updated active digit value.
    static int curr_digit_;

};

#endif  // RF_GENERATOR_TUNING_CONTROLLER_H_
