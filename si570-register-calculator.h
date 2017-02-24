#ifndef RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_
#define RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_

class Si570RegisterCalculator {
  public:
    // Calculates SI570 register values for 'new_frequency', in MHz.
    void UpdateFrequency(double new_frequency);

    // Returns the frequency that the registers currently represent.
    double get_frequency();

    // Returns the multiplier and divider values.
    double get_rfreq();
    int get_n1();
    int get_hs_div();

    // Returns the 8-bit value for the corresponding SI570 register.
    char get_reg7();
    char get_reg8();
    char get_reg9();
    char get_reg10();
    char get_reg11();
    char get_reg12();

  private:
    // The frequency of the SI570's internal crystal resonator Fxtal, in MHz.
    static const double kInternalResonatorFrequency;

    // Minimum and maximum values for DCO frequency, in MHz (per datasheet).
    static const double kDcoMin;
    static const double kDcoMax;

    // Possible values of HS_DIV (per datasheet).
    static const int kHsDivVals[6];
    static const int kHsDivValsLength;

    // The frequency currently being represented by the registers.
    double current_frequency_;

    // Multiplier and divider values.
    double rfreq_;
    int n1_;
    int hs_div_;
};

#endif /* RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_ */
