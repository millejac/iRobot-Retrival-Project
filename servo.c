/*
 * servo.c
 *
 *  Created on: Apr 5, 2021
 *      Author: jcheatum
 */

#include <stdint.h>

#include "servo.h"
#include <inc/tm4c123gh6pm.h>

#define ZERO_DEG_PW 9.0/16 // the pulse width, in ms, that puts the servo at 0 degrees
#define NINETY_DEG_PW 20.5/16 // the pulse width, in ms, that puts the servo at 90 degrees

void servo_init() {
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCTIMER_R |= 0b10;
    while((SYSCTL_PRGPIO_R & 0x2) == 0);
    while((SYSCTL_RCGCTIMER_R & 0x2) == 0);

    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xF00000) | 0x700000;
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;

    TIMER1_CTL_R &= ~0x100;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R |= 0x0B;
    TIMER1_CTL_R &= ~0x4000;
    TIMER1_CTL_R |= 0xC00;
    TIMER1_CTL_R |= 0x0100;
}

int servo_move(float degrees) {

    TIMER1_TBPR_R |= 0x04;
    TIMER1_TBILR_R = 0xE200;

    float m, pw_ms;
    uint32_t pw_cl, match_val;

    if(degrees < 90) {
        m = NINETY_DEG_PW - ZERO_DEG_PW; //size of the range of ms
        pw_ms = ZERO_DEG_PW + (degrees * m / 90); //pulse width in ms
        pw_cl = 16000 * pw_ms; //pulse width in clock cycles
        match_val = 320000 - pw_cl;
    } else if(degrees > 90) {
        m = 2.0 - NINETY_DEG_PW; //size of range of ms
        pw_ms = NINETY_DEG_PW + ((degrees-90) * m / 90);
        pw_cl = 16000 * pw_ms;
        match_val = 320000 - pw_cl;
    } else { //degrees == 90
        pw_cl = 16000 * NINETY_DEG_PW;
        match_val = 320000 - pw_cl;
    }

    TIMER1_TBPMR_R = match_val >> 16;
    TIMER1_TBMATCHR_R = match_val & 0xffff;
}
