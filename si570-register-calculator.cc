#include "si570-register-calculator.h"
#include "math.h"

const double Si570RegisterCalculator::kInternalResonatorFrequency = 114.285;

const double Si570RegisterCalculator::kDcoMin = 4850.0;
const double Si570RegisterCalculator::kDcoMax = 5670.0;
const int Si570RegisterCalculator::kHsDivVals[] = { 11, 9, 7, 6, 5, 4 };
const int Si570RegisterCalculator::kHsDivValsLength = 6;

void Si570RegisterCalculator::UpdateFrequency(double new_frequency) {
  this->current_frequency_ = new_frequency;

  // Determine min and max divider values based on DCO frequency min and max.
  int divider_total_min = ceil(kDcoMin / new_frequency);
  int divider_total_max = floor(kDcoMax / new_frequency);

  // Start with the highest HS_DIV value for best power efficiency.
  for (int i = 0; i < kHsDivValsLength; ++i) {
    this->hs_div_ = kHsDivVals[i];

    // N1 must be even or 1, so find the ceiling of divider/hs_div/2, then
    // double it.  Power efficiency is at a max when N1 is at a min, which
    // occurs when DCO frequency is at a min when HS_DIV is constant.
    this->n1_ = ceil(divider_total_min * 1.0 / this->hs_div_ / 2) * 2;

    // Check that the divider values don't exceed the maximum (If divider max
    // is exceeded, DCO frequency max would also be exceeded).
    if (this->hs_div_ * this->n1_ <= divider_total_max) {
      break;
    }
  }

  // Get the desired DCO frequency from desired frequency and dividers, get
  // RFREQ from DCO / Fxtal, then get the integer representation by multiplying
  // by 2^28 and trunctating the decimal.
  this->rfreq_ = (new_frequency * this->hs_div_ * this->n1_) * 0x10000000
      / kInternalResonatorFrequency;
}

double Si570RegisterCalculator::get_frequency() {
  return this->current_frequency_;
}

long long Si570RegisterCalculator::get_rfreq() {
  return this->rfreq_;
}

int Si570RegisterCalculator::get_n1() {
  return this->n1_;
}

int Si570RegisterCalculator::get_hs_div() {
  return this->hs_div_;
}

char Si570RegisterCalculator::get_reg7() {
  // {HS_DIV[2:0], N1[6:2]}.
  return ((this->hs_div_ - 4) << 5) + ((this->n1_ - 1) >> 2);
}

char Si570RegisterCalculator::get_reg8() {
  // {N1[1:0], RFREQ[37:32]}.
  return ((this->n1_ - 1) << 6) + (this->rfreq_ >> 32);
}

char Si570RegisterCalculator::get_reg9() {
  // {RFREQ[31:24]}.
  return this->rfreq_ >> 24;
}

char Si570RegisterCalculator::get_reg10() {
  // {RFREQ[23:16]}.
  return this->rfreq_ >> 16;
}

char Si570RegisterCalculator::get_reg11() {
  // {RFREQ[15:8]}.
  return this->rfreq_ >> 8;
}

char Si570RegisterCalculator::get_reg12() {
  // {RFREQ[7:0]}.
  return this->rfreq_;
}

