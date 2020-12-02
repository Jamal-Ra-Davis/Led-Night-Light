#include<FastLED.h>
#include "Globals.h"
#define NUM_LEDS 36

CRGBArray<NUM_LEDS> leds;

typedef enum{OFF, CHRG0, CHRG1, CHRG2, CHRG3} animation_states_t;

animation_states_t state = OFF;

void set_led(int x, int y, CRGB color)
{
  leds[x + 12*y] = color;
}

int prev_ir_det; 
void setup() 
{
  //Setup Pins
  DDRD |= (1 << PD5);
  set_user_led(false);
  
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);
  
  pinMode(LED_EN, OUTPUT);
  digitalWrite(LED_EN, LOW);

  pinMode(COIL_EN, OUTPUT);
  digitalWrite(COIL_EN, LOW);

  pinMode(IR_DET, INPUT);
  prev_ir_det = digitalRead(IR_DET);
  
  
  FastLED.addLeds<NEOPIXEL, LED_DOUT>(leds, NUM_LEDS);
}

bool state_change = true;
void loop()
{
  FastLED.clear();
  int ir_det = digitalRead(IR_DET);
  if (ir_det == 1)
  {
    //Idle
    if (ir_det != prev_ir_det)
    {
      //Entering idle

      //
      digitalWrite(COIL_EN, LOW);
    }
    else
    {
      
    }
  }
  else
  {
    //Charging
    if (ir_det != prev_ir_det)
    {
      //Entering charging state
      digitalWrite(COIL_EN, HIGH);
    }
    else
    {
      
    }
  }
  prev_ir_det = ir_det;
  FastLED.delay(20);
  return;
  
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value
    leds[i] = CHSV(hue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(33);
  }
}

