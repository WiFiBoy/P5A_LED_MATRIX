//
// Chinese Big5 Font Example for LED Matrix P5A
//
// V1.0 (Jan 17, 2023) WiFiBoy Computing Lab
//

#include "P5A-I2S-DMA.h"

P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

#include "b5f16.h"

uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

uint32_t b5offset(uint16_t ch)
{
    uint32_t x1, x2, n;
    x1=ch>>8; x2=ch&0xff;
    n=(x1-161)*5024+362;
    if (x2<127) n+=(x2-64)*32;
    else n+=(x2-98)*32;
    return(n);
}

void showch(uint16_t b5, int x, int y, int c) 
{
    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++)
            if (b5f16[b5offset(b5)+2*i] & bits[j]) display->drawPixel(x+j, y+i, c); 
            else display->drawPixel(x+j, y+i, 0);
        for(int j=0; j<8; j++) 
            if (b5f16[b5offset(b5)+2*i+1] & bits[j]) display->drawPixel(x+j+8, y+i, c);
            else display->drawPixel(x+j+8, y+i, 0);
    }
}

void showfont16(int f, int x, int y, int c)
{
    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++)
            if (b5f16[f*32+2*i+40] & bits[j]) display->drawPixel(x+j, y+i, c); 
            else display->drawPixel(x+j, y+i, 0);
        for(int j=0; j<8; j++) 
            if (b5f16[f*32+2*i+1+40] & bits[j]) display->drawPixel(x+j+8, y+i, c);
            else display->drawPixel(x+j+8, y+i, 0);
    }
}

void setup() 
{
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(10);
    
    for(int i=64; i>-128; i--) {
        display->fillScreen(0);
        showch(0xACAF, i+0, 0, rand()%65536); 
        showch(0xA44F, i+16, 0, rand()%65536); 
        showch(0xBF4F, i+32, 0, rand()%65536); 
        showch(0xAA4F, i+48, 0, rand()%65536); 
        showch(0xA672, i+64, 0, rand()%65536); 
        showch(0xABAC, i+80, 0, rand()%65536); 
        showch(0xB4FA, i+96, 0, rand()%65536); 
        showch(0xB8D5, i+112, 0, rand()%65536);

        showch(0xACAF, i*2+0, 16, rand()%65536); 
        showch(0xA44F, i*2+16, 16, rand()%65536); 
        showch(0xBF4F, i*2+32, 16, rand()%65536); 
        showch(0xAA4F, i*2+48, 16, rand()%65536); 
        showch(0xA672, i*2+64, 16, rand()%65536); 
        showch(0xABAC, i*2+80, 16, rand()%65536); 
        showch(0xB4FA, i*2+96, 16, rand()%65536); 
        showch(0xB8D5, i*2+112, 16, rand()%65536);
        
        delay(80);
    }
}

void loop() 
{
  static int fn=40;

  showfont16(fn++, (rand()%4)*16, (rand()%2)*16, (rand()%65536)); 
  if (fn>13000) fn=40;
  
  delay(50); 
}
