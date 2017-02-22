#include "si570-controller.h"

Si570Controller::Si570Controller(Si570RegisterCalculator* reg_calculator,
    double initial_frequency) :
    reg_calculator_(reg_calculator) {
  this->Update(initial_frequency);
}

void Si570Controller::Update(double new_frequency) {
  reg_calculator_->UpdateFrequency(new_frequency);
  // Transmit 'reg_calculator_' registers to chip.
  // TODO(jmtaber129): Transmit registers.
}
