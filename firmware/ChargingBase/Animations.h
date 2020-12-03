#ifndef ANIMATIONS_LIB_
#define ANIMATIONS_LIB_

#include<FastLED.h>
#include "Arduino.h"

extern void set_led(int x, int y, CRGB color);
extern CRGB get_led(int x, int y);

class Animation
{
  protected:
    int NUM_LEDS;
    CRGB *leds;
    int delay_cnt;
    int dly;
  public:
    Animation(CRGB *leds_, int size, int delay_cnt_);
    virtual void reset()=0;
    virtual void update()=0;
    //void draw();
};
Animation::Animation(CRGB *leds_, int size, int delay_cnt_)
{
  NUM_LEDS = size;
  leds = leds_;
  delay_cnt = delay_cnt_;
}

class Animation0 : public Animation
{
  private:
    int idx;
  public:
    Animation0(CRGB *leds_, int size, int delay_cnt_);
    void reset();
    void update();
};

class Animation1 : public Animation
{
  private:
    int idx;
  public:
    Animation1(CRGB *leds_, int size, int delay_cnt_);
    void reset();
    void update();
};

class Animation2 : public Animation
{
  private:
    uint8_t offset;
  public:
    Animation2(CRGB *leds_, int size, int delay_cnt_);
    void reset();
    void update();
};

Animation0::Animation0(CRGB *leds_, int size, int delay_cnt_) : Animation(leds_, size, delay_cnt_)
{
  idx = 0;
  dly = 0;
}
void Animation0::reset()
{
  idx = 0;
  dly = 0;
}
void Animation0::update()
{
  if ((dly++ % delay_cnt) != 0)
  {
    return;
  }
  
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
  idx = (idx+1) % 12;
  FastLED.show();
}

//--------------------------------

Animation1::Animation1(CRGB *leds_, int size, int delay_cnt_) : Animation(leds_, size, delay_cnt_)
{
  idx = 0;
  dly = 0;
}
void Animation1::reset()
{
  idx = 0;
  dly = 0;
}
void Animation1::update()
{
  if ((dly++ % delay_cnt) != 0)
  {
    return;
  }
  
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
  idx = (idx+1) % 12;
  FastLED.show();
}

//--------------------------------

Animation2::Animation2(CRGB *leds_, int size, int delay_cnt_) : Animation(leds_, size, delay_cnt_)
{
  offset = 0;
  dly = 0;
}
void Animation2::reset()
{
  offset = 0;
  dly = 0;
}
void Animation2::update()
{
  if ((dly++ % delay_cnt) != 0)
  {
    return;
  }
  
  for (int i=0; i<12; i++)
  {
    int hue = i*(255/12);
    set_led(i, 0, CHSV(hue+offset+60, 255, 255));
    set_led(i, 1, CHSV(hue+offset+30, 255, 255));
    set_led(i, 2, CHSV(hue+offset+0, 255, 255));
  }
  offset+=3;
  FastLED.show();
}

#endif
