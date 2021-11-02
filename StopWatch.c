#include<avr/io.h>
#include"timer.h"
#include<avr/interrupt.h>
#include<util/delay.h>
volatile unsigned char second=0,minute=0,hour=0; // clock starts from 00:00:00
// ISR code of the TIMER1 will increment the second by 1 and check the condition
void TIMER1_Application(){

	second++;
	// check condition
	if(second == 60){ // if the second is 60 then it is one minute which is 00:01:00
	 second=0;  // here the second is 0
	 minute++;  // the minute will be incremented by one
	 if(minute == 60){// if minute become 60 then it is one houre which is 01:00:00
	    minute=0;
	    hour++;
	     if(hour == 24){//if the hour become 24 we will clock again from the zer0
	           hour=0;
	         }
	 }
	}
	// end of check
}

/* Priority of the external interrupt from high to low(in IVT):
 *   1- interrupt 0
 *   2- interrupt 1
 *   3- interrupt 2
 * */
/* note: when the interrupt happened the avr will disable the I-BIT in the SREG register and enable it
 * again when the ISR code is done . that will disable the nesting interrupt feature so the I-bit should
 * be enabled in the ISR code
 * */
//reset button : it will put zero in all 6-7segment
ISR(INT0_vect){
	SREG|=(1<<7); //to enable the global interrupt incase u press the other buttons
	second=0;
	minute=0;
	hour=0;
}

//pause button
ISR(INT1_vect){
	SREG|=(1<<7); //to enable the global interrupt incase u press the other buttons
	TCCR1B&=~(1<<CS10)&~(1<<CS12); // stop the clock
}

//resume button
ISR(INT2_vect){
	SREG|=(1<<7); //to enable the global interrupt incase u press the other buttons
	TCCR1B |= (1<<CS10) | (1<<CS12); // run the clock with prescalar 1024

}


void INT0_Init(void){
	DDRD&=~(1<<PD2); // configuring the int0 pin as input pin
	PORTD|=(1<<PD2); // to enable the internal pull up resistor
	GICR|=(1<<INT0); // to enable the external interuupt0
	MCUCR|=(1<<ISC01); //trigger with the falling edge
	MCUCR&=~(1<<ISC00); //trigger with the falling edge
}
void INT1_Init(void){
	DDRD&=~(1<<PD3);// configuring the int1 pin as input pin
	GICR|=(1<<INT1); // to enable the external interuupt1
	MCUCR|=(1<<ISC11)|(1<<ISC10);//trigger with the raising edge
}
void INT2_Init(void){
	DDRB&=~(1<<PB2);// configuring the int2 pin as input pin
	PORTB|=(1<<PB2);// to enable the internal pull up resistor
	GICR|=(1<<INT2);// to enable the external interuupt2
	MCUCSR&=~(1<<ISC2);// trigger with the falling edge
}

int main(){
	TIMER_configType config={TIMER1,COMPARE_MODE_TIMER1,F_CPU_1024,0,0,1000};
	TIMER1_setCallBack(TIMER1_Application);
	TIMER_init(&config);
	DDRC = 0xFF; // to make portc o/p pins
	PORTC=0xF0; // initial value to the first 4-pin (initializing with 0)
	DDRA = 0xFF;// to make portc o/p pins
	PORTA=0xC0; // initialize with 1100 0000
	SREG|=(1<<7);//enable the gloabal interrupt
	INT0_Init();
	INT1_Init();
	INT2_Init();
	while(1){
		// display the hour
		PORTC=(PORTC & 0xF0) | ((hour-(hour%10))/10);
		/*if hour is 14 so : 14%10 = 4 ..... 14-14%10 =10 ....(14-14%10)/10 = 1
		 * by this operation, we get the one and put it in the first 4 pin in portc
		 * */
		PORTA=(PORTA & 0xC0)|(1<<0); // to enable the first 7-segment
		_delay_us(1); // this is important to display the number.. without it the 7-segment will output wrong number
		PORTA=PORTA & 0xC0; // disable  the first 7-segment

		// display the hour
		PORTC=(PORTC & 0xF0) | (hour%10);
		/*same example if hour is 14 : 14%10 = 4
		 * by this operation, we get the 4 and put it in the first 4 pin in portc
		 * */
		PORTA=(PORTA & 0xC0)|(1<<1);// to enable the second 7-segment
		_delay_us(1);
		PORTA=PORTA & 0xC0;

		// display the minute
		PORTC=(PORTC & 0xF0) | ((minute-(minute%10))/10);
		PORTA=(PORTA & 0xC0)|(1<<2);
		_delay_us(1);
		PORTA=PORTA & 0xC0;

		// display the minute
		PORTC=(PORTC & 0xF0) | (minute%10);
		PORTA=(PORTA & 0xC0)|(1<<3);
		_delay_us(1);
		PORTA=PORTA & 0xC0;

		// display the second
		PORTC=(PORTC & 0xF0) | ((second-(second%10))/10);
		PORTA=(PORTA & 0xC0)|(1<<4);
		_delay_us(1);
		PORTA=PORTA & 0xC0;

		// display the second
		PORTC=(PORTC & 0xF0) | (second%10);
		PORTA=(PORTA & 0xC0)|(1<<5);
		_delay_us(1);
		PORTA=PORTA & 0xC0;

	}
}
