#ifndef RF_GENERATOR_CONFIGURATION_H_
#define RF_GENERATOR_CONFIGURATION_H_

class Configuration {
  public:
    // Configures MSP430G2553 registers.
    static void Init();
  private:
    // Configures the MSP430G2553 clocks.
    static void ConfigureClocks();
};

#endif /* RF_GENERATOR_CONFIGURATION_H_ */
