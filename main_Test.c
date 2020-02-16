

/**
 * main.c
 */
#define     RCGC0               (*(volatile uint32*)(0x400FE100))
#define     GPIOAFSEL_PORTB     (*(volatile uint32*)(0x40005420))
#define     GPIOAFSEL_PORTA     (*((volatile uint32*)0x40004420))
#define     GPIOPCTL_PORTB      (*(volatile uint32*)(0x4000552C))
#define     GPIOPCTL_PORTA      (*((volatile uint32*)0x4000452C))
#define     RCGCGPIO            (*((volatile uint32*)0x400FE608))        //enable clock for the GPIO
#define     RCC                 (*((volatile uint32*)0x400FE060))       //prescalar to main freq
#define     RCC2                (*((volatile uint32*)0x400FE070))       //prescalar to main freq


#include"Can.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

VAR(extern Can_HwType,CanCODE)         Mailbox;
VAR(extern PduInfoType,CanCODE)        PduInfo;

int main(void)
{
    set_bit(CFGCTRL,1);


    /*Data */
    uint8 Data[2] ;
    uint8 MetaData =0;
    VAR(extern uint8,CanCODE)                    SDU_Data[8]; //the total data
    VAR(extern uint8,CanCODE)                    SDU_MetaData; //the meta data

    /********************/
    /*Set clock to GPIO PORT B*/

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //PB4   -> CAN0Rx       PB5   -> CAN0Tx
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    /*Choosing the Alternative function as CAN0Rx and CAN0Tx*/
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    /*Set clock to CAN0 and CAN1*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    /*********************************************************/
    //    /*Can1 Configuration*/
    //    /*Set clock to GPIO PORT A*/
    //    RCGCGPIO|=1;
    //    //CAN1Rx    PA0
    //    //CAN1Tx    PA1
    //    /*choose alternative functions for PA0 - PA1*/
    //    set_bit(GPIOAFSEL_PORTA,0);
    //    set_bit(GPIOAFSEL_PORTA,1);
    //    /*Choosing the Alternative function as CAN1Rx and CAN1Tx*/
    //    GPIOPCTL_PORTA=0x88;      //PA0   -> CAN1Rx       PA1   -> CAN1Tx
    /*********************************************************/
    /*Setting the Clock for the system*/

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);


    /************************************/
    /*Can initialization*/
    VAR( Can_ConfigType,Can_CODE) Configuration;
    Configuration.Can_Controller=Can_Controller0;
    Configuration.Lenght=1;
    Configuration.Hth=15;
    P2CONST(Can_ConfigType,Can_CODE,Can_CONST) Configuration_Ptr=&Configuration;
    Can_Init(Configuration_Ptr);
    Can_SetControllerMode(Can_Controller0,CAN_CS_STARTED);
    //Can_EnableControllerInterrupts(Can_Controller0);
    /*
    uint8 x[2]={143,97};
    uint8 * Ptr=x;
    VAR( Can_PduType,Can_CODE) PDU_Information;
    PDU_Information.id=25;
    PDU_Information.length=2;
    PDU_Information.sdu=Ptr;
    PDU_Information.swPduHandle=4;
    P2CONST(Can_PduType,Can_CODE,Can_CONST) PDU_Information_Ptr=&PDU_Information;
    Can_Write(3,PDU_Information_Ptr );


    /**********************/
    /*
uint32 counter=0;
    for(counter=0;counter<32;counter++){

    Can_Write(counter,PDU_Information_Ptr );
    }
  /****************************************/

    while(1){
        // CANMessageGet();
    }

    return 0;
}

/*Driver State Machine*/

//***For Can module (CAN_UNINIT,CAN_READY)

/*
Can_Init()      CAN_UNINIT     =>     CAN_READY
Can_DeInit()    CAN_READY      =>     CAN_UNINIT
 */

//***For Can Controller (UNINIT, STOPPED, STARTED and SLEEP)

/*
two possibilities for triggering state changes by external events:

 * Bus-off event
 * HW wakeup event

 */

/******************************************/

/******************How can driver should work************************/

/*
 *The ECU State Manager module shall call Can_Init at most once during runtime.

 *The function Can_Init shall initialize all CAN controllers according to their configuration
  (Hardware register settings that have impact on all CAN controllers inside the HW Unit can only be set in the function Can_Init.)
   and keep the Can controller in the initialization mode to keep it in the Stop state

 * in case of using interrupt mode should call the function    Can_EnableControllerInterrupts(Can_Controller0);

 * then set the BaudRate using the function Can_SetBaudrate while in Stop state

 *CAN controller must then be started separately by calling the function Can_SetControllerMode(CAN_CS_STARTED).
    by exiting the initialization mode and entering the normal mode

 */
/******************************************/


/********************Notes************************/

/*
 * Sleep mode is for the whole controller and if not supported you simulate it by changing to stop state
 * after successful transmission of state use the callback function CanIf_ControllerModeIndication()
 * Can_Init  change the state from   UNINIT => STOPPED
 * Can_SetBaudrate  STOPPED -> STOPPED; SLEEP -> SLEEP; STARTED -> STARTED
 * If the call of Can_SetBaudrate() would cause a re-initialization then the controller should be stopped
 * Can_SetControllerMode shall use the system service GetCounterValue for timeout monitoring to avoid blocking functions
 * Can_SetControllerMode if change don't tack effect for the time CanTimeoutDuration then this function should be left
   and Can_Mainfunction_Mode should Poll the Flag if successful transmission this function should call CanIf_ControllerModeIndication()
 *to leave the sleeping state the Can_SetControllerMode (CAN_CS_STOPPED) (to exit sleep use stop)
 *On hardware wakeup the Can module shall call the function EcuM_CheckWakeup either in interrupt context
  or in the context of Can_MainFunction_Wakeup.

 *[SWS_Can_00056] Post-Build configuration elements
  that are marked as ‘multiple’ (‘M’ or ‘x’) in chapter 10 can be selected by passing the pointer ‘Config’
  to the init function of the module.
 *The CAN driver shall support controllers which implement a hardware FIFO. The size of the FIFO is configured via "CanHwObjectCount
 *
 **/

/**************************************************/
