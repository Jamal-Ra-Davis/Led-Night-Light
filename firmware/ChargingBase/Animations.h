#ifndef ANIMATIONS_LIB_
#define ANIMATIONS_LIB_

#include<FastLED.h>
#include "Arduino.h"

extern void set_led(int x, int y, CRGB color);
extern CRGB get_led(int x, int y);

CRGB randColor()
{
  CRGB out;
  uint8_t sel = rand() % 6;

  switch (sel)
  {
    case 0:
      out.r= 255;
      out.b = 0;
      out.g = rand() % 256;
      break;
    case 1:
      out.r = 255;
      out.b = rand() % 256;
      out.g = 0;
      break;
    case 2:
      out.r = 0;
      out.b = 255;
      out.g = rand() % 256;
      break;
    case 3:
      out.r = rand() % 256;
      out.b = 255;
      out.g = 0;
      break;
    case 4:
      out.r = 0;
      out.b = rand() % 256;
      out.g = 255;
      break;
    case 5:
      out.r = rand() % 256;
      out.b = 0;
      out.g = 255;
      break;
  }
  return out;
}

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

class Animation3 : public Animation
{
  private:
    uint8_t offset;
  public:
    Animation3(CRGB *leds_, int size, int delay_cnt_);
    void reset();
    void update();
};

typedef struct snake_segment
{
  uint8_t x, y;
  CRGB color;
} snake_segment_t;
class Animation4 : public Animation
{
  private:
    typedef enum {CCW, OUT, CW, IN} direction_t;//x+, y+, x-, y-
    const char* DIR_NAMES[4] = {"CCW", "OUT", "CW", "IN"};
    static const uint8_t MAX_SIZE = 20;
    uint8_t dot_x, dot_y;
    uint8_t head_x, head_y;
    uint8_t length;
    snake_segment_t dot;
    snake_segment_t body[MAX_SIZE];
    direction_t dir;
    uint8_t hue;
    uint8_t breath;
    bool first;

    snake_segment_t getDot();
    direction_t getDirection(snake_segment_t *dst, snake_segment *src, direction_t dir_i);
  public:
    Animation4(CRGB *leds_, int size, int delay_cnt_);
    void reset();
    void update();
};

//-------------------------------------

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

//--------------------------------

Animation3::Animation3(CRGB *leds_, int size, int delay_cnt_) : Animation(leds_, size, delay_cnt_)
{
  offset = 0;
  dly = 0;
}
void Animation3::reset()
{
  offset = 0;
  dly = 0;
}
void Animation3::update()
{
  if ((dly++ % delay_cnt) != 0)
  {
    return;
  }

  for (int j=0; j<3; j++)
  {
    int hue = j*50;
    for (int i=0; i<12; i++)
    {
      set_led(i, j, CHSV(hue+offset, 255, 255));
    }
  }
  offset+=1;
  FastLED.show();
}

//--------------------------------

