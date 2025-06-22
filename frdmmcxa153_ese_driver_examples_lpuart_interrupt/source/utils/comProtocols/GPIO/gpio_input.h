/*
 * gpio_input.h
 *
 *  Created on: 19 Jun 2025
 *      Author: henri
 */

#ifndef UTILS_COMPROTOCOLS_GPIO_GPIO_INPUT_H_
#define UTILS_COMPROTOCOLS_GPIO_GPIO_INPUT_H_
#include <stdbool.h>
#include <MCXA153.h>
void gpio_input_init();
bool checkPin(int port,int pin);

#endif /* UTILS_COMPROTOCOLS_GPIO_GPIO_INPUT_H_ */
