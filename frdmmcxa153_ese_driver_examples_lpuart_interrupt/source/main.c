/*! ***************************************************************************
 *
 * \brief     Main application
 * \file      main.c
 * \author    Hugo Arends
 * \date      February 2024
 *
 * \see       NXP. (2024). MCX A153, A152, A143, A142 Reference Manual. Rev. 4,
 *            01/2024. From:
 *            https://www.nxp.com/docs/en/reference-manual/MCXAP64M96FS3RM.pdf
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
/*
#include <MCXA153.h>
#include <stdio.h>
#include "utils/gps/gps.h"
#include "games/gpsGame/gpsGame.h"
#include "games/LevelOne/levelOne.h"
#include "games/LevelTwo/levelTwo.h"
#include "games/LevelThree/levelThree.h"
#include "utils/comProtocols/GPIO/gpio_output.h"
#include "utils/comProtocols/GPIO/gpio_input.h"
#include "utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include "utils/comProtocols/Lpi2c/lpi2c0_controller_interrupt.h"
#include "utils/sdCard/sdCard.h"
#include "utils/joystick/joystick.h"
#include "utils/lcdScreen/lcd.h"
#include "utils/colourSensor/tcs34725_polling.h"
#include "utils/temperatureSensor/lm35d_polling.h"
#include "utils/solenoid/solenoid.h"
#include "utils/flag.h"
#include "utils/timer.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// -----------------------------------------------------------------------------
// Local type definitions
// -----------------------------------------------------------------------------
#ifdef DEBUG
#define TARGETSTR "Debug"
#else
#define TARGETSTR "Release"
#endif

#define EARTH_RADIUS 6371000  // Earth rad in meters

typedef struct Direction
{
 	char abreviation[128];
}direction_t;

enum gameState
{
LEVEL1,
LEVEL2,
LEVEL3,
LEVEL4
};
coordinates_t targets[4];
// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------
//int ms = 0;
//int won = 0;
int wrote = 1;
char c;
// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------


int main(void)
{


	int start = 0;
	int u = 1;
	solenoidInit();
    sdInit();
    lpi2c0_controller_init();
    tcs34725_init();
    lcd_init();
    gpio_output_init();
    gpio_input_init();
    gpsInit();
    timerInit();
    lm35d_init();
    int gameState = LEVEL1; //change to settings from csv file

           // Enable interrupts
  __enable_irq();

  sdWrite();

  	lcd_clear();
   //use f_unlink to delete the file after the run

    while(1)
    {
    	disableLpuart2();
    		while(*getWon() == 1)
    		{

    			while(lpuart0_rxcnt() > 0)
    			{

    			c = lpuart0_getchar();
    			printf("%c",c);
    				if(c == '<' && *getWon() == 1)
    			    {
    			    	sdReadLogs(); //send logs to laptop
    			    }

    				if(c == '>' && *getWon() != 1)
    		    	{
    		    		sdSettings(); //read settings from laptop
    		   			}
    				else if(c == '*')
    				{
    				    start = 1; //laptop app closed
    				    if(*getWon() == 1)
    				    {
    				    	setWon(2);
    				    }
    				}
    			}
    		}
    	enableLpuart2();
    	updatePosition();


    	if(*getSuccessFlag())
    	{  setSuccessFlag(false);
    		gameState++;}

    	if(getFix() == 0)
    	{
    	LCDupdate();


    	switch(gameState)
    	{

    	case LEVEL1:
        	sdReadSettings(getTarget(),LEVEL1);
        	if(*getDistance() < 10)
        	{
        	levelOne();

        	}
    	    break;
    	case LEVEL2:
    		sdReadSettings(getTarget(),LEVEL2);
    		levelTwo();

    		break;
    	case LEVEL3:
    		sdReadSettings(getTarget(),LEVEL3);
    		setWon(1);
    		break;
    	case LEVEL4:
    		sdReadSettings(getTarget(),LEVEL4);

    		//add level Four Gameloop
    		//needs to include updatePosition on every iteration as well so that logs work accurately
    	    break;

    	}

    	}

    	/*
    	if(*getWon() == 1)
    	{
    		if(1){
    		//openLock();
    		//closeLock();
    		}
    		disableLpuart2();
    		int closed = 0;
    		while(!closed)
    		{
    			c = lpuart0_getchar();
    				if(c == '<')
    				{
    					sdReadLogs(); //send logs to laptop
    					}

    				else if(c == '>')
    		    	{
    		    		sdSettings(); //read settings from laptop
    		   			}
    				else if(c == '*')
    				{
    				    closed = 1; //laptop app closed
    				}
    		}
    		//printf("done");
    		setWon(2);
    	}
    	*/




/*
    }


}
*/

// -----------------------------------------------------------------------------
// Local function implementation

