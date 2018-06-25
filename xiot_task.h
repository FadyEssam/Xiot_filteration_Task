
#ifndef XIOT_TASK_H_
#define XIOT_TASK_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

typedef volatile unsigned char  v_uint_8;
typedef unsigned char  uint_8;


//=====================Input & Outputs Registers==========================================
//registers of input switch
#define SWITCH_DIRECTION_R DDRD
#define SWITCH_PIN_R PIND
#define SWITCH_PORT_R PORTD
#define SWICH_PIN_NUM 2

//registers of output LED
#define LED_DIRECTION_R DDRD
#define LED_PIN_R PIND
#define LED_PORT_R PORTD
#define LED_PIN_NUM 3
//=========================================================================================



//==============================Functions Prototypes & ISRs================================

/**************************Essential Bit operations****************************************/

  void set_bit(v_uint_8 *x, uint_8 bitNum);
  void reset_bit(v_uint_8 *x, uint_8 bitNum);
  void toggle_bit(v_uint_8 *x, uint_8 bitNum);
  uint_8 read_bit(v_uint_8 *x, uint_8 bitNum);

/****************************************************************************************/





/************************************** LED ********************************************/
//initialize the led pin function prototype
void init_led();


//toggle the led pin function prototype
void toggle_led();

/***************************************************************************************/






/************************************ UART *********************************************/
//In this project UART is used with baud rate = 9600 to communicate with terminal (PC) and display words

//initialize the uart function prototype
void init_uart();

//send character via uart
void uart_send(char data);

//send a c-string using uart
void uart_send_word(char* data);
/****************************************************************************************/





/************************************** Switch ********************************************/

//initialize the input switch port function prototype
void init_switch();


//read the input switch value function prototype
uint_8 is_switch_pressed();


/******************************************************************************************/






/************************************** Timer ******************************************/
//Timer0 is used in this project to initiate The ADC to read temperature from temprature sensor (LM35) every exactly 3 seconds

//initialize timer function prototypes
void init_timer0();

/*****************************************************************************************/






/************************************** ADC ********************************************/


//initialize adc
//1024 step , voltage refrence = 2.56 , step = 2.5 mv
void init_adc();


/*****************************************************************************************/



#endif /* XIOT_TASK_H_ */
