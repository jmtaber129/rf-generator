#include "LCD.h"

#define CHA BIT0
#define CHB BIT1


volatile int A = 0, B = 0, A_PREV = 0, B_PREV = 0, update = 1, increment = 0;

void update_Station(void);

void main(void) {
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT

  P2DIR &= ~(CHA + CHB);  // Set Pins 2.0 & 2.1 as Inputs for rotary encoder
  P2REN |= (CHA + CHB);   // Enable Resistor
  P2OUT |= (CHA + CHB);   // Set pull-up resistor
  P2IES &= ~(CHA + CHB);    // High-to-Low
  P2IE |= (CHA + CHB);    // Enable Port 2 Interrupts

  P1IFG = 0;  // Clear P1 interrupt-flags
    P2IFG = 0;  // Clear P2 interrupt flags.

    delay(1000);
    initialize_LCD();

    A = (P2IN & CHA) ? 1 : 0;
    B = (P2IN & CHB) ? 1 : 0;
    __enable_interrupt();

    while(1){
      if(update == 1) {
        //update_LCD();   // Update Text displayed on LCD
        update = 0;     // Reset update to 0
      }
    }
}

 /*-----------------------------------------------------------------------------\\
 || Port 2 Interrupt Service Routine                      ||  !
 || Triggered on Port 2.0 & 2.1 Interrupts (triggered by EN11 Rotary Encoder) ||
 || Updates values of A, A_PREV, B, B_PREV, and update              ||
 \\-----------------------------------------------------------------------------*/
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){

  if(P2IFG & CHB){
    if(P2IN & CHB){
      increment = 0;
    }
    else{
      increment = 1;
    }
    update = 1;
  }
  else{
    if(P2IN & CHA){
      increment = 1;
    }
    else{
      increment = 0;
    }
    update = 1;
  }

  P2IFG &= 0;           // Clear Port 2 Interrupt Flag

}
