//******************************************************************************
//  MSP430G2xx3 Demo - USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the slave code. The interrupt driven
//  data receiption is demonstrated using the USCIB0 RX interrupt.
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.2MHz
//
//                                /|\  /|\
//               MSP430G2553      10k  10k     MSP430G2553
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P1.7/UCB0SDA|<-|---+->|P1.7/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P1.6/UCB0SCL|<-+----->|P1.6/UCB0SCL     |
//            |                 |         |                 |
//
//  D. Dang
//  Texas Instruments Inc.
//  February 2011
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//
//  Modified by James Taber
//  Texas Tech University
//  February 2017
//******************************************************************************
#include "msp430g2553.h"

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM
volatile unsigned char registers[16];

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB0I2COA = 0x48;                         // Own Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0I2CIE |= UCSTPIE + UCSTTIE;           // Enable STT and STP interrupt
  IE2 |= UCB0RXIE;                          // Enable RX interrupt

  while (1)
  {
    PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
    RXByteCtr = 0;                          // Clear RX byte count
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until master
                                            // finishes TX
    registers[RxBuffer[0] - 7] = RxBuffer[1];

    __no_operation();                       // Set breakpoint >>here<< and read
  }                                         // read out the RxData buffer or
                                            // the 'registers' buffer.
}

//------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C master
// to the MSP430 memory.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  *PRxData++ = UCB0RXBUF;                   // Move RX data to address PRxData
  RXByteCtr++;                              // Increment RX byte count
}

//------------------------------------------------------------------------------
// The USCI_B0 state ISR is used to wake up the CPU from LPM0 in order to
// process the received data in the main program. LPM0 is only exit in case
// of a (re-)start or stop condition when actual data was received.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);       // Clear interrupt flags
  if (RXByteCtr)                            // Check RX byte counter
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0 if data was
}                                           // received
