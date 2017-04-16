#include "lcd-controller.h"

#include <msp430g2553.h>
#include <cstdio>

LcdController::LcdController(double initial_frequency) {
  P1DIR |= 0xFF;
  P1OUT &= 0x00;
  this->SendChar(kFunctionSet1, true);
  this->SendChar(kFunctionSet2, true);
  this->SendChar(kFunctionSet3, true);
  this->SendChar(kDisplayOn, true);
  this->ClearDisplay();
  this->SendChar(kSetIncrement, true);

  this->Update(initial_frequency, /*digit=*/0);
}

const unsigned char LcdController::kFunctionSet1 = 0x33u;
const unsigned char LcdController::kFunctionSet2 = 0x32u;
const unsigned char LcdController::kFunctionSet3 = 0x28u;
const unsigned char LcdController::kDisplayOn = 0xCu;
const unsigned char LcdController::kDisplayClear = 0x01u;
const unsigned char LcdController::kSetIncrement = 0x06u;

const unsigned int LcdController::kRsBit = BIT4;
const unsigned int LcdController::kRwBit = BIT5;
const unsigned int LcdController::kEnableBit = BIT6;

void LcdController::Update(double new_frequency, int new_digit) {
  this->curr_frequency_ = new_frequency;
  this->curr_digit_ = new_digit;

  char freq_string[16];
  int whole_num = curr_frequency_;
  int decimal = curr_frequency_ * 1000 - whole_num * 1000;
  std::sprintf(freq_string, "%d.%d MHz", whole_num, decimal);
  ClearDisplay();
  Write(freq_string);
  // TODO: Update the digit.
  int arrow_column;
  if (curr_digit_ < 3) {
    arrow_column = curr_digit_;
  } else {
    arrow_column = curr_digit_ + 1;
  }
  SetPosition(/*row=*/1, arrow_column);
  Write("^");

}

void LcdController::UpdateFreq(double new_frequency) {
  // TODO: Update the LCD.
}

void LcdController::UpdateDigit(int new_digit) {
  // TODO: Update the LCD.
}

void LcdController::ClearDisplay() {
  SendChar(kDisplayClear, true);
  Delay(1);
}

void LcdController::SetPosition(int row, int column) {
  if (row < 0 || row > 1 || column < 0 || column > 15) {
    return;  // Invalid position, just return.
  }
  unsigned char ddram_address = column + (row << 6);
  unsigned char instruction = ddram_address + BIT7;
  SendChar(instruction, true);
}

void LcdController::Write(char* str) {
  while (*str) {
    SendChar(*str, false);
    ++str;
  }

  __no_operation();
}

void LcdController::SendChar(unsigned char c, bool is_instruction) {
  WaitUntilNotBusy();
  SetRw(false);
  SetRs(!is_instruction);
  P1OUT = (P1OUT & 0xF0) | ((c >> 4) & 0x0F);  // Send higher nibble.
  DoWriteEdge();
  P1OUT = (P1OUT & 0xF0) | (c & 0x0F);  // Send lower nibble.
  DoWriteEdge();
}

void LcdController::DoWriteEdge() {
  SetEnable(true);
  Delay(1);
  SetEnable(false);
}

void LcdController::DoReadEdge() {
  SetEnable(false);
  Delay(1);
  SetEnable(true);
}

void LcdController::WaitUntilNotBusy() {
  P1DIR &= ~(BIT3);  // Set P1.3 as an input.
  SetRs(false);
  SetRw(true);
  int delays = 0;
  do {
    DoReadEdge();
    ++delays;
  } while ((P1IN & BIT3));
  /*
  while ((P1IN & BIT3) == 1) {
    DoReadEdge();
  }
  */
  P1DIR |= BIT3;  // Set P1.3 as an output.
}

void LcdController::Delay(int delay_time) {
  for (int i = 0; i <= delay_time; i++) {
    for (int j = 0; j < 100; j++) {
      __no_operation();
    }
  }
}

void LcdController::SetRs(bool rs) {
  if (rs) {
    P1OUT |= kRsBit;
  } else {
    P1OUT &= ~(kRsBit);
  }
}

void LcdController::SetRw(bool rw) {
  if (rw) {
    P1OUT |= kRwBit;
  } else {
    P1OUT &= ~(kRwBit);
  }
}

void LcdController::SetEnable(bool enable) {
  if (enable) {
    P1OUT |= kEnableBit;
  } else {
    P1OUT &= ~(kEnableBit);
  }
}


