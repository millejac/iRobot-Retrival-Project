/*
*
*   uart.c
*
*
*
*   @author
*   @date
*/
/*
#include "uart.h"
#include <stdint.h>
#include <string.h>

void uart_init(void){
	//TODO
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0b00000010;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0x02;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0x02 ) == 0) {};
  while ((SYSCTL_PRUART_R & 0x02 ) == 0) {};

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0b00000011;

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0b00000011;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) | 0x00000011;

  int baudrate = 115200;
  //calculate baud rate
  uint16_t iBRD = 16000000/(16*baudrate); //use equations
  uint16_t fBRD = (uint16_t) ((16000000/(16.0*baudrate) - 16000000/(16*baudrate)) * 64 + .5); //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= ~0x00000001;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0x00000060;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  UART1_CTL_R = 0x00000301; //0b11 0000 0001

}

void uart_sendChar(char data){
	while((UART1_FR_R & 0x20) != 0) {}
	UART1_DR_R = data;
}

char uart_receive(void){
	while((UART1_FR_R & 0x10) != 0) {}
	return ((unsigned char) (UART1_DR_R & 0xFF)); //if weird stuff happens, change to 0x7f
}

void uart_sendStr(const char *data){
    int i;
    for(i = 0; i < strlen(data); i++) {
        uart_sendChar(data[i]);
    }
}
*/
