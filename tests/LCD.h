#ifndef LCD_H_
#define LCD_H_

#include <msp430g2553.h>

#define DR P1OUT = P1OUT | BIT4       // define RS high
#define CWR P1OUT = P1OUT & (~BIT4)     // define RS low
#define READ P1OUT = P1OUT | BIT5       // define Read signal R/W = 1 for reading
#define WRITE P1OUT = P1OUT & (~BIT5)   // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT = P1OUT | BIT6  // define Enable high signal
#define ENABLE_LOW P1OUT = P1OUT & (~BIT6)  // define Enable Low signal


unsigned int i, j;

void delay(unsigned int k) {
  for(j=0;j<=k;j++) {
    for(i=0;i<100;i++);
  }
}

void data_write(void) {
  ENABLE_HIGH;
  delay(2);
  ENABLE_LOW;
}

void data_read(void) {
  ENABLE_LOW;
  delay(2);
  ENABLE_HIGH;
}

void check_busy(void){
  P1DIR &= ~(BIT3);   // Set P1.3 as input
  while((P1IN&BIT3)==1) {
    data_read();
  }
  P1DIR |= BIT3;    // Set P1.3 as output
}

void write_Instruction(unsigned char cmd) {
  check_busy();
  WRITE;
  CWR;
  P1OUT = (P1OUT & 0xF0)|((cmd>>4) & 0x0F); // Send higher nibble
  data_write();                 // Give enable trigger
  P1OUT = (P1OUT & 0xF0)|(cmd & 0x0F);    // Send lower nibble
  data_write();                 // Give enable trigger
}

void send_data(unsigned char data) {
  check_busy();
  WRITE;
  DR;
  P1OUT = (P1OUT & 0xF0)|((data>>4) & 0x0F);  // Send higher nibble
  data_write();                 // Give enable trigger
  P1OUT = (P1OUT & 0xF0)|(data & 0x0F);     // Send lower nibble
  data_write();                 // Give enable trigger
}

void write_String(char *s){
  while(*s) {
    send_data(*s);
    s++;
  }
}

/*
void update_LCD(void){
  write_Instruction(0x01u); // Clear Display
  write_String("");
  write_String("");
  write_Instruction(0xC6);
}
*/

void initialize_LCD(void) {
  P1DIR |= 0xFF;
  P1OUT &= 0x00;
  write_Instruction(0x33u); // Reset
  write_Instruction(0x32u); // Reset
  write_Instruction(0x28u); // Enable 4-bit Mode
  write_Instruction(0x0Cu); // Turn Display On, Cursor Off, Blink Off
  write_Instruction(0x01u); // Clear Display
  write_Instruction(0x06u); // Set Increment Mode

  write_Instruction(0x85u);
    write_String("Hello");
    write_Instruction(0xC6);
    write_String("World");
}
#endif /* LCD_H_ */
