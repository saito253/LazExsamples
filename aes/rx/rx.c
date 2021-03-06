#include "rx_ide.h"		// Additional Header


/* FILE NAME: Read_SubGHz.c
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

#define SUBGHZ_CH	    36
#define SUBGHZ_PANID	0xABCD
unsigned char key[]       = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
uint8_t rx_data[256];
uint32_t last_recv_time = 0;
SUBGHZ_STATUS rx;

#define BLUE_LED	26

void setup(void)
{
	SUBGHZ_MSG msg;
	long myAddress;

	Serial.begin(115200);
	
	msg = SubGHz.init();
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	
    SubGHz.setKey(key);

	myAddress = SubGHz.getMyAddress();
	Serial.print("myAddress1 = ");
	Serial.println_long(myAddress,HEX);	
	msg = SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	msg = SubGHz.rxEnable(NULL);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
	
	pinMode(BLUE_LED,OUTPUT);
	digitalWrite(BLUE_LED,HIGH);
	
	Serial.println("TIME	HEADER	SEQ	PANID	RX_ADDR	TX_ADDR	RSSI	PAYLOAD");
	Serial.println("-----------------------------------------------------------------------------------------------------------------");
	
	return;
}

void loop(void)
{
	short rx_len;
	short index=0;
	uint16_t data16;
    uint32_t get_time;
    uint32_t delta_time;

	get_time = millis();
	rx_len = SubGHz.readData(rx_data,sizeof(rx_data));
    delta_time = millis() - get_time;
	
	if(rx_len>0)
	{
		Serial.print("Delta:");
        Serial.println_long(delta_time,DEC);
        
		digitalWrite(BLUE_LED, LOW);
		SubGHz.getStatus(NULL,&rx);
		// print time
		Serial.print_long((long)millis(),DEC);
		Serial.print("\t");
		
		
		// print header
		data16 = rx_data[index], index++;
		data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
		Serial.print_long((long)data16,HEX);
		Serial.print("\t");
		
		// print sequence number
		Serial.print_long((long)rx_data[index],HEX), index++;
		Serial.print("\t");
		
		// print PANID
		data16 = rx_data[index], index++;
		data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
		Serial.print_long((long)data16,HEX);
		Serial.print("\t");
		
		// print RX_ADDR
		data16 = rx_data[index], index++;
		data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
		Serial.print_long((long)data16,HEX);
		Serial.print("\t");
		
		// print TX_ADDR
		data16 = rx_data[index], index++;
		data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
		Serial.print_long((long)data16,HEX);
		Serial.print("\t");
		
		// print RSSI
		Serial.print_long((long)rx.rssi,DEC);
		Serial.print("\t");
		
		// print payload
		for(;index<rx_len;index++)
		{
			Serial.print_long(rx_data[index],HEX);
			Serial.print(" ");
		}
		
		// print ln
		Serial.println("");
		digitalWrite(BLUE_LED, HIGH);
	}
	
	
	return;
}

