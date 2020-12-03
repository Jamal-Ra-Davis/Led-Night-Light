#include<FastLED.h>
#include "Globals.h"
#include "Animations.h"
#define NUM_LEDS 36

CRGBArray<NUM_LEDS> leds;

typedef enum{OFF, CHRG0, CHRG1, CHRG2, CHRG3} animation_states_t;

animation_states_t state = OFF;

void set_led(int x, int y, CRGB color)
{
  leds[x + 12*y] = color;
}
CRGB get_led(int x, int y)
{
  return leds[x + 12*y];
}

#define NUM_ANIMATIONS 3
Animation0 a0(leds, NUM_LEDS, 10);
Animation1 a1(leds, NUM_LEDS, 5);
Animation2 a2(leds, NUM_LEDS, 1);
Animation *animations[NUM_ANIMATIONS] = {
  &a0,
  &a1,
  &a2  
};
static int animation_idx = 2;

int prev_ir_det; 
void setup() 
{
  Serial.begin(115200);
  
  //Setup Pins
  DDRD |= (1 << PD5);
  set_user_led(false);
  
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);
  
  pinMode(LED_EN, OUTPUT);
  digitalWrite(LED_EN, HIGH);

  pinMode(COIL_EN, OUTPUT);
  digitalWrite(COIL_EN, LOW);

  pinMode(IR_DET, INPUT);
  prev_ir_det = digitalRead(IR_DET);
  
  animations[animation_idx]->reset();
  
  FastLED.addLeds<NEOPIXEL, LED_DOUT>(leds, NUM_LEDS);
}

void main_loop();

int idx = 0;

bool state_change = true;
void loop()
{
  main_loop();
  //animations[animation_idx]->update();
  //delay(10);
  return;
  /*
  digitalWrite(IR_LED, LOW);
  digitalWrite(COIL_EN, LOW);
  set_user_led(false);
  delay(2500);
  digitalWrite(IR_LED, HIGH);
  digitalWrite(COIL_EN, HIGH);
  set_user_led(true);
  delay(2500);
  return;
  */
/*
  //FastLED.clear();
  for (int i=0; i<12; i++)
  {
    set_led((i+idx)%12, 2 - (i%3), CRGB(128, 50, 0));
    CRGB color = get_led((i+idx)%12, 2 - ((i + 1)%3));
    if (color.r > 0)
    {
      color.r >>= 1;
    }
    if (color.g > 0)
    {
      color.g >>= 1;
    }
    if (color.b > 0)
    {
      color.b >>= 1;
    }
    set_led((i+idx)%12, 2 - ((i + 1)%3), color);

    color = get_led((i+idx)%12, 2 - ((i + 2)%3));
    if (color.r > 0)
    {
      color.r >>= 1;
    }
    if (color.g > 0)
    {
      color.g >>= 1;
    }
    if (color.b > 0)
    {
      color.b >>= 1;
    }
    set_led((i+idx)%12, 2 - ((i + 2)%3), color);
  }
  FastLED.delay(100);
  idx++;
  if (idx >= 12)
  {
    idx = 0;
  }
  return;
*/
  for (int i=0; i<12; i++)
  {
    if (i == idx)
    {
      set_led(idx, 0, CRGB(255, 100, 0));
      set_led(11 - ((idx + 4)%12), 1, CRGB(0, 255, 100));
      set_led((idx + 8) % 12, 2, CRGB(100, 0, 255));
    }
    else
    {
      for (int j=0; j<3; j++)
      {
        CRGB color = get_led(i, j);
        if (color.r > 0)
        {
          color.r >>= 1;
        }
        if (color.g > 0)
        {
          color.g >>= 1;
        }
        if (color.b > 0)
        {
          color.b >>= 1;
        }
  
        set_led(i, j, color);
      }
    }
  }
  idx++;
  if (idx >= 12)
  {
    idx = 0;
  }
  FastLED.delay(50);
  return;
  /*
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
  */
  
  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value
    leds[i] = CHSV(hue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(50);
  }
}

typedef enum {PWR_OFF, PWR_ON} power_states_t;



void main_loop()
{
  static bool state_change = false;
  static bool prev_det = false;
  static power_states_t power_state = PWR_ON;
  static int cnt = 0;
  
  bool on_activate = false;
  bool on_deactivate = false;

  bool ir_det = digitalRead(IR_DET);
  if (ir_det && !prev_ir_det)
  {
    on_deactivate = true;
  }
  else if (!ir_det && prev_ir_det)
  {
    on_activate = true;
  }

  switch(power_state)
  {
    case PWR_ON:
    {
      if (state_change)
      {
        //On enter
        state_change = false;
        animation_idx = (animation_idx+1) % NUM_ANIMATIONS;
        animations[animation_idx]->reset();
        digitalWrite(LED_EN, HIGH);
        digitalWrite(COIL_EN, HIGH);
      }

      //State Action
      animations[animation_idx]->update();
      
      if (on_deactivate || cnt >= 1500)
      {
        power_state = PWR_OFF;
        state_change = true;
        cnt = 0;
      }
      break; 
    }
    case PWR_OFF:
    {
      if (state_change)
      {
        //On enter
        state_change = false;
        FastLED.clear();
        FastLED.show();
        digitalWrite(LED_EN, LOW);
        digitalWrite(COIL_EN, LOW);
      }

      //State Action

      if (on_activate || cnt >= 150)
      {
        power_state = PWR_ON;
        state_change = true;
        cnt = 0;
      }
      break;
    }
    default:
    {
      Serial.println("Invalid state!");
      break;
    }
  }
  cnt++;
  delay(10);
}

