/*
 * Can_Reg.h
 *
 *  Created on: Jan 28, 2020
 *      Author: OWNER
 */

#ifndef CAN_REG_H_
#define CAN_REG_H_

//Controller CAN controller, whose baud rate shall be set whether
#define Can_Controller0         0x00u
#define Can_Controller1         0x01u

#define Controller1_BaseAddress     0x40040000
#define Controller2_BaseAddress     0x40041000

/*****************Registers**********************/
#if CanControllerBaseAddress ==  Controller1_BaseAddress

#define CANCTL                  (*(volatile uint32*)(0x40040000))
#define CANSTS                  (*(volatile uint32*)(0x40040004))
#define CANERR                  (*(volatile uint32*)(0x40040008))
#define CANBIT                  (*(volatile uint32*)(0x4004000C))
#define CANINT                  (*(volatile uint32*)(0x40040010))
#define CANTST                  (*(volatile uint32*)(0x40040014))
#define CANBRPE                 (*(volatile uint32*)(0x40040018))
#define CANIF1CRQ               (*(volatile uint32*)(0x40040020))
#define CANIF1CMSK              (*(volatile uint32*)(0x40040024))
#define CANIF1MSK1              (*(volatile uint32*)(0x40040028))
#define CANIF1MSK2              (*(volatile uint32*)(0x4004002C))
#define CANIF1ARB1              (*(volatile uint32*)(0x40040030))
#define CANIF1ARB2              (*(volatile uint32*)(0x40040034))
#define CANIF1MCTL              (*(volatile uint32*)(0x40040038))
#define CANIF1DA1               (*(volatile uint32*)(0x4004003C))
#define CANIF1DA2               (*(volatile uint32*)(0x40040040))
#define CANIF1DB1               (*(volatile uint32*)(0x40040044))
#define CANIF1DB2               (*(volatile uint32*)(0x40040048))
#define CANIF2CRQ               (*(volatile uint32*)(0x40040080))
#define CANIF2CMSK              (*(volatile uint32*)(0x40040084))
#define CANIF2MSK1              (*(volatile uint32*)(0x40040088))
#define CANIF2MSK2              (*(volatile uint32*)(0x4004008C))
#define CANIF2ARB1              (*(volatile uint32*)(0x40040090))
#define CANIF2ARB2              (*(volatile uint32*)(0x40040094))
#define CANIF2MCTL              (*(volatile uint32*)(0x40040098))
#define CANIF2DA1               (*(volatile uint32*)(0x4004009C))
#define CANIF2DA2               (*(volatile uint32*)(0x400400A0))
#define CANIF2DB1               (*(volatile uint32*)(0x400400A4))
#define CANIF2DB2               (*(volatile uint32*)(0x400400A8))
#define CANTXRQ1                (*(volatile uint32*)(0x40040100))
#define CANTXRQ2                (*(volatile uint32*)(0x40040104))
#define CANNWDA1                (*(volatile uint32*)(0x40040120))
#define CANNWDA2                (*(volatile uint32*)(0x40040124))
#define CANMSG1INT              (*(volatile uint32*)(0x40040140))
#define CANMSG2INT              (*(volatile uint32*)(0x40040144))
#define CANMSG1VAL              (*(volatile uint32*)(0x40040160))
#define CANMSG2VAL              (*(volatile uint32*)(0x40040164))

#elif CanControllerBaseAddress == Controller2_BaseAddress

#define CANCTL                  (*(volatile uint32*)(0x40041000))
#define CANSTS                  (*(volatile uint32*)(0x40041004))
#define CANERR                  (*(volatile uint32*)(0x40041008))
#define CANBIT                  (*(volatile uint32*)(0x4004100C))
#define CANINT                  (*(volatile uint32*)(0x40041010))
#define CANTST                  (*(volatile uint32*)(0x40041014))
#define CANBRPE                 (*(volatile uint32*)(0x40041018))
#define CANIF1CRQ               (*(volatile uint32*)(0x40041020))
#define CANIF1CMSK              (*(volatile uint32*)(0x40041024))
#define CANIF1MSK1              (*(volatile uint32*)(0x40041028))
#define CANIF1MSK2              (*(volatile uint32*)(0x4004102C))
#define CANIF1ARB1              (*(volatile uint32*)(0x40041030))
#define CANIF1ARB2              (*(volatile uint32*)(0x40041034))
#define CANIF1MCTL              (*(volatile uint32*)(0x40041038))
#define CANIF1DA1               (*(volatile uint32*)(0x4004103C))
#define CANIF1DA2               (*(volatile uint32*)(0x40041040))
#define CANIF1DB1               (*(volatile uint32*)(0x40041044))
#define CANIF1DB2               (*(volatile uint32*)(0x40041048))
#define CANIF2CRQ               (*(volatile uint32*)(0x40041080))
#define CANIF2CMSK              (*(volatile uint32*)(0x40041084))
#define CANIF2MSK1              (*(volatile uint32*)(0x40041088))
#define CANIF2MSK2              (*(volatile uint32*)(0x4004108C))
#define CANIF2ARB1              (*(volatile uint32*)(0x40041090))
#define CANIF2ARB2              (*(volatile uint32*)(0x40041094))
#define CANIF2MCTL              (*(volatile uint32*)(0x40041098))
#define CANIF2DA1               (*(volatile uint32*)(0x4004109C))
#define CANIF2DA2               (*(volatile uint32*)(0x400410A0))
#define CANIF2DB1               (*(volatile uint32*)(0x400410A4))
#define CANIF2DB2               (*(volatile uint32*)(0x400410A8))
#define CANTXRQ1                (*(volatile uint32*)(0x40041100))
#define CANTXRQ2                (*(volatile uint32*)(0x40041104))
#define CANNWDA1                (*(volatile uint32*)(0x40041120))
#define CANNWDA2                (*(volatile uint32*)(0x40041124))
#define CANMSG1INT              (*(volatile uint32*)(0x40041140))
#define CANMSG2INT              (*(volatile uint32*)(0x40041144))
#define CANMSG1VAL              (*(volatile uint32*)(0x40041160))
#define CANMSG2VAL              (*(volatile uint32*)(0x40041164))
#endif
/**************************************/

#define CANCTL_Controller2                  (*(volatile uint32*)(0x40041000))
#define CANBIT_Controller2                  (*(volatile uint32*)(0x4004100C))
#define CANCTL_Controller1                  (*(volatile uint32*)(0x40040000))
#define CANBIT_Controller1                  (*(volatile uint32*)(0x4004000C))
/*NVIC Register*/
#define NVIC_EN1                            (*((volatile uint32 *)((0xE000E104)))) // Interrupt 32-63 Set Enable
#define NVIC_DIS1                           (*((volatile uint32 *)((0xE000E180)))) // Interrupt 32-63 Set Disable
#define CFGCTRL                             (*((volatile uint32 *)((0xE000ED14))))
/*******************/
#define CANIF2DA1_Addr               (*( uint32*)(0x4004009C))

#endif /* CAN_REG_H_ */
