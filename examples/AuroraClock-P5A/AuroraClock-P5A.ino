// AURORA DEMO for LED MATRIX P5A with NTP Clock Example
//
// mod frtom https://github.com/pixelmatix/aurora
//
#include "P5A-I2S-DMA.h"
#include "time.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <esp_bt.h>

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

// 編譯前，請先修改您要連接WiFi的 AP, PWD
// 或是輸入手機打開 Internet 分享 "P5A"/"12345678"
char AP[]="P5A";
char PWD[]="12345678";

char timebuf[15], timestr[15], tmpbuf[15]; 
uint16_t key, bright, xbright;
const char* ntpServer = "tw.pool.ntp.org";
const int udpPort = 8888;
WiFiUDP udp;

void drawText(int x, int y, char *str, int color)
{
    dma_display->setTextColor(color);
    int i=0;
    while(str[i]) {
          dma_display->setCursor(x+i*7,y); dma_display->print(str[i]);
          dma_display->setCursor(x+i*7+1,y); dma_display->print(str[i]);
          i++;
    }
}

uint16_t getkey() 
{
  return(key = digitalRead(35)*4+digitalRead(34)*2+digitalRead(39));
}

void tone(byte pin, int freq, int t) // for ESP32 v2.x.x, tone() is already installed
{
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(25, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
  delay(t);
  ledcWriteTone(0, 0); // play tone
}

void setup()
{
    Serial.begin(115200);

    pinMode(25, OUTPUT); // Buzzer
    pinMode(39, INPUT);  // L Key
    pinMode(34, INPUT);  // M Key
    pinMode(35, INPUT);  // R Key

    //while(digitalRead(39)) {}
  
    Serial.println("...Starting P5A Clock Demo...");
    dma_display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    //dma_display->setBrightness8(8); // 0-255
    bright=8;
    dma_display->setPanelBrightness(bright);
    
    dma_display->clearScreen(); 
     
    dma_display->setCursor(1,1);
    dma_display->print("Connecting");
    dma_display->setCursor(1,11);
    dma_display->print("AP:\"P5A\"");
    dma_display->setCursor(1,21);
    dma_display->print("\"12345678\"");
    
    
    Serial.println("*** Waiting WiFi Connection ***");

    WiFi.begin(AP, PWD); 
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
    tone(25, 220, 30);

    dma_display->fillScreen(0);
    dma_display->setCursor(0,0);
    dma_display->print("Connected:");
    dma_display->println(WiFi.localIP());

    udp.begin(udpPort);
    configTime(8*3600, 0, ntpServer);
  
    while (time(nullptr) < 1000000000) {
        delay(1000);
        Serial.println("Waiting for time to be set...");
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  
    btStop();
    adc_power_off();
    esp_wifi_stop();
    esp_bt_controller_disable();
    
    Serial.println("Disconnected from WiFi");
    tone(25, 1280, 30);
    dma_display->fillScreen(0);

    // start to show clock and effects
  
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

    getkey();
    
    if (key==6) {
        if (bright>1) bright--;
        dma_display->setPanelBrightness(bright);
        tone(25, 440, 10);
        sprintf(timestr, "%02d", bright);
        drawText(24,24,timestr,0x0505);
        delay(100);
    } else if (key==5) {
        if (bright==0) {
            bright=xbright;
            //display = new MatrixPanel_I2S_DMA(mxconfig);
            //display->begin(); 
            dma_display->setPanelBrightness(bright);
            sprintf(timestr, "%02d", bright);
            drawText(24,24,timestr,0x0505);
        } else { 
            xbright=bright; 
            bright=0; 
            dma_display->setPanelBrightness(bright);
            //delete display;
        }
        tone(25, 440, 10);
        delay(200);
    } else if (key==3) {
        if (bright<48) bright++;
        dma_display->setPanelBrightness(bright);
        tone(25, 440, 10);
        sprintf(timestr, "%02d", bright);
        drawText(24,24,timestr,0x0505);
        delay(100);
    }

}

void listPatterns() 
{
  patterns.listPatterns();
}
