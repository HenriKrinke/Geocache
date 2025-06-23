/*
 * levelTwo.c
 *
 *  Created on: 11 Jun 2025
 *      Author: henri
 */
#include "../../utils/colourSensor/color_classifier.h"
#include "../gpsGame/gpsGame.h"
#include "levelTwo.h"
#include "../../utils/flag.h"
#include "../../utils/gps/gps.h"
#include "../../utils/timer.h"
#include "../../utils/joystick/joystick.h"
#include <stdbool.h>
int color;
int currentMs;
int prevMs = 0;
int prevMs1 = 0;

int interval = 30000;
int redCnt = 0;
int blueCnt = 0;

void levelTwo()
{


if(getColor() != color)
{
color  = getColor();
switch(color)
{
case BLUE:
blueCnt++;
prevMs = *getMs();

//flag for an update LCD function or immediate print
if(blueCnt < 5)
{
setLCDFlag(1); //distance
}
else
{
	setLCDFlag(0); //clear
	}

break;
case RED:
redCnt++;
prevMs = *getMs();
//flag for an update LCD function or immediate print
if(redCnt < 5)
{
setLCDFlag(2); //degree
}
else
{
	setLCDFlag(0);
}


break;
case GREEN:
//flag for an update LCD function or immediate print

setLCDFlag(0); // clear

break;
}
}
else{

//add time logic here
currentMs = *getMs();

if((currentMs - prevMs) >= interval)
{

setLCDFlag(0);

}
if(joystick_sw())
{
	setSuccessFlag(true);

}
}
}

/*
if(distance(getPosition(),getTarget()) < 10){
	setSuccessFlag(true);
	}
*/


