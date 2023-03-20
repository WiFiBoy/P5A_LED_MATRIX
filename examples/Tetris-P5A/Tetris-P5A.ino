//
// Tetris Game Example for WiFiBoy LED Matrix P5A
//
// V1.0 (Jan 17, 2023) WiFiBoy Computing Lab
//

#include "P5A-I2S-DMA.h"

P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);

#include "wb_tetris.h"

uint8_t font_num[50]={
  0x0f,0x09,0x09,0x09,0x0f, 0x06,0x02,0x02,0x02,0x07, 0x0f,0x01,0x0f,0x08,0x0F, 0x0f,0x01,0x0f,0x01,0x0f,
  0x09,0x09,0x0f,0x01,0x01, 0x0F,0x08,0x0f,0x01,0x0f, 0x0f,0x08,0x0f,0x09,0x0f, 0x0F,0x01,0x01,0x01,0x01,
  0x0f,0x09,0x0f,0x09,0x0f, 0x0f,0x09,0x0f,0x01,0x0f
};
uint8_t bits[8]={128,64,32,16,8,4,2,1};

void show_digit(int digit, int x, int y, int color)
{
  for(int i=0; i<5; i++)
    for(int j=0; j<4; j++) 
      if (font_num[digit*5+i]&bits[4+j]) display->drawPixel(y+i, (31-x-j), color);
      else display->drawPixel(y+i, (31-x-j), 0);
}

void show_num(int num, int x, int y, int color)
{
  int n=num;
  for(int i=0; i<5; i++) {
    show_digit(n%10, x+(4-i)*5, y, color);
    n/=10;
  }
}

void setup() 
{
    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(8);

    init_game();
}

void loop() 
{
    getkey();
    check_leftkey();
    check_rightkey();
    check_rotrkey();
    draw_board();
    blocks_fall();
    draw_preview();
    display->drawLine(6,0,63,0,0x7000);
    display->drawLine(6,31,63,31,0x0070);
    display->drawLine(63,0,63,31,0x0300);
    show_num(pts, 0, 0, 0xff30);
    show_digit(level, 0, 0, 0xf0c0);
    delay(20); // ~50fps
}
