// NTP Clock Example for LED MATRIX P5A
//
// V1.0 (Feb 17, 2023) WiFiBoy Computing Lab. Taiwan

#include <WiFi.h>
#include <WiFiUdp.h>
#include "time.h"

// 編譯前，請先修改您要連接WiFi的 AP, PWD
// 或是在您的手機打開 P5A/12345678 的 Internet 網路分享
char AP[]="P5A";
char PWD[]="12345678";

char timebuf[15], timestr[15], tmpbuf[15]; 
uint16_t key, bright, xbright;
const char* ntpServer = "tw.pool.ntp.org";
const int udpPort = 8888;
WiFiUDP udp;

#include "P5A-I2S-DMA.h"
P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

uint8_t font_num[35]={
  0x7e,0x42,0x7e,0x42,0x7e, 0x00,0x00,0x7e,0x00,0x00, 0x00,0x7c,0x00,0x00,0xfe, 0x7c,0x00,0x7c,0x00,0xfe,
  0xfe,0xaa,0xaa,0xaa,0xfe, 0xfe,0x10,0x7c,0x14,0xfe, 0x10,0xfe,0x28,0x44,0x82
};
uint8_t bits[8]={128,64,32,16,8,4,2,1};

void show_weekday(int digit, int x, int y, int color)
{
  for(int i=0; i<5; i++)
    for(int j=0; j<7; j++) 
      if (font_num[digit*5+i]&bits[0+j]) display->drawPixel(x+j, y+i, color);
      else display->drawPixel(x+j, y+i, 0);
}

void drawText(int x, int y, char *str, int color)
{
    display->setTextColor(color);
    int i=0;
    while(str[i]) {
          display->setCursor(x+i*7,y); display->print(str[i]);
          display->setCursor(x+i*7+1,y); display->print(str[i]);
          i++;
    }
}

uint16_t getkey() 
{
  return(key = digitalRead(35)*4+digitalRead(34)*2+digitalRead(39));
}

void tone(byte pin, int freq, int t) {
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(25, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
  delay(t);
  ledcWriteTone(0, 0); // play tone
}

void setup() 
{
    setCpuFrequencyMhz(80);
    
    Serial.begin(115200);

    pinMode(25, OUTPUT); // Buzzer
    pinMode(39, INPUT);  // L Key
    pinMode(34, INPUT);  // M Key
    pinMode(35, INPUT);  // R Key
    pinMode(15, OUTPUT); // OE

    bright = 10;
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(bright);
    display->setCursor(0,0);
    display->print("Connecting..");
      
    WiFi.begin(AP, PWD); 
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
    tone(25, 220, 30);

    display->fillScreen(0);
    display->setCursor(0,0);
    display->print("Connected:");
    display->println(WiFi.localIP());
  
    udp.begin(udpPort);
    configTime(8*3600, 0, ntpServer);
  
    while (time(nullptr) < 1000000000) {
        delay(1000);
        Serial.println("Waiting for time to be set...");
    }
    WiFi.disconnect(true);
    Serial.println("Disconnected from WiFi");
    tone(25, 1280, 30);
    display->fillScreen(0);
}

void loop() 
{
    time_t now = time(nullptr);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
  
    sprintf(timebuf, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min); //, timeinfo->tm_sec);
    if (strcmp(timebuf, timestr)!=0) {
        strcpy(timestr, timebuf);
        Serial.println(timebuf);
        display->fillScreen(0);
        sprintf(tmpbuf, "%4d", timeinfo->tm_year+1900);
        drawText(1,1,tmpbuf,0x6300); //0xc600);
        sprintf(tmpbuf,"%02d", timeinfo->tm_mon+1);
        drawText(29,1,tmpbuf,0x18c); //0x0318);
        sprintf(tmpbuf, "%02d", timeinfo->tm_mday);
        drawText(43,1,tmpbuf,0x0285); //0x0505);
        if (timeinfo->tm_wday!=0) show_weekday(timeinfo->tm_wday, 56, 2, 0x8410);
        else show_weekday(timeinfo->tm_wday, 56, 2, 0x5000);
        display->setTextColor(0xF000);
        display->setTextSize(2);
        display->setCursor(2,13);
        display->print(timebuf);
        display->setTextSize(1);
    }

    getkey();
    
    if (key==6) {
        if (bright>1) bright--;
        display->setPanelBrightness(bright);
        tone(25, 440, 10);
        sprintf(timestr, "%02d", bright);
        drawText(24,24,timestr,0x0505);
    } else if (key==5) {
        if (bright==0) {
            bright=xbright;
            display->setPanelBrightness(bright);
            sprintf(timestr, "%02d", bright);
            drawText(24,24,timestr,0x0505);
        } else { 
            xbright=bright; 
            bright=0; 
            display->setPanelBrightness(bright);
        }
        tone(25, 440, 10);
        delay(200);
    } else if (key==3) {
        if (bright<32) bright++;
        display->setPanelBrightness(bright);
        tone(25, 440, 10);
        sprintf(timestr, "%02d", bright);
        drawText(24,24,timestr,0x0505);
    }

    // test nighttime brightness auto dim
    if (timeinfo->tm_hour==0 && timeinfo->tm_min==0 && timeinfo->tm_sec==0) {
        bright=1;
        display->setPanelBrightness(bright);
    }
    if (timeinfo->tm_hour==6 && timeinfo->tm_min==0 && timeinfo->tm_sec==0) {
        if (bright==1) bright=5;
        display->setPanelBrightness(bright);
    }
    
    delay(100);
}
