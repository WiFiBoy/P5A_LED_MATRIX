// AURORA DEMO for LED MATRIX P5A
//
// mod from https://github.com/pixelmatix/aurora

#include "P5A-I2S-DMA.h"
#include "time.h"

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1 

P5A_MatrixPanel_I2S_DMA *dma_display = nullptr;

HUB75_I2S_CFG mxconfig(
	PANEL_RES_X,   // module width
	PANEL_RES_Y,   // module height
	PANEL_CHAIN    // Chain length
);

#include <FastLED.h>
#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
#include "Patterns.h"
Patterns patterns;

unsigned long fps = 0, fps_timer;
unsigned int default_fps = 30, pattern_fps = 30; 
unsigned long ms_animation_max_duration = 20000;
unsigned long last_frame=0, ms_previous=0;

void setup()
{
    Serial.begin(115200);
    delay(100);
  
    Serial.println("...Starting LED Matrix");
    dma_display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(10); // 0-255
    dma_display->clearScreen();  
    delay(100);  
    Serial.println("*** Starting Aurora Effects Demo ***");
  
    effects.Setup();
    delay(200);
    Serial.println("Effects being loaded: ");
    listPatterns();
  
    patterns.moveRandom(1);
  
    Serial.print("Starting with pattern: ");
    Serial.println(patterns.getCurrentPatternName());
    patterns.start();
    ms_previous = millis();
    fps_timer = millis();
}

void loop()
{
    if ((millis() - ms_previous) > ms_animation_max_duration) {
         patterns.stop();      
         patterns.moveRandom(1);
         patterns.start();  
         Serial.print("Changing pattern to:  ");
         Serial.println(patterns.getCurrentPatternName());
         ms_previous = millis();
    }
    if (1000 / pattern_fps + last_frame < millis()) {
        last_frame = millis();
        pattern_fps = patterns.drawFrame();
        if (!pattern_fps) pattern_fps = default_fps;
        ++fps;
    }
    if (fps_timer + 1000 < millis()) {
       Serial.printf_P(PSTR("Effect fps: %ld\n"), fps);
       fps_timer = millis();
       fps = 0;
    }
}

void listPatterns() 
{
  patterns.listPatterns();
}
