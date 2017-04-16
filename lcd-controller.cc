#include "lcd-controller.h"

#include <msp430g2553.h>

LcdController::LcdController(double initial_frequency) {
  P1DIR |= 0xFF;
  P1OUT &= 0x00;
  this->SendChar(kFunctionSet1, true);
  this->SendChar(kFunctionSet2, true);
  this->SendChar(kFunctionSet3, true);
  this->SendChar(kDisplayOn, true);
  this->SendChar(kDisplayClear, true);
  this->SendChar(kSetIncrement, true);

  this->Update(initial_frequency, /*digit=*/0);
}

unsigned char LcdController::kFunctionSet1 = 0x33u;
unsigned char LcdController::kFunctionSet2 = 0x32u;
unsigned char LcdController::kFunctionSet3 = 0x28u;
unsigned char LcdController::kDisplayOn = 0xCu;
unsigned char LcdController::kDisplayClear = 0x01u;
unsigned char LcdController::kSetIncrement = 0x06u;

void LcdController::UpdateFreq(double new_frequency) {
  // TODO: Update the LCD.
}

void LcdController::UpdateDigit(int new_digit) {
  // TODO: Update the LCD.
}

void LcdController::ClearDisplay() {
  // TODO: Clear the display.
}

void LcdController::SetPosition(int row, int column) {
  // TODO: Set the position.
}

void LcdController::Write(char* str) {
  // TODO: Write 'str' to the display.
}

void LcdController::SendChar(unsigned char c, bool is_instruction) {
  // TODO: Send the char.
}

void LcdController::DoHighToLow() {
  // TODO: Perform the write.
}

void LcdController::DoLowToHigh() {
  // TODO: Perform the read.
}

void LcdController::WaitUntilNotBusy() {
  // TODO: Wait until LCD not busy.
}

void LcdController::Delay(int delay_time) {
  for (int i = 0; i <= delay_time; ++i) {
    for (int j = 0; j < 100; ++j);
  }
}

void LcdController::SetRs(bool rs) {
  // TODO: Set the rs bit.
}

void LcdController::SetRw(bool rw) {
  // TODO: Set the r/w bit.
}

void LcdController::SetEnable(bool enable) {
  // TODO: Set the enable bit.
}


