//
// Chinese 12x12 Font Demo Example for LED Matrix P5A
//
// V1.0 (Jan 17, 2023) WiFiBoy Computing Lab
//

#include "P5A-I2S-DMA.h"

P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

#include "b5f12.h"

uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
uint8_t fbuf[18];
uint16_t _cx, _cy, _color1;

void showbits(uint8_t c, uint8_t n)
{
    for(int i=0; i<4; i++) {
        if (c & bits[i+n]) display->drawPixel(_cx, _cy, _color1);
        else display->drawPixel(_cx, _cy, 0);
        _cx+=1;
    }
}

void dispfont(uint16_t n)
{
     memcpy_P(fbuf, (uint8_t *)(&b5f12[n*18]), 18);
     int count = 0;
     for(int i=0; i<18; i++) {
         showbits(fbuf[i], 0);
         if (++count % 3 == 0) { _cx-=12; _cy+=1; } 
         showbits(fbuf[i], 4);
         if (++count % 3 == 0) { _cx-=12; _cy+=1; } 
     }
}

void setup() 
{
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(10);
    display->fillScreen(0);
}

void loop() 
{
  _cx=(rand()%5)*13;
  _cy=(rand()%2)*14+3;
  _color1=(rand()%65536);
  dispfont(rand()%5401);
  delay(20);
}