/*! ***************************************************************************
 *
 * \brief     Main application
 * \file      main.c
 * \author    Hugo Arends
 * \date      February 2024
 *
 * \see       NXP. (2024). MCX A153, A152, A143, A142 Reference Manual. Rev. 4,
 *            01/2024. From:
 *            https://www.nxp.com/docs/en/reference-manual/MCXAP64M96FS3RM.pdf
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include <MCXA153.h>
#include <stdio.h>
#include "utils/gps/gps.h"
#include "games/gpsGame/gpsGame.h"
#include "games/LevelOne/levelOne.h"
#include "games/LevelTwo/levelTwo.h"
#include "games/LevelThree/levelThree.h"
#include "games/LevelFour/levelFour.h"
#include "utils/comProtocols/GPIO/gpio_output.h"
#include "utils/comProtocols/GPIO/gpio_input.h"
#include "utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include "utils/comProtocols/Lpi2c/lpi2c0_controller_interrupt.h"
#include "utils/sdCard/sdCard.h"
#include "utils/joystick/joystick.h"
#include "utils/lcdScreen/lcd.h"
#include "utils/colourSensor/tcs34725_polling.h"
#include "utils/temperatureSensor/lm35d_polling.h"
//#include "utils/solenoid/solenoid.h"
#include "utils/flag.h"
#include "utils/timer.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// -----------------------------------------------------------------------------
// Local type definitions
// -----------------------------------------------------------------------------
#ifdef DEBUG
#define TARGETSTR "Debug"
#else
#define TARGETSTR "Release"
#endif

#define EARTH_RADIUS 6371000  // Earth rad in meters

typedef struct Direction
{
 	char abreviation[128];
}direction_t;

enum gameState
{
LEVEL1,
LEVEL2,
LEVEL3,
LEVEL4
};
coordinates_t targets[4];
// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------
//int ms = 0;
//int won = 0;
int started = 1;
// -----------------------------------------------------------------------------
// Main application
// -----------------------------------------------------------------------------


int main(void)
{

		int u = 1;
	    sdInit();
	    lpi2c0_controller_init();
	    tcs34725_init();
	    lcd_init();
	    gpio_output_init();
	    gpio_input_init();
	    gpsInit();
	    timerInit();
	    lm35d_init();
	    int gameState = LEVEL1; //change to settings from csv file


	           // Enable interrupts
	  __enable_irq();

	  sdWrite();

	  	lcd_clear();
	   //use f_unlink to delete the file after the run

	    while(1)
	    {

	    	updatePosition();
	    	//levelFour();



	    	LCDupdate();

	    	if(*getSuccessFlag())
	    	{gameState++;
	    	setSuccessFlag(false);}

	    	if(getFix() == 0)
	    	{
	    	//LCDupdate();// checks buffer for new relevant NMEA sentence every cycle
	    	//might add seperate file to handle all the game controlling and switching stuff


	    	switch(gameState)
	    	{

	    	case LEVEL1:
	        	sdReadSettings(getTarget(),LEVEL1);
	        	levelOne();
	    		//sdReadSettings(getTarget(),LEVEL1);
	    		//add level One Gameloop
	    		//needs to include updatePosition on every iteration as well so that logs work accurately
	    	    break;
	    	case LEVEL2:

	    		levelTwo();

	    	    break;
	    	case LEVEL3:
	    		sdReadSettings(getTarget(),LEVEL3);
	    		levelThree();

	    		//add level Three Gameloop
	    		//needs to include updatePosition on every iteration as well so that logs work accurately
	    		break;
	    	case LEVEL4:
	    		sdReadSettings(getTarget(),LEVEL4);
	    		setWon(1);

	    		//add level Four Gameloop
	    		//needs to include updatePosition on every iteration as well so that logs work accurately
	    	    break;

	    	}
	    	}
	    	if(*getWon() == 1)
	    	{
	    		disableLpuart2();
	    		int closed = 0;
	    		while(!closed)
	    		{
    			char c = lpuart0_getchar();
	    				if(c == '<')
	    				{
	    					sdReadLogs(); //send logs to laptop
	    					}

	    				else if(c == '>')
	    		    	{
	    		    		sdSettings(); //read settings from laptop
	    		   			}
	    				else if(c == '*')
	    				{
	    				    closed = 1; //read settings from laptop
	    				}
	    		}
	    		//printf("done");
	    		setWon(2);
	    	}


}
}


// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------
/*
void SysTick_Handler(void)
{
	 	ms++;

	    if((ms % 1000) == 0 && won == 0 && getFix() == 0) // change fix condition to 1
	    {
	    	sdLog(29, getPosition());
	    }
	    if((ms % 20000) == 0 && won == 0)
	    {
	    	won = 1;
	    }

}
*/
