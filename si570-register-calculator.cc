#include "si570-register-calculator.h"

const double Si570RegisterCalculator::kInternalResonatorFrequency = 114.285;

void Si570RegisterCalculator::UpdateFrequency(double new_frequency) {
  // TODO(jmtaber129): Calculate register values.
}

char Si570RegisterCalculator::get_reg7() {
  return this->reg7_;
}

char Si570RegisterCalculator::get_reg8() {
  return this->reg8_;
}

char Si570RegisterCalculator::get_reg9() {
  return this->reg9_;
}

char Si570RegisterCalculator::get_reg10() {
  return this->reg10_;
}

char Si570RegisterCalculator::get_reg11() {
  return this->reg11_;
}

char Si570RegisterCalculator::get_reg12() {
  return this->reg12_;
}


