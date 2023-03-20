
/*
 *  Tetris-like Game Example -- PixelBoy (OK:ESP32) version
 *  
 *  Oct 26, 2017 Created. (derek@wifiboy.org)
 *  Nov 20, 2020. PixelBoy port. (tklua@wifiboy.org)
 *
 */
#include <Preferences.h>

uint8_t blk_t[28][4] = { // seven shapes, four rotation types
  {0,10,20,30},{0,1,2,3},{0,10,20,30},{0,1,2,3},
  {1,10,11,12},{0,10,11,20},{0,1,2,11},{1,10,11,21},
  {0,1,10,11},{0,1,10,11},{0,1,10,11},{0,1,10,11},
  {0,10,20, 21},{0,1,2,10},{0,1,11,21},{2,10,11,12},
  {1,11,20,21},{0,10,11,12},{0,1,10,20},{0,1,2,12},
  {0,1,11,12},{1,10,11,20},{0,1,11,12},{1,10,11,20},
  {1,2,10,11},{0,10,11,21},{1,2,10,11},{0,10,11,21}
};

uint16_t bcolor[8]={
  //0, 0xC318, 0x1880, 0x3330, 0x030f, 0x0700, 0xC003, 0x0858
  0, 0xf800, 0xf8f0, 0xff80, 0x07e0, 0x001f, 0xf81f, 0x0707
};

Preferences preferences;
uint8_t board[20][10], offboard[20][10]; 
uint16_t cx, cy, kc, task_c, sfx_on, sfxn, sfxc, freq, fall_time, fall_limit, stage_limit,highscore; 
uint16_t ctype, nctype, level, pts, pn, i, j, k, pos, px, py, key, last_key, cline;
char tmpbuf[10];
int rot, ret;

void init_keys()
{
  pinMode(39,INPUT); pinMode(34,INPUT); pinMode(35,INPUT);

  kc = 0;
}

uint16_t getkey() // global key: 8bit key value: L R U D A B Sel Start 
{
  return(key = digitalRead(35)*128+digitalRead(39)*64+32+16
        +8+digitalRead(34)*4+2+1);
}

void draw_board()
{
  for(i=0; i<19; i++)
      for(j=0; j<10; j++)
          display->fillRect(i*3+6, 31-j*3-3, 3, 3, bcolor[board[i][j]]);

}

void clear_board()
{
  //display.fillScreen(wbRED);
  for(i=0; i<20; i++)     // board is 10x20
    for(j=0; j<10; j++) {
      offboard[i][j]=255; // initial state
      board[i][j]=0;      // clear all blocks
    }
  display->fillRect(0, 1, 63, 30, 0);
}

void get_pos(uint16_t pos)
{
  py=pos/10; px = pos%10;
}

void draw_blocks(uint16_t x, uint16_t y, uint16_t v)
{
  for(int i=0; i<4; i++) {
    get_pos(y*10 + x + blk_t[ctype*4+rot][i]); 
    board[py][px]=v;
  }
}

void draw_preview() // next blocks
{
  display->fillRect(0, 0, 5, 5, 0);
  for(i=0; i<4; i++) {
    get_pos(blk_t[nctype*4][i]);
    display->fillRect(px+1, py+1, 1, 1, bcolor[nctype+1]);
  }
}

void update_cline_level()
{
  if (cline > 150) level = 10;
  else if (cline > 120) level=9;
  else if (cline > 100) level=8;
  else if (cline > 80) level=7;
  else if (cline > 60) level=6;
  else if (cline > 40) level=5;
  else if (cline > 25) level=4;
  else if (cline > 15) level=3;
  else if (cline > 10) level=2;
  else if (cline > 5) level=1;
}

void update_score()
{
  update_cline_level();
  fall_limit = stage_limit = (level<=12?13-level:1);
}

int check_line()
{
  int pt;
  for(i=19; i>0; i--) {
    for(j=0, pt=0; j<10; j++) if (board[i][j]!=0) pt++; else break;
    if (pt==10) {
      for(j=i; j>0; j--)
        for(k=0; k<10; k++) board[j][k]=board[j-1][k];
      return 1;
    }
  }
  return 0;
}

void go_next_blocks()
{
  pn=0;
  while(check_line()) { // check all stacked line
    cline++;
    pts+=20*(2^pn);
    pn++;
    tone(25, 880, 20);tone(25, 1220, 20);tone(25, 1444, 20);
    delay(200);
  }
  //sfxn=pn+1; sfxc=0;
  ctype=nctype;
  nctype=random(7);
  pts+=10;
  cx = 4; cy = 0;
  rot=random(4);
  draw_blocks(cx, cy, ctype+1);
  draw_preview();
  update_score();
}

