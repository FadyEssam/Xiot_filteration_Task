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

//registers of output switch
#define LED_DIRECTION_R DDRD
#define LED_PIN_R PIND
#define LED_PORT_R PORTD
#define LED_PIN_NUM 3
//=========================================================================================


//==============================Functions Prototypes & ISRs================================

/**************************Essential Bit operations****************************************/

static inline void set_bit(v_uint_8 *x, uint_8 bitNum);
static inline void reset_bit(v_uint_8 *x, uint_8 bitNum);
static inline void toggle_bit(v_uint_8 *x, uint_8 bitNum);
static inline uint_8 read_bit(v_uint_8 *x, uint_8 bitNum);

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


//interrupt service routine at falling edge of Switch
//It toggles the LED and sends text representing its state to the terminal (using uart)
ISR(INT0_vect)
{
	toggle_led();
	
	if(read_bit(&LED_PORT_R , LED_PIN_NUM))
	uart_send_word("LED is on\n\r");
	
	else
	uart_send_word("LED is off\n\r");
}

/******************************************************************************************/


/************************************** Timer ******************************************/
//Timer0 is used in this project to initiate The ADC to read temperature from temprature sensor (LM35) every exactly 3 seconds

//initialize timer function prototypes
void init_timer0();

//Timer0 interrupt service routine (initiated every 3 seconds)
ISR(TIMER0_COMP_vect)
{
	//used to count exactly 12 * 250ms = 3 s
	static int quarter_second_counter = 0;

	quarter_second_counter++;
	
	if(quarter_second_counter==12) //every 3 seconds exactly
	{
		set_bit(&ADCSRA, 6);
		quarter_second_counter = 0;
	}
}
/*****************************************************************************************/


/************************************** ADC ********************************************/


//initialize adc
//1024 step , voltage refrence = 2.56 , step = 2.5 mv
void init_adc();

//ADC Interrupt service routine which reads the temprature from sensor LM35 and display it on terminal using uart
//in LM35 1 degree = 10 mv increase -> degrees = ADC reading / 4
ISR(ADC_vect)
{
	//using a static char array to avoid using dynamic allocation in such small memory
	char temprature_full_text[100] = "Temprature is now ";
	
	//read temperature in Celesius
	unsigned int temp_in_C = ADC/4;
	
	char temperature_number_text [10];
	sprintf(temperature_number_text, "%d", temp_in_C);
	
	strcat(temprature_full_text, temperature_number_text);
	strcat(temprature_full_text, " C\n\r");
	
	//display the data on serial uart
	uart_send_word(temprature_full_text);
}

/*****************************************************************************************/


//======================================== Main =======================================

int main(void)
{
	//initialize modules and configuration
	
	sei(); //enable global interrupts

	init_led(); //initialize LED
	
	init_switch(); //initialize switch
	
	init_uart(); //initialize UART
	
	init_timer0(); //initialize Timer0
	
	init_adc(); //initialize ADC
	
	
    while (1) 
    {
	}
}

//=====================================================================================



//=========================Functions bodies============================================


/**************************Essential Bit operations**************************************/
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


