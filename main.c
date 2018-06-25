#include "xiot_task.h"


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



//========================================= ISRs ============================================


/****************************************** ADC *********************************************/

//ADC Interrupt service routine which reads the temperature from sensor LM35 and display it on terminal using uart
//in LM35 1 degree = 10 mv increase -> degrees = ADC reading / 4
ISR(ADC_vect)
{
	//using a static char array to avoid using dynamic allocation in such small memory
	char temprature_full_text[100] = "Temperature is now ";
	
	//read temperature in Celesius
	unsigned int temp_in_C = ADC/4;
	
	char temperature_number_text [10];
	sprintf(temperature_number_text, "%d", temp_in_C);
	
	strcat(temprature_full_text, temperature_number_text);
	strcat(temprature_full_text, " C\n\r");
	
	//display the data on serial uart
	uart_send_word(temprature_full_text);
}

/********************************************************************************************/


/****************************************** UART ********************************************/

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

/********************************************************************************************/


/****************************************** Timer *******************************************/
//Timer0 interrupt service routine (initiated every 3 seconds) to initiate the ADC to get the temperature reading
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
/********************************************************************************************/