void init_blocks()
{
  clear_board();
  cline=0; level=0; pts=0;
  fall_limit = stage_limit = (level<=12?13-level:1);
  fall_time=0;
  cx=4; cy=0; 
  rot=random(4); ctype=random(7); nctype=random(7);
  draw_blocks(cx, cy, ctype+1);
  draw_preview();
  update_score();
}

void blocks_fall()
{
  if (++fall_time > fall_limit) {
    for(i=0; i<4; i++) {
      get_pos(pos=(cy+1)*10 + cx + blk_t[ctype*4+rot][i]);
      if (pos < 190) board[py-1][px]=0;
      else { // at bottom
        draw_blocks(cx, cy, ctype+1);
        go_next_blocks();
        tone(25, 1220, 30);
        return;
      }
    }
    cy++;
    for(i=0; i<4; i++) {
      get_pos(cy*10 + cx + blk_t[ctype*4+rot][i]);
      if (board[py][px]!=0) {
        tone(25, 880, 30);
        draw_blocks(cx, cy-1, ctype+1);
        if (cy==1) {
          // game over, and restart
          tone(25, 220, 120);tone(25, 110, 60);tone(25, 55, 60);
          tone(25, 110, 60);tone(25, 50, 260);
          /*wbpro_fillRect(17, 100, 127, 50, wbRED);
          wbpro_fillRect(22, 105, 117, 40, wbBLACK);
          wbpro_drawString("Game Over", 32, 117, 2, 2);
          */
          delay(2000); // wait 2 secs to prevent user's extra keys
          //wbpro_fillRect(16, 240, 128, 16, wbBLACK);
          //wbpro_drawString("Press a key to restart!", 21, 244, 2, 1);
          while(getkey()!=0xff); while(getkey()==0xff);
          while(getkey()!=0xff); 
          sfxn=7; sfxc=0;
          //wbpro_fillRect(20, 100, 130, 50, wbBLACK);
          init_blocks();
          return;
        }
        go_next_blocks();
        return;
      }
    }
    draw_blocks(cx, cy, ctype+1);
    fall_time=0;
  }
}

void check_left()
{
  tone(25, 110, 60);
  for(i=0; i<4; i++) {
    get_pos(cy*10 + cx-1 + blk_t[ctype*4+rot][i]);
    if (px>cx+4) return; //left bound
  }
  draw_blocks(cx, cy, 0);
  for(i=0; i<4; i++) {
    get_pos(cy*10 + cx-1 + blk_t[ctype*4+rot][i]);
    if (board[py][px]!=0) {
      draw_blocks(cx, cy, ctype+1);
      return;
    }
  }
  cx--;
  draw_blocks(cx, cy, ctype+1);
}

void check_right()
{
  tone(25, 110, 60);
  for(i=0; i<4; i++) {
    get_pos(cy*10 + cx+1 + blk_t[ctype*4+rot][i]);
    if (px>cx-1) {
      board[py][px-1]=0;
    } else {
      draw_blocks(cx, cy, ctype+1);
      return;
    }
  }
  for(i=0; i<4; i++) {
    get_pos(cy*10 + cx+1 + blk_t[ctype*4+rot][i]);
    if (board[py][px]!=0) {
      draw_blocks(cx, cy, ctype+1);
      return;
    }
  }
  cx++;
  draw_blocks(cx, cy, ctype+1);
}

int check_rotate()
{
  tone(25, 110, 60);
  for(i=0; i<4; i++) {
    get_pos(cy*10 + cx + blk_t[ctype*4+rot][i]);
    if (pos<190) {
      if (board[py][px]!=0) return -2;
      if (px < cx) {
        cx--; return -1;
      }
      if (px > cx+4) {
        cx++; return -1;
      }
    }
  }
  return 0;
}

uint16_t sfx[8][8]={
  {0},
  {262, 0, 0, 0, 0, 0, 0, 0},
  {262, 330, 0, 0, 0, 0, 0, 0},
  {262, 330, 392, 0, 0, 0, 0, 0},
  {262, 330, 392, 523, 660, 0, 0, 0},
  {262, 330, 392, 523, 660, 784, 1047, 0},
  {262, 392, 330, 262, 16, 165, 131, 0},
  {1047,0,0,0,0,0,0,0}
};

void sfx_engine() // multi-tasking sound effect engine
{
  if (sfx_on) {
    if ((++task_c%3)==0) {
      if (sfxn!=0) {
        freq=sfx[sfxn][sfxc];
        if (freq) {
          ledcSetup(1, freq, 8);
          ledcWrite(1, 30);
        } else ledcWrite(1, 0);
        sfxc++;
        if (sfxc>7) sfxn=0;
      } 
    }
  }
}

