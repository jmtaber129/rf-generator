#ifndef RF_GENERATOR_LCD_CONTROLLER_H_
#define RF_GENERATOR_LCD_CONTROLLER_H_

class LcdController {
  public:
    // Initialized the LCD's displayed frequency with 'initial_frequency'.
    LcdController(double initial_frequency);

    // Updates the LCD's displayed frequency with 'new_frequency' and the
    // active digit position with 'new_digit'.
    void Update(double new_frequency, int new_digit);

  private:
    // Clears the display.
    void ClearDisplay();

    // Sets the LCD cursor position to ('row', 'column').  'row' should be
    // between 0 and 1, inclusive.  'column' should be between 0 and 15,
    // inclusive.
    void SetPosition(int row, int column);

    // Writes 'str' to the LCD, starting at the current cursor position.
    void Write(char* str);

    // Transmits the char 'c' to the LCD.  Sends 'c' as an instruction if
    // 'is_instruction' is true, otherwise sends 'c' as data.
    void SendChar(unsigned char c, bool is_instruction);

    // Performs a high-to-low transition of the enable signal, prompting a
    // write.
    void DoWriteEdge();

    // Performs a low-to-high transition of the enable signal, prompting a
    // read.
    void DoReadEdge();

    // Blocks until the LCD is ready to receive data or an instruction.
    void WaitUntilNotBusy();

    // Blocks for a period of time linear to 'delay_time'.
    void Delay(int delay_time);

    // Sets the RS bit high if 'rs' is true, and low otherwise.
    void SetRs(bool rs);

    // Sets the read/write bit high (read) if 'rw' is true, and low (write)
    // otherwise.
    void SetRw(bool rw);

    // Sets the enable bit high if 'enable' is true, and low otherwise.
    void SetEnable(bool enable);

    // Instruction characters.
    static const unsigned char kFunctionSet1;
    static const unsigned char kFunctionSet2;
    static const unsigned char kFunctionSet3;
    static const unsigned char kDisplayOn;
    static const unsigned char kDisplayClear;
    static const unsigned char kSetIncrement;

    // Control bits (on Port 2).
    static const unsigned int kRsBit;
    static const unsigned int kRwBit;
    static const unsigned int kEnableBit;

    // Bit for the busy flag (on Port 1).
    static const unsigned char kBusyFlag;

    // Lookup for mapping char nibbles to output pins.
    static const unsigned char kNibbleLookup[16];
};

#endif /* RF_GENERATOR_LCD_CONTROLLER_H_ */
