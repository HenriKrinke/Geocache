/*
 * solenoid.c
 *
 *  Created on: 18 Jun 2025
 *      Author: henri
 */
#include "../comProtocols/GPIO/gpio_output.h"
#include "solenoid.h"
void solenoidInit()
{
gpio_output_init();
}
void openLock()
{
	GPIO3->PDOR |=  (1<<12);	}
void closeLock()
{
	GPIO3->PCOR |=  (1<<12);
}