snake_segment_t Animation4::getDot()
{
  snake_segment_t out_dot;
  uint8_t x, y;
  //Spawn new dot
  while(1)
  {
    x = rand() % 12;
    y = rand() % 3;
    bool dot_okay = true;
    for (int i=0; i<length; i++)
    {
      if (x == body[i].x && y == body[i].y)
      {
        dot_okay = false;
        break;
      }
    }
    if (dot_okay == true)
      break;
  }
  out_dot.x = x;
  out_dot.y = y;
  out_dot.color = CHSV(hue, 255, 255);//randColor();
  hue+=16;
  return out_dot;
}
Animation4::direction_t Animation4::getDirection(snake_segment_t *dst, snake_segment *src, direction_t dir_i)
{
  direction_t out_dir = dir_i;
    
  //Determine direction: 
  int8_t delta_x = dst->x - src->x;
  int8_t delta_y = dst->y - src->y; 

  if (delta_x > 6)
  {
    delta_x -= 12;
  }
  else if (delta_x < -6)
  {
    delta_x += 12;
  }
  
  int8_t abs_delta_x = delta_x;
  int8_t abs_delta_y = delta_y;
  if (abs_delta_x < 0)
    abs_delta_x *= -1;
  if (abs_delta_y < 0)
    abs_delta_y *= -1;

/*
  //Correct direction in the larger direction first
  if (abs_delta_x >= abs_delta_y)
  {
    if (delta_x >= 0)
    {
      out_dir = CCW;
    }
    else
    {
      out_dir = CW;
    }
  }
  else
  {
    if (delta_y >= 0)
    {
      out_dir = OUT;
    }
    else
    {
      out_dir = IN;
    }
  }
*/
  switch(dir_i)
  {
    case CCW:
    {
      //x++;
      if (abs_delta_x >= abs_delta_y)
      {
        //Larger gap in X dir
        if (delta_x >= 0)
        {
          //Do nothing
        }
        else
        {
          if (body[0].y < 2)
          {
            //If y pos is 0 or 1,
            out_dir = OUT;
          }
          else
          {
            out_dir = IN;
          }
        }
      }
      else
      {
        //Larger gap in Y dir
        if (delta_y >= 0)
        {
          out_dir = OUT;
        }
        else
        {
          out_dir = IN;
        }
      }
      break;
    }
    case CW:
    {
      //x--;
      if (abs_delta_x >= abs_delta_y)
      {
        //Larger gap in X dir
        if (delta_x >= 0)
        {
          if (body[0].y < 2)
          {
            //If y pos is 0 or 1,
            out_dir = OUT;
          }
          else
          {
            out_dir = IN;
          }
        }
        else
        {
          //Do nothing
        }
      }
      else
      {
        //Larger gap in Y dir
        if (delta_y >= 0)
        {
          out_dir = OUT;
        }
        else
        {
          out_dir = IN;
        }
      }
      break;
    }
    case OUT:
    {
      //y++;
      if (abs_delta_x >= abs_delta_y)
      {
        //Larger gap in X dir
        if (delta_x >= 0)
        {
          out_dir = CCW;
        }
        else
        {
          out_dir = CW;
        }
      }
      else
      {
        //Larger gap in Y dir
        if (delta_y >= 0)
        {
          //Do nothing
        }
        else
        {
          out_dir = CW;
        }
      }
      break;
    }
    case IN:
    {
      //y--;
      if (abs_delta_x >= abs_delta_y)
      {
        //Larger gap in X dir
        if (delta_x >= 0)
        {
          out_dir = CCW;
        }
        else
        {
          out_dir = CW;
        }
      }
      else
      {
        //Larger gap in Y dir
        if (delta_y >= 0)
        {
          out_dir = CCW;
        }
        else
        {
          //Do nothing
        }
      }
      break;
    }
  }

  
  return out_dir;
}
Animation4::Animation4(CRGB *leds_, int size, int delay_cnt_) : Animation(leds_, size, delay_cnt_)
{
  dly = 0;
  first = true;
  hue = 0;
  breath = 64;
}
void Animation4::reset()
{
  dly = 0;
  first = true;

  hue = 0;
  uint8_t x = 5;//rand() % 12;
  uint8_t y = 1;//rand() % 3;
  length = 3;
  for (int i=0; i<length; i++)
  {
    body[i].x = x-i;
    body[i].y = y;
    body[i].color = CRGB(255, 255, 255);
  }

  dir = CCW;
  dot = getDot();
  //dir = getDirection(&dot, &(body[0]), dir);
}
void Animation4::update()
{
  int x, y;
  direction_t prev_dir;
  CRGB d_color;
  breath += 4;
  if (breath < 64)
  {
    breath = 64;
  }
  if ((dly++ % delay_cnt) != 0)
  {
    //return;
    goto snake_draw;
  }

  //Serial.print("X: ");
  //Serial.println(length);
  FastLED.clear();
  if (first)
  {
    set_led(dot.x, dot.y, dot.color);
    for (int i=length-1; i>=0; i--)
    {
      set_led(body[i].x, body[i].y, body[i].color);
    }
    FastLED.show();
    first = false;
    return;
  }
  
  //Update
  //Choose new direction
  prev_dir = dir;
  dir = getDirection(&dot, &(body[0]), dir);

  while (0)
  {
    dot.x = 8;
    dot.y = 0;
    body[0].x = 8;
    body[0].y = 1;
    direction_t test_dir = getDirection(&dot, &(body[0]), IN);
    Serial.print("DIR = IN, src = (8,1), dst = (8,0), new_dir = ");
    Serial.println(DIR_NAMES[test_dir]);
    delay(5000);
  }
  
  x = body[0].x;
  y = body[0].y;
  switch(dir)
  {
    case CCW:
      x++;
      break;
    case CW:
      x--;
      break;
    case OUT:
      y++;
      break;
    case IN:
      y--;
      break;
  }


  //Bounds Check
  if (x < 0)
  {
    x = 11;
  }
  if (x >= 12)
  {
    x = 0;
  }
  if (y < 0 || y >= 3)
  {
    Serial.println("Out of bounds");
    char buf[64];
    sprintf(buf, "head = (%d, %d)\n", body[0].x, body[0].y);
    Serial.print(buf);
    goto do_reset;
  }

  char up_buf[100];
  sprintf(up_buf, "dot = (%d, %d), prev_pos = (%d, %d), pos = (%d, %d),  prev_dir = %s, dir = %s\n", 
    dot.x, dot.y, 
    body[0].x, body[0].y,
    x, y,
    Animation4::DIR_NAMES[prev_dir],
    Animation4::DIR_NAMES[dir]);
  Serial.print(up_buf);

  for (int i=length-1; i >= 1; i--)
  {
    body[i].x = body[i-1].x;
    body[i].y = body[i-1].y;
  }
  body[0].x = x;
  body[0].y = y;

  //Collided with self
  for (int i=1; i<length; i++)
  {
    if (body[0].x == body[i].x && body[0].y == body[i].y)
    {
      Serial.println("Collided with self");
      Serial.print("DIR: ");
      Serial.println(dir);
      char buf[64];
      sprintf(buf, "head = (%d, %d)\n", body[0].x, body[0].y);
      Serial.print(buf);
      for (int i=1; i<length; i++)
      {
        sprintf(buf, "body[%d] = (%d, %d)\n", i, body[i].x, body[i].y);
        Serial.print(buf);
      }
      Serial.println();
      goto do_reset;
    }
  }

  //Snake got dot
  if (body[0].x == dot.x && body[0].y == dot.y)
  {
    if (length < MAX_SIZE)
    {
      body[length] = dot;
      length++;  
    }

    Serial.println("Got dot, spawning new one");
    //Spawn new dot
    dot = getDot();
  }


  //Choose new direction
  //dir = getDirection(&dot, &(body[0]), dir);

snake_draw:
  //Draw
  d_color = dot.color;
  d_color.r = breath;
  set_led(dot.x, dot.y, dot.color);
  for (int i=length-1; i>=0; i--)
  {
    set_led(body[i].x, body[i].y, body[i].color);
  }
  FastLED.show();
  return;

do_reset:
  d_color = dot.color;
  d_color.r = breath;
  set_led(dot.x, dot.y, dot.color);
  for (int i=length-1; i>=0; i--)
  {
    set_led(body[i].x, body[i].y, body[i].color);
  }
  FastLED.show();
  reset();
}

#endif
