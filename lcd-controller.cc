#include "lcd-controller.h"

#include <msp430g2553.h>
#include <cstdio>

LcdController::LcdController(double initial_frequency, int initial_digit) {
  P1DIR |= 0xFF;
  P2DIR |= kRsBit + kRwBit + kEnableBit;

  this->SendChar(kFunctionSet1, true);
  this->SendChar(kFunctionSet2, true);
  this->SendChar(kFunctionSet3, true);
  this->SendChar(kDisplayOn, true);
  this->ClearDisplay();
  this->SendChar(kSetIncrement, true);

  this->Update(initial_frequency, initial_digit);
}

const unsigned char LcdController::kFunctionSet1 = 0x33u;
const unsigned char LcdController::kFunctionSet2 = 0x32u;
const unsigned char LcdController::kFunctionSet3 = 0x28u;
const unsigned char LcdController::kDisplayOn = 0xCu;
const unsigned char LcdController::kDisplayClear = 0x01u;
const unsigned char LcdController::kSetIncrement = 0x06u;

const unsigned int LcdController::kRsBit = BIT2;
const unsigned int LcdController::kRwBit = BIT1;
const unsigned int LcdController::kEnableBit = BIT0;

const unsigned char LcdController::kBusyFlag = BIT0;

const unsigned char LcdController::kNibbleLookup[16] = {
    0x00,                       // 0x0.
    BIT5,                       // 0x1.
    BIT4,                       // 0x2.
    BIT5 + BIT4,                // 0x3.
    BIT3,                       // 0x4.
    BIT3 + BIT5,                // 0x5.
    BIT3 + BIT4,                // 0x6.
    BIT3 + BIT4 + BIT5,         // 0x7.
    BIT0,                       // 0x8.
    BIT0 + BIT5,                // 0x9.
    BIT0 + BIT4,                // 0xA.
    BIT0 + BIT5 + BIT4,         // 0xB.
    BIT0 + BIT3,                // 0xC.
    BIT0 + BIT3 + BIT5,         // 0xD.
    BIT0 + BIT3 + BIT4,         // 0xE.
    BIT0 + BIT3 + BIT4 + BIT5   // 0xF.
};

void LcdController::Update(double new_frequency, int new_digit) {
  char freq_string[16];

  // Print the frequency to a string, with the form "XX.XXX MHz".
  int whole_num = new_frequency;
  int decimal = new_frequency * 1000 - whole_num * 1000;
  std::sprintf(freq_string, "%.2d.%.3d MHz", whole_num, decimal);

  // Write the frequency string to the display.
  ClearDisplay();
  Write(freq_string);

  // Determine the correct location of the active-digit arrow, and write the
  // arrow to the display at that position.
  int arrow_column;
  if (new_digit < 2) {
    arrow_column = new_digit;
  } else {
    arrow_column = new_digit + 1;
  }
  SetPosition(/*row=*/1, arrow_column);
  Write("^");
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
}

void LcdController::SendChar(unsigned char c, bool is_instruction) {
  WaitUntilNotBusy();
  SetRw(false);
  SetRs(!is_instruction);

  // Get the higher and lower nibbles in the char.
  unsigned int high_index = (c >> 4) & 0x0F;
  unsigned int low_index = (c & 0x0F);

  // Map the char nibbles to the correct pins.
  unsigned char high_nibble = kNibbleLookup[high_index];
  unsigned char low_nibble = kNibbleLookup[low_index];

  // Send the high nibble.
  P1OUT &= 0xC6;
  P1OUT |= high_nibble;
  DoWriteEdge();

  // Send the low nibble.
  P1OUT &= 0xC6;
  P1OUT |= low_nibble;
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
  P1DIR &= ~(kBusyFlag);  // Set the busy bit as an input.
  SetRs(false);
  SetRw(true);

  // Read and test the busy flag until the flag is not set.
  do {
    DoReadEdge();
  } while ((P1IN & kBusyFlag));

  P1DIR |= kBusyFlag;  // Set the busy bit as an output.
}

void LcdController::Delay(int delay_time) {
  for (int i = 0; i <= delay_time; i++) {
    for (int j = 0; j < 1600; j++) {
      __no_operation();  // NOP to make sure these loops get executed.
    }
  }
}

void LcdController::SetRs(bool rs) {
  if (rs) {
    P2OUT |= kRsBit;
  } else {
    P2OUT &= ~(kRsBit);
  }
}

void LcdController::SetRw(bool rw) {
  if (rw) {
    P2OUT |= kRwBit;
  } else {
    P2OUT &= ~(kRwBit);
  }
}

void LcdController::SetEnable(bool enable) {
  if (enable) {
    P2OUT |= kEnableBit;
  } else {
    P2OUT &= ~(kEnableBit);
  }
}
