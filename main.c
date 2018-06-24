#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

typedef volatile unsigned char  v_uint_8;
typedef unsigned char  uint_8;

//registers of input switch
#define SWITCH_DIRECTION_R DDRD
#define SWITCH_PIN_R PIND
#define SWITCH_PORT_R PORTD
#define SWICH_PIN_NUM 2

//registers of output switch
#define LED_DIRECTION_R DDRD
#define LED_PIN_R PIND
#define LED_PORT_R PORTD
#define LED_PIN_NUM 3

//bitwise operations prototypes
static inline void set_bit(v_uint_8 *x, uint_8 bitNum);
static inline void reset_bit(v_uint_8 *x, uint_8 bitNum);
static inline void toggle_bit(v_uint_8 *x, uint_8 bitNum);
static inline uint_8 read_bit(v_uint_8 *x, uint_8 bitNum);

//initialize the input switch port function prototype
void init_switch();

//read the input switch value function prototype
uint_8 is_switch_pressed();

//initialize the led pin function prototype
void init_led();

//toggle the led pin function prototype 
void toggle_led();

//initialize the uart function prototype
void init_uart();

//send character via uart
void uart_send(char data);

//send a c-string using uart
void uart_send_word(char* data);

//interrupt at falling edge of pin D2
ISR(INT0_vect)
{
	toggle_led();
	
	if(read_bit(&LED_PORT_R , LED_PIN_NUM))
		uart_send_word("LED is on\n\r");
	
	else
		uart_send_word("LED is off\n\r");
}

int main(void)
{
	//initialize pins
	init_led();
	init_switch();
	init_uart();
	sei(); //enable global interrupts
	
    while (1) 
    {
	}
}


//=========================Functions bodies===============================

//bitwise operations

static inline void set_bit(v_uint_8 *x, uint_8 bitNum) {
	*x |= (1 << bitNum);
}

static inline void reset_bit(v_uint_8 *x, uint_8 bitNum) {
	*x &= ~(1 << bitNum);
}

static inline void toggle_bit(v_uint_8 *x, uint_8 bitNum) {
	*x ^= (1 << bitNum);
}

static inline uint_8 read_bit(v_uint_8 *x, uint_8 bitNum) {
	return (((*x) & 1<<bitNum) >> bitNum);
}

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
