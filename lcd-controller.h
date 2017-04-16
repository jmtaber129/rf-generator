#ifndef RF_GENERATOR_LCD_CONTROLLER_H_
#define RF_GENERATOR_LCD_CONTROLLER_H_

class LcdController {
  public:
    // Initialized the LCD's displayed frequency with 'initial_frequency'.
    LcdController(double initial_frequency);

    // Updates the LCD's displayed frequency with 'new_frequency' and the
    // active digit position with 'new_digit'.
    void Update(double new_frequency, int new_digit);

    // Updates the LCD's displayed frequency with 'new_frequency'.
    void UpdateFreq(double new_frequency);

    // Updates the LCD's active digit position with 'new_digit', where 0
    // corresponds to the 1kHz digit, and 4 corresponds to the 10MHz digit.
    void UpdateDigit(int new_digit);
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
    void DoHighToLow();

    // Performs a low-to-high transition of the enable signal, prompting a
    // read.
    void DoLowToHigh();

    // Blocks until the LCD is ready to receive a data or an instruction.
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

    // The current frequency.
    double curr_frequency_;

    // The current active digit position.  0 is 1kHz, 4 is 10MHz.
    int curr_digit_;

};

#endif /* RF_GENERATOR_LCD_CONTROLLER_H_ */
