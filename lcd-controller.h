#ifndef RF_GENERATOR_LCD_CONTROLLER_H_
#define RF_GENERATOR_LCD_CONTROLLER_H_

class LcdController {
  public:
    // Initialized the LCD's displayed frequency with 'initial_frequency'.
    LcdController(double initial_frequency);

    // Updates the LCD's displayed frequency with 'new_frequency'.
    void Update(double new_frequency);
};

#endif /* RF_GENERATOR_LCD_CONTROLLER_H_ */
