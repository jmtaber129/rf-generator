#ifndef SI570_REGISTER_TRANSMITTER_H_
#define SI570_REGISTER_TRANSMITTER_H_

// Describes what will occur during the next TxIsr() call.
enum TransmitState {
  kTransmitRegister,
  kTransmitValue,
  kTransmitStop,
};

class Si570RegisterTransmitter {
  public:
    // Initializes the transmitter to transmit to 'slave_address'.
    Si570RegisterTransmitter(unsigned char slave_address);

    // Writes 'value' to the SI570 register at 'register_address'.
    void TransmitRegister(unsigned char register_address, unsigned char value);

    static bool TxIsr();

  private:
    // The state of the transmit state machine.
    static TransmitState transmit_state_;

    // The register to be transmitted.
    static unsigned char transmit_register_;

    // The value to be transmitted
    static unsigned char transmit_value_;
};

#endif /* SI570_REGISTER_TRANSMITTER_H_ */
