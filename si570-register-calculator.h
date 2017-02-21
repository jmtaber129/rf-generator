#ifndef RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_
#define RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_

class Si570RegisterCalculator {
  public:
    // Calculates SI570 register values for 'new_frequency'.
    void UpdateFrequency(double new_frequency);

    // Returns the 8-bit value for the corresponding SI570 register.
    char get_reg7();
    char get_reg8();
    char get_reg9();
    char get_reg10();
    char get_reg11();
    char get_reg12();

  private:
    double current_frequency_;
    char reg7_;
    char reg8_;
    char reg9_;
    char reg10_;
    char reg11_;
    char reg12_;
};



#endif /* RF_GENERATOR_SI570_REGISTER_CALCULATOR_H_ */
