#include "tx_ide.h"		// Additional Header

/* FILE NAME: Welcome_SubGHz.c
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015  Lapis Semiconductor Co.,Ltd.
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/


#define LED 26						// pin number of Blue LED
#define SUBGHZ_CH		36			// chael number (frequency)
#define SUBGHZ_PANID	0xabcd		// panid
#define HOST_ADDRESS	0xac54		// distination address

unsigned char send_data[] = {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazurite"};
unsigned char key[]       = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
unsigned char flg;


void setup(void)
{
	
	SubGHz.init();					// initializing Sub-GHz
    SubGHz.setKey(key);
	Serial.begin(115200);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
}

void loop(void)
{
	SUBGHZ_MSG msg;
	// Initializing
    uint32_t get_time;
    uint32_t delta_time;

    flg= flg^1;
    flg=1;

    if (flg) {
        SubGHz.setKey(key);
    }else{
        SubGHz.setKey(NULL);
    }

	SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);		// start Sub-GHz

	// unicast preparing data
	digitalWrite(LED,LOW);														// LED ON
    get_time = millis();
    msg=SubGHz.send(SUBGHZ_PANID, HOST_ADDRESS, &send_data, sizeof(send_data),NULL);// send data
    delta_time = millis() - get_time;
	digitalWrite(LED,HIGH);														// LED off
	Serial.print("AES ON Delta:");
    Serial.println_long(delta_time,DEC);
	SubGHz.msgOut(msg);
	
	sleep(1000);																// sleep

	// braodcast preparing data
	digitalWrite(LED,LOW);														// LED ON
    get_time = millis();
    msg=SubGHz.send(0xffff, 0xffff, &send_data, sizeof(send_data),NULL);// send data
    delta_time = millis() - get_time;
	digitalWrite(LED,HIGH);														// LED off
	Serial.print("AES OFF Delta:");
    Serial.println_long(delta_time,DEC);
	SubGHz.msgOut(msg);

	// close
	SubGHz.close();																// Sub-GHz module sets into power down mode.
	
	sleep(1000);																// sleep

	return;
}

