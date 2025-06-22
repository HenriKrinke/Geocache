/*
 * gpio_input.c
 *
 *  Created on: 19 Jun 2025
 *      Author: henri
 */
#include <stdbool.h>
#include "gpio_input.h"

void gpio_input_init()
{
	MRCC0->MRCC_GLB_CC0_SET = MRCC_MRCC_GLB_CC0_PORT1(1);
	MRCC0->MRCC_GLB_CC0_SET = MRCC_MRCC_GLB_CC0_PORT2(1);
	MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_PORT3(1);
	MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO1(1);
	MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO2(1);
	MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);

	MRCC0->MRCC_GLB_RST0_SET = MRCC_MRCC_GLB_CC0_PORT1(1);
	MRCC0->MRCC_GLB_RST0_SET = MRCC_MRCC_GLB_CC0_PORT2(1);
	MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_PORT3(1);
	MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_GPIO1(1);
	MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_GPIO2(1);
	MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);

	//Lines
    PORT2->PCR[1] = PORT_PCR_LK(1) | PORT_PCR_IBE(1) | PORT_PCR_MUX(0) | PORT_PCR_PS(1)| PORT_PCR_PE(1);
    PORT2->PCR[2] = PORT_PCR_LK(1) | PORT_PCR_IBE(1) | PORT_PCR_MUX(0)| PORT_PCR_PS(1)| PORT_PCR_PE(1);;
    PORT2->PCR[3] = PORT_PCR_LK(1) | PORT_PCR_IBE(1) | PORT_PCR_MUX(0)| PORT_PCR_PS(1)| PORT_PCR_PE(1);;

   //
    PORT3->PCR[11] = PORT_PCR_LK(1) | PORT_PCR_IBE(1) | PORT_PCR_MUX(0);

}
bool checkPin(int port,int pin)
{
	switch(port)
	{
	case 1:
		if((GPIO1->PDIR & (1<<pin)) == 0){return true;}
		else{return false;}
		break;
	case 2:
		if((GPIO2->PDIR & (1<<pin)) == 0){return true;}
		else{return false;}
		break;
	case 3:
		if((GPIO3->PDIR & (1<<pin)) == 0){return true;}
		else{return false;}
		break;

	}

}
