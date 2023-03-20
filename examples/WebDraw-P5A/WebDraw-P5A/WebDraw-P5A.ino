//
// Web Drawing Example for LED Matrix P5A
//
// mod from https://github.com/witnessmenow/ESP8266-Led-Matrix-Web-Draw
//
// V1.0 (Jan 17, 2023) WiFiBoy Computing Lab
//

// 編譯前，請先修改 AP, PWD 為您使用的 WiFi 無線分享器!!
char AP[]="lailab";
char PWD[]="26333256!";

#include "P5A-I2S-DMA.h"
#include <WiFi.h>
#include <WebSocketsServer.h>

P5A_MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(64, 32, 1);
WebSocketsServer webSocket = WebSocketsServer(81);

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) 
{
  	const uint8_t* src = (const uint8_t*) mem;
  	Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  	for(uint32_t i = 0; i < len; i++) {
    		if(i % cols == 0) {
    			  Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    		}
    		Serial.printf("%02X ", *src);
    		src++;
  	}
  	Serial.printf("\n");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) 
{
    int x, y, h, w;
    uint16_t colour;
    int commas[] = {-1,-1,-1,-1};
    int command;
    String cmd, colourString;
    
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                display->fillScreen(0);
        				// send message to client
    		        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            cmd = String((char *) payload);
            Serial.printf("[%u] get Text: %s\n", num, payload);
            if (cmd=="CLEAR") {
                display->fillScreen(0);
                break;
            }
            for(int i=0; i<4; i++) commas[i]=-1;
            int commaIndex=0;
            for(int i=0; i<cmd.length(); i++)  
                if (cmd[i] ==',') commas[commaIndex++]=i;
            int commandSeperator = cmd.indexOf(":");
            command = cmd.substring(0,commandSeperator).toInt();
            Serial.println(command);
            x = cmd.substring(commandSeperator+1, commas[0]).toInt();
            y = cmd.substring(commas[0] + 1, commas[1]).toInt();
            Serial.print(x);
            Serial.print(",");
            Serial.println(y);
            if (command==0) { // Draw Pixel
                colourString = cmd.substring(commas[1] + 1);
                Serial.println(colourString);
                colour = strtol(colourString.c_str(), NULL, 0);
                Serial.println(colour);
                display->drawPixel(x , y, colour);
            }
            break;
    }
}

void setup() 
{
    Serial.begin(115200); // 啟用序列埠訊息

    display = new P5A_MatrixPanel_I2S_DMA(mxconfig);
    display->begin(); 
    display->setPanelBrightness(8);
    display->setCursor(0,0);
    display->print("Connecting..");

    WiFi.begin(AP, PWD); // 連接用戶的WiFi分享器    
    
    while(WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    display->fillScreen(0);
    display->setCursor(0,0);
    display->print("Connected:");
    display->println(WiFi.localIP());

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() 
{
    webSocket.loop();
}
