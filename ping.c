/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include "math.h"
#include "lcd.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile long TIME_DIFF = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

    SYSCTL_RCGCTIMER_R |= 0x08;   //enable clock to timer3
    SYSCTL_RCGCGPIO_R |= 0x02;      //enable clock to gpioB
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};  //wait until portb is ready
    while ((SYSCTL_RCGCTIMER_R & 0x08) == 0) {};  //wait until timer3 is ready

    GPIO_PORTB_DEN_R |= 0x08;   //set pb3 as a digital pin
    GPIO_PORTB_AFSEL_R |= 0x08;   //enable alternate function for pb3
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xF000) | 0x7000;     //set pb3 to T3CCp1

    TIMER3_CTL_R &= ~0x0100;    //disable timer 3b
    TIMER3_CFG_R = 0x4;         //16 bit timer mode
    TIMER3_TBMR_R = 0x07;       //set capture mode, count down, edge-time
    TIMER3_CTL_R |= 0xC00;      //set timer 3b event to both edges
    TIMER3_TBPR_R |= 0xFF;      //set pre-scaler to max value
    TIMER3_TBILR_R |= 0xFFFF;   //set timer3b to max value
    TIMER3_ICR_R |= 0x400;      //clear capture interrupt flag
    TIMER3_IMR_R |= 0x400;       //enable capture mode interrupt
    TIMER3_CTL_R |= 0x0100;     //enable timer 3b

    NVIC_PRI9_R = (NVIC_PRI9_R & ~0xE0) | 0x20;        //set timer3b interrupt to first priority
    NVIC_EN1_R |= 0x10;         //enable timer3b interrupts in NVIC

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

}

void ping_trigger (void){
    STATE = LOW;
    TIME_DIFF = 0;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x0100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0x08;       //set pb3 as output
    GPIO_PORTB_DATA_R &= ~0x08;          //write pb3 low
    GPIO_PORTB_DATA_R |= 0x08;          //write pb3 high
    timer_waitMicros(5);            //wait 5us
    GPIO_PORTB_DATA_R &= ~0x08;          //write pb3 low
    GPIO_PORTB_DIR_R &= ~0x08;      //set pb3 as input

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x0100;
}

void TIMER3B_Handler(void){

  if (TIMER3_MIS_R & 0x400) {

      if(STATE == LOW) {                        //checks to see if start of echo

          START_TIME = TIMER3_TBR_R;
          STATE = HIGH;
      }
      else if(STATE == HIGH) {                  // checks to see if echo is high

          END_TIME = TIMER3_TBR_R;
          TIME_DIFF = START_TIME - END_TIME;    //calc time diff
          STATE = DONE;                         //end of echo
          END_TIME = 0;
          START_TIME = 0;
      }

      TIMER3_ICR_R |= 0x400;      //clear capture interrupt flag
  }

}



float ping_getDistance (void){

    float clk_cycles;
    float distance;
    ping_trigger();
    while (STATE != DONE) {};

    //clk_cycles = TIME_DIFF;
    distance = (TIME_DIFF / 2) * (62.5 * pow(10, -9 )) * 344 * 100;

      /*if (clk_cycles < 0) {
            lcd_printf("overflow");
      }
      else {

            lcd_printf("%.1f cycles      %.1f cm", clk_cycles, distance);
      }*/
     return distance;
}
