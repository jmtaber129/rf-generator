#ifndef RF_GENERATOR_SI570_CONTROLLER_H_
#define RF_GENERATOR_SI570_CONTROLLER_H_

#include "si570-register-calculator.h"

class Si570Controller {
  public:
    Si570Controller(Si570RegisterCalculator* reg_calculator, double initial_frequency);

    // Updates the SI570 chip with 'new_frequency' via I2C.
    void Update(double new_frequency);

  private:
    Si570RegisterCalculator* reg_calculator_;
};

#endif /* RF_GENERATOR_SI570_CONTROLLER_H_ */
