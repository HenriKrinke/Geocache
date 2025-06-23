/*
 * levelFour.c
 *
 *  Created on: 22 Jun 2025
 *      Author: henri
 */
#include "levelFour.h"
#include "../../utils/joystick/joystick.h"
#include "../../utils/gps/gps.h"
#include "../../utils/flag.h"
#include "../../utils/timer.h"

#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include <stdbool.h>
typedef enum { UP, DOWN, LEFT, RIGHT, NONE } Direction;
 //defined as 1500
bool inputDetected = false;
Direction move = NONE;
bool stickNeutral = true;
int pw[4];
char buf[5];
int passwordCnt;
Direction getJoystickDir(void);
void levelFour()
{
	setLCDFlag(6);
	int waitTime = 1500;
	while (waitTime > 0) {
	            updatePosition();  //to ensure gps data gets update despite the wondrful while loops

	        	//updatePosition here
	            Direction dir = getJoystickDir();

	            if (dir == NONE) {
	                stickNeutral = true;
	            } else if (stickNeutral) {
	                move = dir;
	                inputDetected = true;
	                stickNeutral = false;
	                break;
	            }

	            delay_ms(10);
	            waitTime -= 10;
	        }
if(move == pw[passwordCnt])
{
	buf[passwordCnt] = '0' + move;
	passwordCnt++;
	if(passwordCnt > 3)
	{
		buf[passwordCnt] = '\0';
		setSuccessFlag(true);}

}

}

Direction getJoystickDir() {
    int x = joystick_vrx();
    int y = joystick_vry();

    if (x > 60000 && y < 55000) return RIGHT;
    if (x < 5000  && y < 55000) return LEFT;
    if (y < 1000  && x < 55000) return UP;
    if (y > 60000 && x < 55000) return DOWN;

    return NONE;
}

void genPw()
{
	srand(joystick_vrx());
	for (int i = 0; i < 4; i++) {
	        pw[i] = rand() % 4;  // %5 gives values from 0 to 4
	    }
	for (int i = 0; i < 4; i++) {
	        printf("%d", pw[i]);
	    }
	    printf("\n");
}
char* getBuf()
{
	return buf;
}
int* getPw()
{
return pw;
}
