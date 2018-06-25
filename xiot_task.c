#include "xiot_task.h"

/**************************Essential Bit operations**************************************/
  void set_bit(v_uint_8 *x, uint_8 bitNum) {
	*x |= (1 << bitNum);
}

  void reset_bit(v_uint_8 *x, uint_8 bitNum) {
	*x &= ~(1 << bitNum);
}

  void toggle_bit(v_uint_8 *x, uint_8 bitNum) {
	*x ^= (1 << bitNum);
}

  uint_8 read_bit(v_uint_8 *x, uint_8 bitNum) {
	return (((*x) & 1<<bitNum) >> bitNum);
}

/***************************************************************************************/






/**************************************** LED *******************************************/

//initialize the led pin
void init_led()
{
	set_bit(&LED_DIRECTION_R, LED_PIN_NUM);
	set_bit(&LED_PORT_R, LED_PIN_NUM);
}

//toggle the led pin
void toggle_led()
{
	toggle_bit(&LED_PORT_R, LED_PIN_NUM);
}


/****************************************************************************************/







/************************************ UART *********************************************/

//initialize uart
void init_uart()
{
	
	set_bit(&PORTD, 1);
	
	
	//UCSRB....
	//Bit 7 = 0 -> disable recieving data interrupts
	//Bit 6 = 0 -> disable sending data interrupts
	//Bit 5 = 0 -> diable uart empty interrupts
	//Bit 4 = 0 -> disable receiver
	//Bit 3 = 1 -> Transmitter enable
	//Bit 2 = 0 -> 8-bits mode
	//Bit 1 = 0 -> 8-bits mode
	//Bit 1 = 0 -> 8-bits mode
	UCSRB = 0b00001000;
	
	//UCSRC....
	//Bit 7 = 1 -> Enable UCSRC
	//Bit 6 = 0 -> Asynchronous
	//Bit 5 = 0 -> no parity
	//Bit 4 = 0 -> no parity
	//Bit 3 = 0 -> one stop bit
	//Bit 2 = 1 -> 8 bits mode
	//Bit 1 = 1 -> 8 bits mode
	//Bit 0 = x -> used for synchronous mode
	UCSRC = 0b00000110;
	
	//enable x2 speed
	set_bit(&UCSRA, 1);
	
	UBRRL = 12; //set baud rate of 9600
	
}

//send a character using uart
void uart_send(char data)
{
	while(!read_bit(&UCSRA, UDRE)); //wait until the transmitter is empty
	UDR = data;
}


//send a whole c-string using uart
void uart_send_word(char* data)
{
	unsigned int length = strlen(data);
	int i;
	
	for(i = 0; i<length; i++)
	uart_send(*(data+i));
}

/***************************************************************************************/








/************************************** Switch *******************************************/

//initialize the input switch port
void init_switch()
{
	reset_bit(&SWITCH_DIRECTION_R, SWICH_PIN_NUM);
	set_bit(&SWITCH_PORT_R, SWICH_PIN_NUM);  //to enable pull up resistance
	
	//enable interrupt for INT0 (PORT D PIN 2)
	set_bit(&MCUCR, 1);
	reset_bit(&MCUCR, 0);
	set_bit(&GICR, 6);
}

//read the input switch value
unsigned char is_switch_pressed()
{
	return !read_bit(&SWITCH_PIN_R , SWICH_PIN_NUM);
}

/****************************************************************************************/






/************************************** Timer *******************************************/

//initialize Timer0 to run on CTC mode
void init_timer0()
{
	//clear timer on compare mode
	reset_bit(&TCCR0, WGM00);
	set_bit(&TCCR0, WGM01);
	
	//prescaler = 1024
	set_bit(&TCCR0, 0);
	set_bit(&TCCR0, 2);
	
	//enable interrupt
	set_bit(&TIMSK, OCIE0);
	OCR0 = 244;
}

/***************************************************************************************/


/************************************** ADC ********************************************/


//initialize ADC
//1024 step , voltage refrence = 2.56 , step = 2.5 mv
void init_adc()
{
	
	//set pin ADC0 to input (Port A Pin 0)
	reset_bit(&DDRA, 0);
	
	
	//Single ended input adc0
	//voltage refrence = 2.56  (1024 step) ---> step = 2.5 mv
	ADMUX = 0b11000000;
	
	//control register
	ADCSRA = 0;
	ADCSRA |= 0b111; //128 prescaler
	set_bit(&ADCSRA, 3); //enable interrupt
	set_bit(&ADCSRA, 7); //enable adc


	//set ADC to work at timer0 compare match
	reset_bit(&SFIOR, 5);
	reset_bit(&SFIOR, 6);
	reset_bit(&SFIOR, 7);

}


/****************************************************************************************/

