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
#include "../../utils/timer.h"
#include "../../utils/joystick/joystick.h"
#include <stdbool.h>
int color;
int currentMs;
int prevMs = 0;
int prevMs1 = 0;

int interval = 10000;
int cnt = 0;
void levelTwo()
{


if(getColor() != color)
{
cnt++;
color  = getColor();
switch(color)
{
case BLUE:
prevMs = *getMs();

//flag for an update LCD function or immediate print
setLCDFlag(1);

break;
case RED:
prevMs = *getMs();
//flag for an update LCD function or immediate print
setLCDFlag(2);


break;
case GREEN:
//flag for an update LCD function or immediate print
prevMs = *getMs();
setLCDFlag(3);

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
if(*getDistance() < 10){
	setSuccessFlag(true);
	}
*/


