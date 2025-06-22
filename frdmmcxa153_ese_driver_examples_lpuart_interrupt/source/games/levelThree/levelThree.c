/*
 * levelThree.c
 *
 *  Created on: 19 Jun 2025
 *      Author: henri
 */


#include "levelThree.h"
#include "../../utils/comProtocols/GPIO/gpio_input.h"
#include "../../utils/joystick/joystick.h"

#include "../gpsGame/gpsGame.h"
#include "../../utils/timer.h"

#include "../../utils/flag.h"
#include <stdbool.h>
int cnt1 = 0;
void levelThree()
{


if(checkPin(3,11))
{
    GPIO3->PCOR |=  (1<<12);
    cnt1++;
	setLCDFlag(3);
}
else
{
    GPIO3->PSOR |=  (1<<12);

	setLCDFlag(5);
}
if(cnt1 == 100)
{
setSuccessFlag(true);
}
if(*getDistance() < 10){
	//setSuccessFlag(true);
	}
}
