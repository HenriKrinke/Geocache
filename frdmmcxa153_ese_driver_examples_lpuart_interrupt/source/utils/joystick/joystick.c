/*!
 * \file      joystick.c
 * \brief     Joystick handling for MCXA153 (VRx, VRy, and SW)

 */

#include "joystick.h"
#include "MCXA153.h"
#include "delay.h"
// ----------------------------------------------------------------------------
// Local variables
// ----------------------------------------------------------------------------
static volatile uint32_t joystick_sw_press_count = 0;

// ----------------------------------------------------------------------------
// Joystick initialization (ADC0 and SW button)
// ----------------------------------------------------------------------------
void joystick_init(void)
{
    // Enable clocks for PORT1, GPIO1, ADC0
    MRCC0->MRCC_GLB_CC0_SET = MRCC_MRCC_GLB_CC0_PORT1(1) | MRCC_MRCC_GLB_CC0_ADC0(1);
    MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO1(1);
    MRCC0->MRCC_GLB_RST0_SET = MRCC_MRCC_GLB_RST0_PORT1(1) | MRCC_MRCC_GLB_RST0_ADC0(1);
    MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_RST1_GPIO1(1);

    // Configure Joystick SW pin as GPIO input with pull-up
    PORT1->PCR[7] = PORT_PCR_LK(1) | PORT_PCR_IBE(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(0);

    // Falling edge interrupt on P1_07
    GPIO1->ICR[7] = GPIO_ICR_ISF(1) | GPIO_ICR_IRQC(0b1010);

    // Enable interrupt
    NVIC_SetPriority(GPIO1_IRQn, 3);
    NVIC_ClearPendingIRQ(GPIO1_IRQn);
    NVIC_EnableIRQ(GPIO1_IRQn);

    // ---------- ADC0 setup ----------

    MRCC0->MRCC_ADC0_CLKSEL = MRCC_MRCC_ADC0_CLKSEL_MUX(0);
    MRCC0->MRCC_ADC0_CLKDIV = 0;

    ADC0->CFG = ADC_CFG_PWREN(1) | ADC_CFG_PUDLY(0x80) | ADC_CFG_REFSEL(0b10);
    ADC0->CTRL |= ADC_CTRL_ADCEN(1) | ADC_CTRL_CAL_AVGS(0b1010);
    ADC0->CTRL |= ADC_CTRL_CALOFS(1);
    while ((ADC0->STAT & ADC_STAT_CAL_RDY_MASK) == 0) {}

    ADC0->CTRL |= ADC_CTRL_CAL_REQ(1);
    while ((ADC0->GCC[0] & ADC_GCC_RDY_MASK) == 0) {}

    uint16_t value = (uint16_t)ADC0->GCC[0];
    float gain_adj = 131072.0f / (131072.0f - value);
    uint32_t result = (uint32_t)(gain_adj * (1UL << 16));

    ADC0->GCR[0] = ADC_GCR_GCALR(result);
    ADC0->GCR[0] |= ADC_GCR_RDY(1);
}

// ----------------------------------------------------------------------------
// Interrupt handler
// ----------------------------------------------------------------------------
void GPIO1_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(GPIO1_IRQn);

    if ((GPIO1->ISFR[0] & GPIO_ISFR_ISF7(1)) != 0) {
        GPIO1->ISFR[0] = GPIO_ISFR_ISF7(1);
        joystick_sw_press_count++;
    }
}

// ----------------------------------------------------------------------------
// Joystick SW press
// ----------------------------------------------------------------------------
bool joystick_sw(void)
{
    if (joystick_sw_press_count > 0) {
        joystick_sw_press_count--;
        delay_ms(10);
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Joystick VRx
// ----------------------------------------------------------------------------
uint16_t joystick_vrx(void)
{
    ADC0->STAT |= ADC_STAT_TCOMP_INT_MASK;
    ADC0->TSTAT |= ADC_TSTAT_TCOMP_FLAG(0b0001);
    ADC0->CMD[0].CMDL = ADC_CMDL_MODE(1) | ADC_CMDL_ADCH(13);
    ADC0->TCTRL[0] = ADC_TCTRL_TCMD(0b001);
    ADC0->SWTRIG = ADC_SWTRIG_SWT0(1);

    uint32_t timeout = 10000;
    while (((ADC0->STAT & ADC_STAT_TCOMP_INT_MASK) == 0) && --timeout) {}
    ADC0->STAT |= ADC_STAT_TCOMP_INT_MASK;
    timeout = 10000;
    while (((ADC0->TSTAT & ADC_TSTAT_TCOMP_FLAG_MASK) == 0) && --timeout) {}
    ADC0->TSTAT |= ADC_TSTAT_TCOMP_FLAG(0b0001);

    return (uint16_t)(ADC0->RESFIFO);
}

// ----------------------------------------------------------------------------
// Joystick VRy
// ----------------------------------------------------------------------------
uint16_t joystick_vry(void)
{
    ADC0->STAT |= ADC_STAT_TCOMP_INT_MASK;
    ADC0->TSTAT |= ADC_TSTAT_TCOMP_FLAG(0b0001);
    ADC0->CMD[0].CMDL = ADC_CMDL_MODE(1) | ADC_CMDL_ADCH(12);
    ADC0->TCTRL[0] = ADC_TCTRL_TCMD(0b001);
    ADC0->SWTRIG = ADC_SWTRIG_SWT0(1);

    uint32_t timeout = 10000;
    while (((ADC0->STAT & ADC_STAT_TCOMP_INT_MASK) == 0) && --timeout) {}
    ADC0->STAT |= ADC_STAT_TCOMP_INT_MASK;
    timeout = 10000;
    while (((ADC0->TSTAT & ADC_TSTAT_TCOMP_FLAG_MASK) == 0) && --timeout) {}
    ADC0->TSTAT |= ADC_TSTAT_TCOMP_FLAG(0b0001);

    return (uint16_t)(ADC0->RESFIFO);
}
