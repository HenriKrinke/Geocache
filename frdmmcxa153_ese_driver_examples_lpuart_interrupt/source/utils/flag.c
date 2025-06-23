/*
 * flag.c
 *
 *  Created on: 13 Jun 2025
 *      Author: henri
 */
#include "flag.h"
#include <stdbool.h>
#include "lcdScreen/lcd.h"
#include "gps/gps.h"
#include "../games/gpsGame/gpsGame.h"
#include "comProtocols/Lpuart/lpuart2_interrupt.h"
#include "../games/levelFour/levelFour.h"
#include "colourSensor/color_classifier.h"
#include <stdlib.h>
#include <stdio.h>
int levelFlag = 0;
int LCDflag = 0;
bool successFlag = false;
void setLevelFlag(int a)
{
	levelFlag = a;
}
void setLCDFlag(int flag)
{
	if(flag != LCDflag)
	{
	lcd_clear();
	LCDflag = flag;
	}
}
int* getLCDFlag()
{return &LCDflag;}
void setSuccessFlag(bool flag)
{successFlag = flag;}
bool* getSuccessFlag()
{return &successFlag;}


void LCDupdate()
{
	char word[16];  // buffer large enough for one LCD row
	    int degree;
	    int dis;
	    int* pw;
	    char pwChar[2];
	    switch(LCDflag)
	    {
	        case 0:
	            lcd_set_cursor(0, 0);
	            sprintf(word, "%-16s", "");
	            lcd_send_string(word);
	            break;

	        case 1:  // Show distance
	            dis = (int)(distance(getPosition(),getTarget()));
	            //printf("casted:%d",distance);
	            sprintf(word, "%-10d", dis);  // left-align in 4 spaces
	            lcd_set_cursor(1, 0);
	            lcd_send_string("meters");
	            lcd_set_cursor(0, 0);
	            lcd_send_string(word);
	            pw = getPw();
	            char pwChar[2];
	            pwChar[0] = '0' + pw[levelFlag];
	            pwChar[1] = '\0';
	            lcd_set_cursor(0, 16);
	            lcd_send_string(pwChar);

	            break;

	        case 2:  // Show bearing (numeric)
	            degree = (int)(calculateBearing(getPosition(),getTarget()));
	            sprintf(word, "%-4d", degree);  // left-align in 4 spaces
	            lcd_set_cursor(0, 0);
	            lcd_send_string(word);
	            lcd_set_cursor(1, 0);
	            lcd_send_string("degrees");
	            pw = getPw();
	            pwChar[2];
	            pwChar[0] = '0' + pw[levelFlag];
	            pwChar[1] = '\0';
	            lcd_set_cursor(0, 16);
	            lcd_send_string(pwChar);
	            break;

	        case 3:  // Show compass direction
	            degree = (int)(calculateBearing(getPosition(),getTarget()));
	            const char* direction;

	            if (degree >= 337 || degree < 22)
	                direction = "N";
	            else if (degree < 67)
	                direction = "NE";
	            else if (degree < 112)
	                direction = "E";
	            else if (degree < 157)
	                direction = "SE";
	            else if (degree < 202)
	                direction = "S";
	            else if (degree < 247)
	                direction = "SW";
	            else if (degree < 292)
	                direction = "W";
	            else
	                direction = "NW";

	            sprintf(word, "%-4s", direction);  // pad with spaces to 4 characters
	            lcd_set_cursor(0, 0);
	            lcd_send_string(word);
	            lcd_set_cursor(0, 16);
	            pw = getPw();
	            pwChar[2];
	            pwChar[0] = '0' + pw[levelFlag];
	            pwChar[1] = '\0';
	            lcd_set_cursor(0, 16);
	            lcd_send_string(pwChar);
	            break;

	        case 4:
	            lcd_set_cursor(0, 0);
	            sprintf(word, "%-16s", "Wait for fix...");
	            lcd_send_string(word);
	            break;
	        case 5:
	        	lcd_set_cursor(0, 0);
	        	sprintf(word, "%-16s", "Box is tilted!!!!");
	        	lcd_send_string(word);
	        	break;
	        case 6:
	        	lcd_set_cursor(0, 0);
	        	sprintf(word, "%-4s", getBuf());
	        	lcd_send_string(word);
	            lcd_set_cursor(0, 16);
	            pw = getPw();
	            pwChar[2];
	            pwChar[0] = '0' + pw[levelFlag];
	            pwChar[1] = '\0';
	            lcd_set_cursor(0, 16);
	            lcd_send_string(pwChar);



	    }


}
