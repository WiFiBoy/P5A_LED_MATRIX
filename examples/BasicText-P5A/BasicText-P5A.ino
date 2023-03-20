//
// Basic Text Example for LED Matrix P5A
//
// V1.0 (Jan 17, 2023) WiFiBoy Computing Lab
//

#include "P5A-I2S-DMA.h"

P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

void show_str(char *s, int16_t x, int16_t y, uint16_t color)
{
    display->setTextColor(color);
    display->setCursor(x,y);
    display->print(s);
}

void setup() 
{
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(10);
    
    show_str("Hello!", 14, 6, 0xFFFF);
    show_str("P", 10, 16, 0x07F0);
    show_str("5", 16, 16, 0xFF00);
    show_str("A", 22, 16, 0xF800);
    show_str("LED!", 32, 16, 0x07FF);

    display->drawRect(0,0,64,32,0xF80F);
    display->drawRect(1,1,62,30,0x780F);
}

void loop() 
{
  display->drawRect(1,1,62,30,rand());
  delay(300);
}
