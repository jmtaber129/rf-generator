#ifndef SI570_REGISTER_TRANSMITTER_H_
#define SI570_REGISTER_TRANSMITTER_H_

class Si570RegisterTransmitter {
  public:
    // Initializes the transmitter to transmit to 'slave_address'.
    Si570RegisterTransmitter(unsigned char slave_address);

    // Writes 'value' to the SI570 register at 'register_address'.
    void TransmitRegister(unsigned char register_address, unsigned char value);
};

#endif /* SI570_REGISTER_TRANSMITTER_H_ */
