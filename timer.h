/*[FILE NAME]   : timer.h
 *[AUTHOR]      : AHMED IBRAHIM
 *[DATE CREATED]: OCT 22, 2021
 *[DESCRIPTION] : Header file for Full implemented timer driver with TIMER0,TIMER1 and TIMER2
 */

#ifndef TIMER_H_
#define TIMER_H_

#include"std_types.h"
/*AVR has 3-timers so we have 3-ID for each timer
 * TIMER0 = 0
 * TIMER1 = 1
 * TIMER2 = 2
 * */
typedef enum{
	TIMER0,TIMER1,TIMER2
}TIMER_ID;

/* we have 2-modes which are normal and compare mode but TIMER1 mode number is different from timer0 and timer2
 * so, we made the third element of the enum for the timer1
 * COMPARE_MODE_TIMER1 = 4   0b 0000 0100
 * Why again ??
 * --> In TIMER0 & TIMER1 --> the normal and compare mode are 0 and 2 but
 *     In TIMER1 --> the normal mode is 0 as the other timers but the cmpare mode is 4 so we need to add
 *                   another element in the enum
 * Why we make 0,2 and 4 ??
 * --> To make the coding easily by just fit the number into the specified bits
 * */
typedef enum{
	NORMAL_MODE,COMPARE_MODE=2,COMPARE_MODE_TIMER1=4
}TIMER_modeType;

/* TIMER0 and TIMER1 have the same table in the data sheet for the prescaler selection but unfortunately
 * TIMER2 don't that why we made 2 typedef enum belows. one for timer2 only and the other for timer0 and timer1 both
 * (timer0 & timer1)---> page 85 & 113 in data sheet
 *     (timer2)     ---> page 130
 * */
typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXT_CLOCK_FALLING_EDGE,EXT_CLOCK_RISING_EDGE
}TIMER_prescaler;

typedef enum{
	NO2_CLOCK,F2_CPU_CLOCK,F2_CPU_8,F2_CPU_32,F2_CPU_64,F2_CPU_128,F2_CPU_256,F2_CPU_1024
}TIMER2_prescaler;

/* Structure members :
 *  (TIMER_ID timer_id)          ---> to select between Timers
 *  (TIMER_modeType mode)        ---> to choose which mode of operation (normal or compare)
 *  (TIMER_prescaler prescaler)  ---> to choose prescaler of TIMER0 or TIMER1 (it will not affect TIMER2)
 *  (TIMER2_prescaler prescaler2)---> to choose prescaler of TIMER2 in case we want to use TIMER2
 *  (uint16 timerStartValue)     ---> to set the initial value of the timer register
 *                                    in case of timer0 and timer1 we will make casting to uint8 because they are 8-bits timer
 *	(uint16 compareValue)        ---> to set the compare value in case we use the compare mode
 *	                                  in case of timer0 and timer1 we will make casting to uint8 because they are 8-bits timer
 *	why uint16 ??
 *	--> if we use timer1, it is 16-bit register in wide so we need uint16. if we use TIMER0 and TIMER2, we need
 *	take care because they are 8-bit register in wide so we need to make cast to uint8.
 * */

typedef struct{
	TIMER_ID timer_id;
	TIMER_modeType mode;
	TIMER_prescaler prescaler;
	TIMER2_prescaler prescaler2;
	uint16 timerStartValue;
	uint16 compareValue;
}TIMER_configType;



/*
 * Description :
 * Function responsible for initializing the timer
 */
void TIMER_init(const TIMER_configType *Config_Ptr);
/*
 * Description :
 * Function responsible for setting the call back function of timer0 if the Interrupt happened
 */
void TIMER0_setCallBack(void (*a_ptr)(void));
/*
 * Description :
 * Function responsible for setting the call back function of timer1 if the Interrupt happened
 */
void TIMER1_setCallBack(void (*a_ptr)(void));
/*
 * Description :
 * Function responsible for setting the call back function of timer2 if the Interrupt happened
 */
void TIMER2_setCallBack(void (*a_ptr)(void));
/*
 * Description :
 * Function responsible for de-initializing the timer
 * 	This function takes the timer id to de-init the timer based on its id
 */
void TIMER_DeInit(TIMER_ID timer_id);




#endif /* TIMER_H_ */
