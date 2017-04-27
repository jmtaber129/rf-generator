#include "tuning-controller.h"

#include <msp430g2553.h>

void TuningController::Init(double initial_freq, int initial_digit) {
  P2IE |= kPushButton + kEncoderA;
  P2IES &= kPushButton;
  P2IFG &= 0x00;

  curr_freq_ = initial_freq;
  curr_digit_ = initial_digit;

  __bis_SR_register(GIE);
}

double TuningController::curr_freq_ = 0;
int TuningController::curr_digit_= 0;

const unsigned char TuningController::kPushButton = BIT3;

const unsigned char TuningController::kEncoderA = BIT5;
const unsigned char TuningController::kEncoderB = BIT4;

const double TuningController::kMaxFreq = 20.0;
const double TuningController::kMinFreq = 10.0;

const double TuningController::kDeltaFrequencyLookup[5] = {
    10.0,
    1.0,
    0.1,
    0.01,
    0.001
};

void TuningController::UpdateTuning() {
  // Update active digit.
  if (P2IFG & kPushButton) {
    if (curr_digit_ < 4) {
      ++curr_digit_;
    } else {
      curr_digit_ = 0;
    }
  }

  // Update frequency.
  if (P2IFG & kEncoderA) {
    // Determine how much we should increment/decrement by based off of the
    // current active digit.
    double delta_frequency;

    if (curr_digit_ > 4 || curr_digit_ < 0) {
      curr_digit_ = 4;
    }
    delta_frequency = kDeltaFrequencyLookup[curr_digit_];

    // CCW if encoder B is high, CW if encoder B is low.
    double new_freq;
    if(P2IN & kEncoderB){
      new_freq = curr_freq_ - delta_frequency;
    } else{
      new_freq = curr_freq_ + delta_frequency;
    }

    // If the new frequency would go out of bounds, just keep the old
    // frequency (i.e., don't change it).
    if (new_freq >= kMinFreq && new_freq <= kMaxFreq) {
      curr_freq_ = new_freq;
    }
  }

  P2IFG &= 0x00;
}

double TuningController::get_curr_freq() {
  return curr_freq_;
}

int TuningController::get_curr_digit() {
  return curr_digit_;
}

bool TuningController::CheckUpdate(double freq, int digit) {
  return (freq == curr_freq_ && digit == curr_digit_);
}
