#include "configuration.h"

#include <msp430g2553.h>

void Configuration::Init() {
  ConfigureClocks();

  __bis_SR_register(GIE);
}

void Configuration::ConfigureClocks() {
  // Note: If you modify clock speeds or sources, you need to change the timer
  // configuration methods, too.
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
}
