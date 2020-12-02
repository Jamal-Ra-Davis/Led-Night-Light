#ifndef GLOBALS_LIB_
#define GLOBALS_LIB_

#define LED_DOUT  6
#define IR_DET    7
#define LED_EN    8
#define COIL_EN   9
#define IR_LED    10

void set_user_led(bool on)
{
  if (on)
    PORTD |= (1 << PD5);
  else
    PORTD &= ~(1 << PD5);
}

#endif
