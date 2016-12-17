#include "config.h"
#include "mode.h"

extern Config config;
extern Adafruit_NeoPixel strip;

#define RGB  (config.leds==3)
#define RGBW (config.leds==4)

/*
  mode 0: individual pixel control
  channel 1  = pixel 1 red
  channel 2  = pixel 1 green
  channel 3  = pixel 1 blue
  channel 4  = pixel 1 white
  channel 5  = pixel 2 red
  etc.
*/

void mode0(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
  for (int i = 0; i < length / config.leds; i++) {
    int pixel   = i + (universe - config.universe) * 512;
    int channel = i * config.leds + config.offset;

    if (pixel >= 0 && pixel < strip.numPixels())
      if (channel >= 0 && (channel + config.leds) < length) {
        if (RGB)
          strip.setPixelColor(pixel, data[channel + 0], data[channel + 1], data[channel + 2]);
        else if (RGBW)
          strip.setPixelColor(pixel, data[channel + 0], data[channel + 1], data[channel + 2], data[channel + 3]);
      }
  }
  strip.show();
} // mode0


/*
  mode 1: single uniform color
  channel 1  = red
  channel 2  = green
  channel 3  = blue
  channel 4  = white
  channel 5 = intensity (this allows scaling a preset RGBW color with a single channel)
*/

void mode1(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
  int r, g, b, w, intensity;
  if (universe != config.universe)
    return;
  if (RGB && (length - config.offset) < 4)
    return;
  if (RGBW && (length - config.offset) < 5)
    return;
  if (RGB) {
    r = data[config.offset + 0];
    g = data[config.offset + 1];
    b = data[config.offset + 2];
    intensity = data[config.offset + 3];
  }
  else if (RGBW) {
    r = data[config.offset + 0];
    g = data[config.offset + 1];
    b = data[config.offset + 2];
    w = data[config.offset + 3];
    intensity = data[config.offset + 3];
  }
  // scale with the intensity
  r = (r * intensity) / 255;
  g = (g * intensity) / 255;
  b = (b * intensity) / 255;
  w = (w * intensity) / 255;
  for (int pixel = 0; pixel < strip.numPixels(); pixel++) {
    if (RGB)
      strip.setPixelColor(pixel, r, g, b);
    else if (RGBW)
      strip.setPixelColor(pixel, r, g, b, w);
  }
  strip.show();
} // mode1

/*
  mode 2: two color mixing
  channel 1  = color 1 red
  channel 2  = color 1 green
  channel 3  = color 1 blue
  channel 4  = color 1 white
  channel 5 = color 1 intensity
  channel 6  = color 2 red
  channel 7  = color 2 green
  channel 8  = color 2 blue
  channel 9  = color 2 white
  channel 10 = ratio (between color 1 and color2)
  channel 11 = intensity (this also allows to black out the colors)
*/

void mode2(uint16_t, uint16_t, uint8_t, uint8_t *) {}

/*
  mode 3: single uniform color, blinking between the color and black
  channel 1  = red
  channel 2  = green
  channel 3  = blue
  channel 4  = white
  channel 5 = intensity
  channel 6 = speed (number of flashes per unit of time)
  channel 7 = ramp (to specify whether there is a abrubt or more smooth transition)
  channel 8 = duty cycle (what is the time ratio between color and black)
*/

void mode3(uint16_t, uint16_t, uint8_t, uint8_t *) {}

/*
  mode 4: uniform color, blinking between color 1 and color 2
  channel 1  = color 1 red
  channel 2  = color 1 green
  channel 3  = color 1 blue
  channel 4  = color 1 white
  channel 5 = color 1 intensity
  channel 6  = color 2 red
  channel 7  = color 2 green
  channel 8  = color 2 blue
  channel 9  = color 2 white
  channel 10 = intensity
  channel 11 = speed
  channel 12 = ramp (here it is the time it takes to transition, i.e color transitions can be abrupt, but also gradually fused)
  channel 13 = duty cycle
*/
