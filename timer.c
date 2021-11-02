/*[FILE NAME]   : timer.c
 *[AUTHOR]      : AHMED IBRAHIM
 *[DATE CREATED]: OCT 22, 2021
 *[DESCRIPTION] : Source file for Full implemented timer driver with TIMER0,TIMER1 and TIMER2
 */

#include"timer.h"
#include"common_macros.h"
#include<avr/io.h>
#include<avr/interrupt.h>

/*
 * Static   because : it is for the TIMER function only and no one can use it but timer.c can
 * Volatile because : it is used in function and the ISR
 * NULL_PTR because : it is just an initialization
 * */
static volatile void (*g_timer0CallBackPtr)(void)=NULL_PTR;
static volatile void (*g_timer1CallBackPtr)(void)=NULL_PTR;
static volatile void (*g_timer2CallBackPtr)(void)=NULL_PTR;


ISR(TIMER0_OVF_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}

ISR(TIMER0_COMP_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}

ISR(TIMER2_OVF_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		(*g_timer2CallBackPtr)();
	}
}

ISR(TIMER2_COMP_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		(*g_timer2CallBackPtr)();
	}
}

ISR(TIMER1_OVF_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		(*g_timer1CallBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		(*g_timer1CallBackPtr)();
	}
}


void TIMER_init(const TIMER_configType *Config_Ptr){
	/*In case of timer0*/
	if(Config_Ptr->timer_id == TIMER0){
		TCCR0 = (1<<FOC0);
		/* In the AVR, the 2-BITS that is responsible for choosing the mode are not in the same bit order
		 * By meaning:
		 *       WGM01 -----> BIT 3 in TCCR0
		 *       WGM00 -----> BIT 6 in TCCR0
		 *SO, we will make extra efforts to fit the mode type in those 2-bits
		 *if normal mode : WGM00=0 , WGM01=0
		 *if normal mode : WGM00=0 , WGM01=1 (So, in this case the WGM00 will ZERO)
		 *Example: if "compare mode" --> (the Config_Ptr->mode = 2) which is 0b 0000 0010
		 * we need to put the 1 in the WGM01 which in bit 3 but first we need to clear the first value
		 * of this bit that's why we made the line below
		 * if TCCR0     =    0B1111 1111
		 * Anding with(0xB7) 0B1011 0111
		 *                  -------------
		 *Result will be     0B1011 0111 (Here, we clear bit 3)
		 *-Config_Ptr->mode    = 0B0000 0010
		 *-Config_Ptr->mode<<2 = 0B0000 1000 (Here, we move the '1' in bit3)
		 *Oring with the result  0B1011 0111
		 *                      -------------
		 *                       0B1011 1111
		 * and then, we adjust the mode of operation of the timer0
		 * */
		TCCR0 = (TCCR0 & 0xB7) | (Config_Ptr->mode<<2);
		/* The next line is to adjust the prescaler
		 * */
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->prescaler);
		/*The below line is for initial value of the timer register */
		TCNT0 = (uint8)(Config_Ptr->timerStartValue);
		/* In case of normal mode, this line is not important
		 * */
		OCR0  = (uint8)(Config_Ptr->compareValue);
		/*Enabling the interrupt of overflow and compare mode in order to execute the ISR code*/
		if(Config_Ptr->mode == NORMAL_MODE){
			TIMSK |=(1<<TOIE0);
		}
		else if(Config_Ptr->mode == COMPARE_MODE){
		TIMSK |=(1<<OCIE0);
		}
	}
	/*In case of timer1*/
	else if(Config_Ptr->timer_id == TIMER1){
		/*The below line is for initial value of the timer register */
		TCNT1  = Config_Ptr->timerStartValue;
		/* To adjust the compare value
		 * In case of normal mode, this line is not important
		 */
		OCR1A  = Config_Ptr->compareValue;

		TCCR1A = (1<<FOC1A);
		TCCR1B=0;
		/*To choose the mode of operation
		 *  if mode = COMPARE_MODE_TIMER1 = 4   0b 0000 0100
		 *                        mode<<1 =      0b 0000 1000
		 *  TCCR1B       0b 0000 0111
		 *  MASK         0b 1110 0111
		 *              --------------
		 *  result       0b 0000 0111
		 *  oring
		 *  mode<<1 =    0b 0000 1000
		 *               ------------
		 *               0b 0000 1111
		 * */
		TCCR1B = (TCCR1B & 0xE7) | (Config_Ptr->mode<<1);
		/* The next line is to adjust the prescaler
		 * */
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);
		/*Enabling the interrupt of overflow and compare mode in order to execute the ISR code*/
		if(Config_Ptr->mode == NORMAL_MODE){
			TIMSK |=(1<<TOIE1);
		}
		else if(Config_Ptr->mode == COMPARE_MODE_TIMER1){
			TIMSK |=(1<<OCIE1A);
		}

	}
	/*In case of timer2*/
	else if(Config_Ptr->timer_id == TIMER2){
		TCCR2 = (1<<FOC2);
		/* In the AVR, the 2-BITS that is responsible for choosing the mode are not in the same bit order
		 * By meaning:
		 *       WGM21 -----> BIT 3 in TCCR0
		 *       WGM20 -----> BIT 6 in TCCR0
		 *SO, we will make extra efforts to fit the mode type in those 2-bits
		 *if normal mode : WGM20=0 , WGM21=0
		 *if normal mode : WGM20=0 , WGM21=1 (So, in this case the WGM00 will ZERO)
		 *Example: if "compare mode" --> (the Config_Ptr->mode = 2) which is 0b 0000 0010
		 * we need to put the 1 in the WGM21 which in bit 3 but first we need to clear the first value
		 * of this bit that's why we made the line below
		 * if TCCR0     =    0B1111 1111
		 * Anding with(0xB7) 0B1011 0111
		 *                  -------------
		 *Result will be     0B1011 0111 (Here, we clear bit 3)
		 *-Config_Ptr->mode    = 0B0000 0010
		 *-Config_Ptr->mode<<2 = 0B0000 1000 (Here, we move the '1' in bit3)
		 *Oring with the result  0B1011 0111
		 *                      -------------
		 *                       0B1011 1111
		 * and then, we adjust the mode of operation of the timer0
		 * */
		TCCR2 = (TCCR2 & 0xB7) | (Config_Ptr->mode<<2);
		/* The next line is to adjust the prescaler
		 * */
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->prescaler2);
		TCNT2 = (uint8)(Config_Ptr->timerStartValue);
		/* In case of normal mode, this line is not important
		 * */
		OCR2  = (uint8)(Config_Ptr->compareValue);
		/*Enabling the interrupt of overflow and compare mode in order to execute the ISR code*/
		if(Config_Ptr->mode == NORMAL_MODE){
			TIMSK |=(1<<TOIE2);
		}
		else if(Config_Ptr->mode == COMPARE_MODE){
		TIMSK |=(1<<OCIE2);
		}
	}

}



