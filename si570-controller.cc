#include "si570-controller.h"

Si570Controller::Si570Controller(Si570RegisterCalculator* reg_calculator,
    Si570RegisterTransmitter* reg_transmitter, double initial_frequency) :
    reg_calculator_(reg_calculator), reg_transmitter_(reg_transmitter) {
  this->Update(initial_frequency);
}

void Si570Controller::Update(double new_frequency) {
  reg_calculator_->UpdateFrequency(new_frequency);

  // Transmit 'reg_calculator_' registers to chip.
  unsigned char reg7;
  unsigned char reg8;
  unsigned char reg9;
  unsigned char reg10;
  unsigned char reg11;
  unsigned char reg12;

  reg7 = reg_calculator_->get_reg7();
  reg8 = reg_calculator_->get_reg8();
  reg9 = reg_calculator_->get_reg9();
  reg10 = reg_calculator_->get_reg10();
  reg11 = reg_calculator_->get_reg11();
  reg12 = reg_calculator_->get_reg12();

  // Freeze the DCO (set bit 4 of register 137).
  // TODO(jmtaber129): Read register 137, then write 'reg_137 | BIT4' if
  // writing over other bits causes problems.
  reg_transmitter_->TransmitRegister(0x89, 0x10);

  // Write frequency configuration registers.
  reg_transmitter_->TransmitRegister(0x07, reg7);
  reg_transmitter_->TransmitRegister(0x08, reg8);
  reg_transmitter_->TransmitRegister(0x09, reg9);
  reg_transmitter_->TransmitRegister(0x0a, reg10);
  reg_transmitter_->TransmitRegister(0x0b, reg11);
  reg_transmitter_->TransmitRegister(0x0c, reg12);

  // Unfreeze the DCO (clear bit 4 of register 137).
  // TODO(jmtaber129): Read register 137, then write 'reg_137 | BIT4' if
  // writing over other bits causes problems.
  reg_transmitter_->TransmitRegister(0x89, 0x00);

  // Assert NewFreq (set bit 6 of register 135);
  reg_transmitter_->TransmitRegister(0x87, 0x40);

}
