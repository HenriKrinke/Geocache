/*! ***************************************************************************
 *
 * \brief     General purpose input/output - output
 * \file      gpio_output.c
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
#include "gpio_output.h"

// -----------------------------------------------------------------------------
// Local type definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------
void gpio_output_init(void)
{
    // From section 14.3 Functional description (NXP, 2024)
    //
    // Before a module can be used, its clocks must be enabled (CC != 00) and it
    // must be released from reset (MRCC_GLB_RST [peripherals name] = 1). If a
    // module is not released from reset (MRCC_GLB_RST [peripherals name] = 0),
    // an attempt to access a register within that module is terminated with a
    // bus error.

    // Enable modules and leave others unchanged
    // GPIO3: [1] = Peripheral clock is enabled
    // PORT3: [1] = Peripheral clock is enabled
    MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_PORT3(1);
    MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);

    // Release modules from reset and leave others unchanged
    // GPIO3: [1] = Peripheral is released from reset
    // PORT3: [1] = Peripheral is released from reset
    MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_PORT3(1);
    MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);

    // From section 11.4 Initialization (NXP, 2024)
    //
    // 1. Initialize the pin functions:
    //    -  Initialize single pin functions by writing appropriate values to
    //       PCRn
    //    -  Initialize multiple pins (up to 16) with the same configuration by
    //       writing appropriate values to Global Pin Control Low (GPCLR) or
    //       Global Pin Control High (GPCHR).
    // 2. Lock the configuration for a given pin, by writing 1 to PCRn [LK], so
    //    that it cannot be changed until the next reset.

    // 1. & 2.
    //
    // Configure P3_13
    // LK : [1] = Locks this PCR
    // INV: [0] = Does not invert
    // IBE: [0] = Disables
    // MUX: [0000] = Alternative 0 (GPIO)
    // DSE: [0] = low drive strength is configured on the corresponding pin,
    //            if the pin is configured as a digital output
    // ODE: [0] = Disables
    // SRE: [0] = Fast
    // PE:  [0] = Disables
    // PS:  [0] = n.a.
    PORT3->PCR[13] = PORT_PCR_LK(1);
    PORT3->PCR[12] = PORT_PCR_LK(1);
    PORT3->PCR[0] = PORT_PCR_LK(1);

    // From section 12.5 Initialization (NXP, 2024)
    //
    // 1. Initialize the GPIO pins for the output function:
    //    a. Configure the output logic value for each pin by using Port Data
    //       Output (PDOR).
    //    b. Configure the direction for each pin by using Port Data Direction
    //       (PDDR).
    // 2. Interrupt function not used.

    // 1. a.
    //
    // PDO13, PDO12 and PDO0: [1] = Logic level 1 – LEDs off
    GPIO3->PDOR |= (1<<13) | (1<<12) | (1<<0);

    // 1. b.
    //
    // PDD13, PDO12 and PDO0: [1] = Output
    GPIO3->PDDR |= (1<<13) | (1<<12) | (1<<0);
}