void TIMER0_setCallBack(void (*a_ptr)(void)){
	/*a_ptr is local variable so it will be deleted after quiting the function and we need to save the
	 * value of function address and that can be done by putting its value inside global pointer to function
	 * */
	g_timer0CallBackPtr=a_ptr;
}

void TIMER1_setCallBack(void (*a_ptr)(void)){
	/*a_ptr is local variable so it will be deleted after quiting the function and we need to save the
	 * value of function address and that can be done by putting its value inside global pointer to function
	 * */
	g_timer1CallBackPtr=a_ptr;
}

void TIMER2_setCallBack(void (*a_ptr)(void)){
	/*a_ptr is local variable so it will be deleted after quiting the function and we need to save the
	 * value of function address and that can be done by putting its value inside global pointer to function
	 * */
	g_timer2CallBackPtr=a_ptr;
}

void TIMER_DeInit(TIMER_ID timer_id){
	/*Based on Timer ID , we will de-initializing the timer*/
	/*In case of timer0*/
	if(timer_id == TIMER0){
		/*Here, we will put zero in all timer0 registers */
	    TCCR0=0;
	    TCNT0=0;
	    OCR0=0;
	    /*disable the interrupt module of timer0*/
	    TIMSK &=~(1<<TOIE0) &~(1<<OCIE0);
	}
	/*In case of timer1*/
	else if(timer_id == TIMER1){
		/*Here, we will put zero in all timer0 registers */
	    TCCR1A=0;
	    TCCR1B=0;
	    TCNT1=0;
	    OCR1A=0;
	    /*disable the interrupt module of timer1*/
	    TIMSK &=~(1<<TOIE1) &~(1<<OCIE1A);
	}
	/*In case of timer2*/
	else if(timer_id == TIMER2){
		/*Here, we will put zero in all timer0 registers */
     	TCCR2=0;
     	TCNT2=0;
    	OCR2=0;
    	/*disable the interrupt module of timer2*/
	    TIMSK &=~(1<<TOIE2) &~(1<<OCIE2);
	}
}

