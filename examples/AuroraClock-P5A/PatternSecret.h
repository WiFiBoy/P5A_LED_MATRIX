/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from Andrew: http://pastebin.com/f22bfe94d
 * which, in turn, was "Adapted from the Life example on the Processing.org site"
 *
 * Made much more colorful by J.B. Langston: https://github.com/jblang/aurora/commit/6db5a884e3df5d686445c4f6b669f1668841929b
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PatternSecret_H
#define PatternSecret_H

class secretCell {
public:
  uint16_t color;
};

uint16_t secrets[280]=
{0x822c,0x82e5,0x6ce2,0x7f85,0x871c,0x591a,0x5fc3,0x7d93,0x5357,0x7121,0x6c99,0x66fc,0x5854,0x5e03,0x9054,0x5357,0x54c7,0x5409,0x62c9,0x54a9
,0x89c0,0x81ea,0x5728,0x83e9,0x85a9,0x884c,0x6df1,0x822c,0x82e5,0x6ce2,0x7f85,0x871c,0x591a,0x6642,0x7167,0x898b,0x4e94,0x860a,0x7686,0x7a7a
,0x5ea6,0x4e00,0x5207,0x82e6,0x5384,0x820d,0x5229,0x5b50,0x8272,0x4e0d,0x7570,0x7a7a,0x7a7a,0x4e0d,0x7570,0x8272,0x8272,0x5373,0x662f,0x7a7a
,0x7a7a,0x5373,0x662f,0x8272,0x53d7,0x60f3,0x884c,0x8b58,0x4ea6,0x5fa9,0x5982,0x662f,0x820d,0x5229,0x5b50,0x662f,0x8af8,0x6cd5,0x7a7a,0x76f8
,0x4e0d,0x751f,0x4e0d,0x6ec5,0x4e0d,0x57a2,0x4e0d,0x6de8,0x4e0d,0x589e,0x4e0d,0x6e1b,0x662f,0x6545,0x7a7a,0x4e2d,0x7121,0x8272,0x7121,0x53d7
,0x60f3,0x884c,0x8b58,0x7121,0x773c,0x8033,0x9f3b,0x820c,0x8eab,0x610f,0x7121,0x8272,0x8072,0x9999,0x5473,0x89f8,0x6cd5,0x7121,0x773c,0x754c
,0x4e43,0x81f3,0x7121,0x610f,0x8b58,0x754c,0x7121,0x7121,0x660e,0x4ea6,0x7121,0x7121,0x660e,0x76e1,0x4e43,0x81f3,0x7121,0x8001,0x6b7b,0x4ea6
,0x7121,0x8001,0x6b7b,0x76e1,0x7121,0x82e6,0x96c6,0x6ec5,0x9053,0x7121,0x667a,0x4ea6,0x7121,0x5f97,0x4ee5,0x7121,0x6240,0x5f97,0x6545,0x83e9
,0x63d0,0x85a9,0x57f5,0x4f9d,0x822c,0x82e5,0x6ce2,0x7f85,0x871c,0x591a,0x6545,0x5fc3,0x7121,0x7f63,0x7919,0x7121,0x7f63,0x7919,0x6545,0x7121
,0x6709,0x6050,0x6016,0x9060,0x96e2,0x985b,0x5012,0x5922,0x60f3,0x7a76,0x7adf,0x6d85,0x69c3,0x4e09,0x4e16,0x8af8,0x4f5b,0x4f9d,0x822c,0x82e5
,0x6ce2,0x7f85,0x871c,0x591a,0x6545,0x5f97,0x963f,0x8028,0x591a,0x7f85,0x4e09,0x85d0,0x4e09,0x83e9,0x63d0,0x6545,0x77e5,0x822c,0x82e5,0x6ce2
,0x7f85,0x871c,0x591a,0x662f,0x5927,0x795e,0x5492,0x662f,0x5927,0x660e,0x5492,0x662f,0x7121,0x4e0a,0x5492,0x662f,0x7121,0x7b49,0x7b49,0x5492
,0x80fd,0x9664,0x4e00,0x5207,0x82e6,0x771f,0x5be6,0x4e0d,0x865b,0x6545,0x8aaa,0x822c,0x82e5,0x6ce2,0x7f85,0x871c,0x591a,0x5492,0x5373,0x8aaa
,0x5492,0x66f0,0x63ed,0x8ae6,0x63ed,0x8ae6,0x6ce2,0x7f85,0x63ed,0x8ae6,0x6ce2,0x7f85,0x50e7,0x63ed,0x8ae6,0x83e9,0x63d0,0x85a9,0x5a46,0x8a36};



class PatternSecret : public Drawable {
private:
    secretCell world[MATRIX_WIDTH][MATRIX_HEIGHT];
    unsigned int density = 50;
    int generation = 0;

    void randomFillWorld() {
        for (int i = 0; i < MATRIX_WIDTH; i++) 
          for(int j = 0; j< MATRIX_HEIGHT; j++)
            world[i][j].color = 0x8888;
            
        for (int i = 0; i < 280; i++) {
            world[(i%20)*3+2][(i/20)*2+2].color = secrets[i];
            world[(i%20)*3+3][(i/20)*2+2].color = secrets[i];
            world[(i%20)*3+4][(i/20)*2+2].color = secrets[i];
            world[(i%20)*3+2][(i/20)*2+3].color = secrets[i];
            world[(i%20)*3+3][(i/20)*2+3].color = secrets[i];
            world[(i%20)*3+4][(i/20)*2+3].color = secrets[i];
        }
    }

public:
    PatternSecret() {
        name = (char *)"Secrets";
    }

    unsigned int drawFrame() {
        if (generation == 0) {
            effects.ClearFrame();
            randomFillWorld();
        }
  
        // Display current generation
        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                effects.leds[XY(i, j)] = effects.ColorFromCurrentPalette((world[i][j].color+generation)%256,100); // hue, brightness
            }
        }
        
        generation++;
        effects.ShowFrame();
  
        return 30;
    }
};

#endif
