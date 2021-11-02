/*[FILE NAME]   : common_macros.h
 *[AUTHOR]      : AHMED IBRAHIM
 *[DATE CREATED]: Sep 29, 2021
 *[DESCRIPTION] : this file contain the common macros which will be used in all applications
 */

#ifndef COMMON_MACROS_H_
/*this line is mandatory to not include the file more than one in the same file
 * */
#define COMMON_MACROS_H_
/*To put a logic 1 in a specific bit in a register*/
#define SET_BIT(REG,BIT_NUM)   ((REG)=(REG)|((1<<(BIT_NUM)))) /*To put logic 1 inside bit */
/*To put a logic 0 in a specific bit in a register*/
#define CLEAR_BIT(REG,BIT_NUM)   ((REG)=(REG)&(~(1<<(BIT_NUM)))) /*To put logic 0 inside bit */
/*To toggle the logic  in a specific bit in a register*/
#define TOGGLE_BIT(REG,BIT_NUM)    ((REG)=( REG )^(1<<(BIT_NUM))) /*To toggle the logic of the bit */
/*To check if the bit is set (logic 1)(5v)*/
#define BIT_IS_SET(REG,BIT_NUM)  ((REG)&((1<<(BIT_NUM)))) /*To check if the bit is 1 */
/*To check if the bit is clear (logic 0)(0v)*/
#define BIT_IS_CLEAR(REG,BIT_NUM) (!((REG)&(1<<(BIT_NUM)))) /*To check if the bit is 0 */
/*To rotate the register bits values in the right direction based on the user choice*/
#define ROR(REG,BIT_NUM)  ((REG)=(((REG)>>(BIT_NUM))|((REG)<<((8*sizeof((REG)))-(BIT_NUM)))))
/*To rotate the register bits values in the left direction based on the user choice*/
#define ROL(REG,BIT_NUM)  ((REG) = (((REG)<<(BIT_NUM) )|((REG)>>((8*sizeof((REG)))-(BIT_NUM)))))
/*To check the logic of a specific bit (to make if(==0 or 1))*/
#define GET_BIT(REG,BIT_NUM)  (((REG)&(1<<(BIT_NUM)))>>(BIT_NUM))



#endif /* COMMON_MACROS_H_ */
