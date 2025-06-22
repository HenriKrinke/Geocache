/*
 * levelOne.c
 *
 *  Created on: 15 Jun 2025
 *      Author: henri
 */
#include "../../utils/joystick/joystick.h"
#include "../../utils/lcdScreen/lcd.h"
#include "../../utils/gps/gps.h"
#include "../../utils/flag.h"
#include "../../utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "delay.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "levelOne.h"


int countdownTime = BASE_TIMEOUT_MS;
int score = 0;
bool countdownActive = false;
bool arrowVisible = false;
bool gameActive = true;
bool stickInNeutral = true;
Direction currentArrow = NONE;

void init_hardware(void) {
    joystick_init();
    ;
    lcd_clear();
}

Direction getJoystickDirection(void) {
    int x = joystick_vrx();
    int y = joystick_vry();

    if (x > 60000 && y < 55000) return RIGHT;
    if (x < 5000  && y < 55000) return LEFT;
    if (y < 1000  && x < 55000) return UP;
    if (y > 60000 && x < 55000) return DOWN;

    return NONE;
}

void showArrow(void) {
    lcd_clear();
    lcd_set_cursor(0, 0);

    char buf[17];
    switch (currentArrow) {
        case UP:    snprintf(buf, sizeof(buf), "^  SCORE:%2d", score); break;
        case DOWN:  snprintf(buf, sizeof(buf), "v  SCORE:%2d", score); break;
        case LEFT:  snprintf(buf, sizeof(buf), "<  SCORE:%2d", score); break;
        case RIGHT: snprintf(buf, sizeof(buf), ">  SCORE:%2d", score); break;
        default:    snprintf(buf, sizeof(buf), "   SCORE:%2d", score); break;
    }

    lcd_send_string(buf);
}

void generateNewArrow(void) {
    currentArrow = (Direction)(rand() % 4);
    countdownActive = false;
    arrowVisible = true;
    showArrow();
}

void resetGame(void) {
    score = 0;
    countdownTime = BASE_TIMEOUT_MS;
    gameActive = true;
    countdownActive = false;
    arrowVisible = false;
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Game Reset!     ");

    disableLpuart2();
    delay_ms(1500);

    enableLpuart2();

    generateNewArrow();
}

void startCountdown(void) {
    countdownActive = true;
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Wrong!          ");

    disableLpuart2();
    delay_ms(3000);

    enableLpuart2();

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Press to Reset  ");

    int wait = 15000;
    while (wait > 0) {
        updatePosition();  //to ensure gps data gets update despite the wonderful while loops

        if (joystick_sw()) {
            while (joystick_sw()) delay_ms(10);
            resetGame();
            return;
        }
        delay_ms(10);
        wait -= 10;
    }

    resetGame();
}

void increaseScore(void) {
    score++;
    countdownTime = BASE_TIMEOUT_MS - score * 50;
    if (countdownTime < 800) countdownTime = 800;
}

void winGame(void) {
    gameActive = false;
    arrowVisible = false;
    countdownActive = false;
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("YOU WIN!        ");
    //protect from gps interrupts to prevent overflow
    disableLpuart2();

    delay_ms(5000);

    enableLpuart2();
    //remove this portion as the game would be done here

}

bool levelOne(void) {
    init_hardware(); //move
    srand(joystick_vrx());

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Press to Start  ");
    //update postition in the not pressed loop
    while (!joystick_sw()){
        updatePosition();  //to ensure gps data gets update despite the wondrful while loops
    	delay_ms(10);

    }
    while (joystick_sw()) delay_ms(10);

    delay_ms(100);

    generateNewArrow();
    stickInNeutral = true;

    while (1) {
        updatePosition();  //to ensure gps data gets update despite the wondrful while loops

    	//update in here or find way to remove 1 loop by doing above setup only once
        /*
        if (!gameActive && joystick_sw()) {
            resetGame();
            while (joystick_sw()) delay_ms(10);
            continue;
        }
*/ //redundant due to the next condition doing the same thing with only 1 of the 2 conditions
        if (joystick_sw()) {
            resetGame();
            while (joystick_sw()) delay_ms(10);
            continue;
        }

        int waitTime = countdownTime; //defined as 1500
        bool inputDetected = false;
        Direction move = NONE;

        while (waitTime > 0) {
            updatePosition();  //to ensure gps data gets update despite the wondrful while loops

        	//updatePosition here
            Direction dir = getJoystickDirection();

            if (dir == NONE) {
                stickInNeutral = true;
            } else if (stickInNeutral) {
                move = dir;
                inputDetected = true;
                stickInNeutral = false;
                break;
            }

            delay_ms(10);
            waitTime -= 10;
        }

        if (!inputDetected) {
            arrowVisible = false;
            countdownActive = true;
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_send_string("Too slow!       ");
            //protect delay from gps interrupt
            disableLpuart2();
            delay_ms(1500);
            enableLpuart2();

            startCountdown();
            continue;
        }

        if (move == currentArrow) {
            increaseScore();
            if (score >= MAX_SCORE) {
                winGame();
                setSuccessFlag(true);
                return true;
            } else {
                generateNewArrow();
            }
        } else {
            startCountdown();
        }
    }
}


