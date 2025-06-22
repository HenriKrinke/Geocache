/*
 * matrix.c
 *
 *  Created on: 20 Jun 2025
 *      Author: henri
 */
#include "matrix.h"
#include "../comProtocols/GPIO/gpio_input.h"
#include "../comProtocols/GPIO/gpio_output.h"

#define ROW_COUNT 3
#define COL_COUNT 3

// Define your row (input) and column (output) pin mapping
const int row_ports[ROW_COUNT] = {2, 2, 2};   // GPIO1 for example
const int row_pins[ROW_COUNT]  = {1, 2, 3};  // e.g., PTA8, PTA9, PTA10

const int col_ports[COL_COUNT] = {2, 2, 2};   // GPIO2 for example
const int col_pins[COL_COUNT]  = {4, 5, 6};   // e.g., PTB0, PTB1, PTB2

int button_matrix[ROW_COUNT][COL_COUNT];

const char* keymap[ROW_COUNT][COL_COUNT] = {
    {"",  "1", "" },
    {"2", "A", "3"},
    {"",  "4", "" },

};


// Matrix scanning function
void scanMatrix(void) {
    for (int col = 0; col < COL_COUNT; col++) {
        // Set all columns HIGH
        for (int i = 0; i < COL_COUNT; i++) {
            setPin(col_ports[i], col_pins[i], true);
        }

        // Pull current column LOW
        setPin(col_ports[col], col_pins[col], false);

        // Small delay if needed (debounce or signal settle)
        for (volatile int d = 0; d < 100; d++);

        // Read all rows
        for (int row = 0; row < ROW_COUNT; row++) {
            button_matrix[row][col] = checkPin(row_ports[row], row_pins[row]) ? 1 : 0;
        }
    }
}

void detectButtonPress(void) {
    scanMatrix(); // Update the button matrix

    for (int row = 0; row < ROW_COUNT; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
            if (button_matrix[row][col] == 1) {
                // Button at (row, col) is pressed
            	printf("Key Pressed: %s\n", keymap[row][col]);
                return; // Exit after detecting the first press
            }
        }
    }
}
