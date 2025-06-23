/*
 * flag.h
 *
 *  Created on: 13 Jun 2025
 *      Author: henri
 */

#ifndef UTILS_FLAG_H_
#define UTILS_FLAG_H_
#include <stdbool.h>

void setLCDFlag(int flag);
int* getLCDFlag();
void setSuccessFlag(bool flag);
bool* getSuccessFlag();
void LCDupdate();
void setLevelFlag(int a);


#endif /* UTILS_FLAG_H_ */
