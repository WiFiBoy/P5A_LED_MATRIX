//
// Chinese String Scrolling Example for LED Matrix P5A
//
// V1.0 (Feb 17, 2023) WiFiBoy Computing Lab
//

#include "P5A-I2S-DMA.h"
P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

#include "b5f16.h"

uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

uint16_t goodstr[]={ 0xa140,
  0xB752,0xAC4F,0xABED,0xA45B,0xA7D4,0xAD40,0xA141,0xA453,0xA6B3,0xAEA6,0xB74F,0xA141,0xB752,
  0xAC4F,0xA4A3,0xB6FA,0xA7AA,0xA141,0xB752,0xAC4F,0xA4A3,0xA6DB,0xB8D8,0xA141,0xA4A3,0xB169,
  0xA867,0xA141,0xA4A3,0xB0B5,0xAE60,0xB2DB,0xAABA,0xA8C6,0xA141,0xA4A3,0xA844,0xA6DB,0xA476,
  0xAABA,0xAF71,0xB342,0xA141,0xA4A3,0xBBB4,0xA9F6,0xB56F,0xABE3,0xA141,0xA4A3,0xAD70,0xBAE2,
  0xA448,0xAE61,0xAABA,0xB463,0xA141,0xA4A3,0xB3DF,0xC577,0xA4A3,0xB871,0xA575,0xB3DF,0xC577,
  0xAF75,0xB27A,0xA141,0xA45A,0xA8C6,0xA55D,0xAE65,0xA141,0xA45A,0xA8C6,0xACDB,0xAB48,0xA141,
  0xA45A,0xA8C6,0xACDF,0xB1E6,0xA141,0xA45A,0xA8C6,0xA7D4,0xAD40,0xA141,0xA45A,0xA8C6,0xAD6E,
  0xA7D4,0xAD40,0xA141,0xB752,0xAC4F,0xA5C3,0xA4A3,0xA4EE,0xAEA7,0xA143
};

uint32_t b5offset16(uint16_t ch)
{
    uint32_t x1, x2, n;
    x1=ch>>8; x2=ch&0xff;
    n=(x1-161)*5024+362;
    if (x2<127) n+=(x2-64)*32;
    else n+=(x2-98)*32;
    return(n);
}

void showB5_16(uint16_t b5, int x, int y, int c) 
{
    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++)
            if (b5f16[b5offset16(b5)+2*i] & bits[j]) display->drawPixel(x+j, y+i, c); 
            else display->drawPixel(x+j, y+i, 0);
        for(int j=0; j<8; j++) 
            if (b5f16[b5offset16(b5)+2*i+1] & bits[j]) display->drawPixel(x+j+8, y+i, c);
            else display->drawPixel(x+j+8, y+i, 0);
    }
}

void showFont_16(int f, int x, int y, int c)
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

uint16_t ix=0;

void setup() 
{
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(10);
    display->fillScreen(0);
    ix=0;
}

void loop() 
{
    display->drawRect(0,0,64,2,0x8008);
    display->drawRect(0,30,64,2,0x0aaa);
    
    for(int i=0;i<5;i++) {
        int n=ix/16+i;
        if (n<102) showB5_16(goodstr[n], i*16-(ix%16), 8, goodstr[n] | 0x8410);
    }
    ix++;
    if (ix>1616) ix=0;
    
    delay(50);
}