/*
hw_timer_t * _timer = NULL;  // the esp32 timer
void ticker_setup() // multitasking ticker engine
{
  wbpro_tickerInit(30000, sfx_engine);
  ledcAttachPin(25, 1);
  ledcSetup(1, 200, 8);
  sfx_on=1;
  sfxn=0;
  task_c=0;
}
*/

void init_preferences()
{
  preferences.begin("tetris",false);
  highscore=preferences.getUInt("highscore",1);
  if (highscore==1) { // first run
    preferences.putUInt("highscore", 0);
    highscore=0;
  }
  if ((getkey()&0x90)==0) preferences.putUInt("highscore", 0);
  preferences.end();
}

void init_game()
{
  init_keys();
  /*
  wbpro_drawFastHLine(0,319,160, wbRED);
  wbpro_drawFastVLine(0,0,320, wbRED);
  wbpro_drawFastVLine(160,0,320, wbRED);
  */
  init_blocks();
  /*
  wbpro_setTextColor(wbYELLOW, wbYELLOW);
  wbpro_drawString("Best", 170, 95, 2, 2);
  wbpro_drawString("Score", 170, 150, 2, 2);
  wbpro_drawString("Lines", 170, 205, 2, 2);
  wbpro_drawString("Level", 170, 260, 2, 2);
  wbpro_drawString("Boytris", 165, 2, 2, 2);
  wbpro_setTextColor(wbGREEN, wbGREEN);
  wbpro_drawString("(C) 2017 WiFiBoy.Org", 165, 310, 1, 1);
  wbpro_drawString(itoa(highscore, tmpbuf, 10), 175, 120, 1, 2);
  */
}

void check_downkey()
{
  if ((((key&0x10)==0)&&last_key==0)) { // down
    fall_limit=1; 
    last_key=5;
  } else if (last_key==5) { // down release
    fall_limit=stage_limit; 
    last_key=0;
  }
}

void check_leftkey()
{
  if (((key&0x80)==0)) { // repeatable key
    last_key=1;
    if (kc++%6==0) check_left(); // move delay = 6
    fall_limit=stage_limit;
  } else if (((key&0x80)==0x80)&&(last_key==1)) { // left release
    last_key=0; 
    kc=0;
  }
}

void check_rightkey()
{
  if (((key&0x40)==0)) { // repeatable key
    last_key=2;
    if (kc++%6==0) check_right(); // move delay = 6
    fall_limit=stage_limit; 
  } else if (((key&0x40)==0x40)&&(last_key==2)) { // left release
    last_key=0; 
    kc=0;
  }
}

void check_selectkey()
{
  if (((key&0x2)==0)&&(last_key==0)) { // repeatable key
    last_key=6;
    if (sfx_on==0) { 
      //wbpro_fillRect(230, 290, 8, 8, wbBLACK); 
      sfxn=0; 
    } else {
      //wbpro_setTextColor(wbRED, wbRED);
      //wbpro_drawString("X", 230, 290, 1, 1);
    }
    sfx_on = 1 - sfx_on;
  } else if (((key&0x2)==0x2)&&(last_key==6)) { // left release
    last_key=0; 
  }
}

void check_rotrkey()
{
  if (((key&0x04)==0)){ 
    last_key=3;
    if (kc++ % 15 == 0) { // rotate delay = 15
      draw_blocks(cx, cy, 0); // clean current blocks
      if (++rot>3) rot=0;
      while(1) { // dangerous!
        ret = check_rotate();
        if (ret==-2) {
          if (++rot>3) rot=0;
        } else if (ret==0) break; // pick next legal rotation
      }
      draw_blocks(cx, cy, ctype+1); // draw rotated blocks
    }
  } else if (((key&0x04)==0x04)&&(last_key==3)) {
    last_key=0;
    kc=0;
  }
}

void check_rotlkey()
{ 
  if (((key&0x08)==0)) { 
    last_key=4;
    if (kc++ % 15 == 0) { // rotate delay = 15
      draw_blocks(cx, cy, 0); 
      if (--rot<0) rot=3;
      while(1) { // dangerous!
        ret = check_rotate();
        if (ret==-2) {
          if (--rot<0) rot=3;
        } else if (ret==0) break; 
      }
      draw_blocks(cx, cy, ctype+1);
    }
  } else if (((key&0x08)==0x08)&&(last_key==4)) {
    last_key=0;
    kc=0;
  }
}
