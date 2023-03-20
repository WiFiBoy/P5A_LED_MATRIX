//
// Color Icon Scrolling Example for LED Matrix P5A
//
// V1.0 (Feb 17, 2023) WiFiBoy Computing Lab
//

#include "foods.h"

#include "P5A-I2S-DMA.h"
P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

uint16_t ix;

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
  for(int i=0; i<3; i++) {
    int n= (ix/32+i) % 8;
    display->drawIcon(food+1024*n, i*32-(ix%32), 0, 32, 32);
  }
  ix++;
  delay(30);
}
