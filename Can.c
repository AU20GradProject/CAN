#include"Can.h"
#include"UTILES.h"
#include"CanIf_Cbk.h"
#include "Det.h"


/*Necessary Global variables*/

VAR(static uint8,Can_CODE) Can_HTHMutex = NumberOfHTH;        //HTH Hardware Transmit handle

VAR(static Can_ControllerStateType,Can_CODE) Can_Controller0_Statues = CAN_CS_UNINIT;   //Controller 1 State
VAR(static Can_ControllerStateType,Can_CODE) Can_Controller1_Statues = CAN_CS_UNINIT;   //Controller 2 State

VAR( Can_ConfigType,Can_CODE) Configurations;
VAR(static BaudeRate_ConfigType,Can_CODE) BaudeRate_Config[Max_BaudeRateIDs];    //For the Post linking configuration
VAR(static uint16,Can_CODE)Can_InterruptsCounter=1;
VAR(extern uint32,Can_CODE) Can_StatuesReg;        //use variable because this register is read sensitive
VAR(extern boolean,Can_CODE) Error_Flag;             //Error Flag

//POLLING Flages

#if CanTxProcessing==POLLING
VAR(static boolean,Can_CODE)TX_Flag=FALSE ;             //Successful transmission flag
#endif
#if CanRxProcessing==POLLING
VAR(static boolean,Can_CODE)RX_Flag=FALSE ;             //Successful Reception flag
#endif
#if CanBusoffProcessing==POLLING
VAR(static boolean,Can_CODE)BusOff_Flag=FALSE ;             //Bus Off flag
#endif
#if CanWakeupProcessing == POLLING
VAR(static boolean,Can_CODE)Wakeup_Flag=FALSE ;             //Wakeup Flage
#endif

/***************************************/










/*Semaphores and Mutex control Functions*/

FUNC(static void,Can_CODE) Mutex_Wait(uint8 Mutex){
    while(Mutex<=0);
    Mutex--;
}
FUNC(static void,Can_CODE) Mutex_Signal(uint8 Mutex){
    Mutex++;
}
/*******************************/










/***********************************************************************************************************/
/*API IDs*/

#define Can_Init_ID                             0x00
#define Can_DeInit_ID                           0x10
#define Can_SetBaudrate_ID                      0x0f
#define Can_SetControllerMode_ID                0x03
#define Can_DisableControllerInterrupts_ID      0x04
#define Can_EnableControllerInterrupts_ID       0x05
#define Can_CheckWakeup_ID                      0x0b
#define Can_GetControllerErrorState_ID          0x0b
#define Can_GetControllerMode_ID                0x12
#define Can_Write_ID                            0x06


/*Can Driv Errors*/

/*Development Errors*/

#define   CAN_E_PARAM_POINTER                   0x01
#define   CAN_E_PARAM_HANDLE                    0x02
#define   CAN_E_PARAM_DATA_LENGTH               0x03
#define   CAN_E_PARAM_CONTROLLER                0x04
#define   CAN_E_UNINIT                          0x05
#define   CAN_E_TRANSITION                      0x06
#define   CAN_E_PARAM_BAUDRATE                  0x07
#define   CAN_E_ICOM_CONFIG_INVALID             0x08
#define   CAN_E_INIT_FAILED                     0x09

/*Runtime Errors*/

#define    CAN_E_DATALOST                        0x01

/***********************************************************************************************************/













//This function initializes the module.
FUNC(void, Can_CODE) Can_Init( const Can_ConfigType* Config ){




#if CanControllerBaseAddress == Controller1_BaseAddress

    if(Can_Controller0_Statues!=CAN_CS_UNINIT){

#if CanDevErrorDetect==STD_ON

        /*implementation of the DeT*/

        Det_ReportError(Can_MODULE_ID, 0, Can_Init_ID, CAN_E_TRANSITION);

#endif


    }


#elif CanDevErrorDetect == Controller2_BaseAddress


    if(Can_Controller1_Statues!=CAN_CS_UNINIT){

        return 0;

    }

#endif

    Configurations = *(Config);


    /*setting the INIT bit in the CAN Control (CANCTL) register stopping all bus activities */

    set_bit(CANCTL,0); //Initialization started

    while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

    /*****************************************************************************************************/

    /*Initialize all the memory objects*/

    set_bit(CANIF1CMSK,7);      //WRND
    set_bit(CANIF1CMSK,5);      //ARB
    set_bit(CANIF1CMSK,4);      //Control

    /*CLear registers for Initilizations*/

    CANIF1ARB2=0;
    CANIF1MCTL=0;

    /***********************************/

    // Loop through to program all 32 message objects

    VAR(static uint8,Can_CODE)MessageCounter=0;

    for(MessageCounter=1;MessageCounter<=32;MessageCounter++){

        while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

        // Initiate programming the message object

        CANIF1CRQ=MessageCounter;

    }

    /*Make sure that the interrupt and new data flags are updated for the message objects.*/

    set_bit(CANIF1CMSK,2);      //NewData
    set_bit(CANIF1CMSK,3);      //Clear the interrupt bending bit

    // Loop through to program all 32 message objects

    for(MessageCounter=1;MessageCounter<=32;MessageCounter++){

        /*CAN Data registers initially is zero so looping through memory objects will just clear them*/

        while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

        // Initiate programming the message object

        CANIF1CRQ=MessageCounter;
        CANIF1MSK1=0;
        CANIF1MSK2=0;
        CANIF1DA1=0;
        CANIF1DA2=0;
        CANIF1DB1=0;
        CANIF1DB2=0;
    }
    /*****************************************************************************************************/

    /***********************************/

    /*Baud Rate and Timming Configuration*/

    /* Time Quantum (tq) => tq = BRP / fsys(system frequency)   where fsys is CanCpuClockRef
      ________________________________________
      | CANBIT      |   Register Field Setting  |
      |-----------------------------------------|
      | TSEG2       |   Phase2 - 1              |
      | TSEG1       |   Prop + Phase1 - 1       |
      | SJW         |   SJW - 1                 |
      | BRP         |   Baud rate - 1           |
      |_____________|___________________________|

     *Bit length=[TSEG1 + TSEG2 + 3] × tq OR Bit length=[Sync + Prop + Phase1 + Phase2] × tq
     */

    // Initialize Baud rate and timing

    set_bit(CANCTL,0); //Initialization started
    set_bit(CANCTL,6);  //Configuration Change Enable

    //calculating the Prescaler for the baud rate

    VAR(uint32,Can_CODE) Prescaler = (CanCpuClockRef/(CanControllerBaudRate*1000));

    CANBIT|=((Prescaler-1)&0x3F);   //Setting the Prescaler in the BRP section of the register, BRP= Baud rate prescaler - 1

    //Setting Time Segment after Sample Point

    CANBIT|=((CanControllerSeg2-1)&0x7000);       //TSEG2=Time Segment after Sample Point-1   in quants

    //Setting Time Segment before Sample Point

    CANBIT|=((CanControllerSeg1-1)&0xF00);       //TSEG1=Time Segment after Sample Point-1  in quants

    //Setting   (Re)Synchronization Jump Width

    CANBIT|=((CanControllerSyncJumpWidth-1)&0xC0);       //SJW=(Re)Synchronization Jump Width-1     in quants

    /********************************************/


    /*Enable interrupts*/

    set_bit(CANCTL,1);      //General Can interrupt
    set_bit(CANCTL,3);      //Error Interrupt
    set_bit(CANCTL,2);      //Statues Interrupt

    /***********************************/


    /***********************************/

    /*Initialize message objects as not valid*/

    reset_bit(CANIF1ARB2,15);   //MSGVAL = 0

    /***********************************/

    /* 1 ) In the CAN IFn Command Mask (CANIFnCMASK) register:*/

    //Set the WRNRD bit to specify a Write on the memory object to intialize it

    set_bit(CANIF1CMSK,7);

    /*Specify which bits to transfer using the and bits DATAA DATAB*/

    //Set the Pin DATAA to allow the use Data A registers

    set_bit(CANIF1CMSK,1);

    //Set the Pin DATAB to allow the use Data B registers

    set_bit(CANIF1CMSK,0);

    //Set the Control access pin

    set_bit(CANIF1CMSK,4);

    /* This the initial state for the registers.*/

    CANIF1ARB1=0;
    CANIF1ARB2=0;
    CANIF1MCTL=0;
    CANIF1MSK1=0;
    CANIF1MSK2=0;

    /*set the DIR bit to indicate receive*/

    reset_bit(CANIF1ARB2,13);

    //specify whether to transfer the IDMASK, DIR, and MXTD of the message object into the CAN IFn

    reset_bit(CANIF1CMSK,6);

    /*Specify whether to transfer the ID, DIR, XTD, and MSGVAL of the message object into the interface registers*/

    reset_bit(CANIF1CMSK,5);

    /*Specify whether to clear the bit in the CANIFnMCTL register using the INTPND CLRINTPND bit*/

    reset_bit(CANIF1CMSK,3);

    /*Specify whether to clear the bit in the CANNWDAn register using the bit NEWDAT NEWDAT*/

    reset_bit(CANIF1CMSK,2);


    /* 2 ) Specify which of the bits are used for acceptance filtering*/


#if CanIdType==STANDARD //IDs is 11bit

    CANIF1MSK1=0;
    CANIF1MSK2|=((uint32)((CanHwFilterMask)&(0x7FF))<<2);

#elif   CanIdType==EXTENDED     //IDs is 29bit

    CANIF1MSK1|=((uint32)((CanHwFilterMask)&(0xFFFF)));
    CANIF1MSK2|=((uint32)((CanHwFilterMask)&(0x1FFF>>16)));

#endif

    /*Enable the Use of Acceptance Mask*/

    set_bit(CANIF1MCTL,12);

    /*Enable the Access to mask bits*/

    set_bit(CANIF1CMSK,6);

    /*Enable the Access Arbitration Bits*/

    set_bit(CANIF1CMSK,5);

    /* 3 ) Use the MXTD and MDIR bits to specify whether to use XTD and DIR for acceptance filtering.*/


    /*The extended identifier bit XTD is used for acceptance filtering.*/

    reset_bit(CANIF1MSK2,15);

    /*The message direction bit DIR is used for acceptance filtering.*/

    reset_bit(CANIF1MSK2,14);

    /* 4 ) Set the Id and the Type as receive memory object*/

#if CanIdType==STANDARD //IDs is 11bit

    reset_bit(CANIF1ARB2,14);
    CANIF1ARB2|=((uint32)((CanObjectId)&(0x1FFF))<<2);

#elif   CanIdType==EXTENDED     //IDs is 29bit

    set_bit(CANIF1ARB2,14);
    CANIF1ARB1|=((uint32)((CanObjectId)&(0xFFFF)));
    CANIF1ARB2|=((uint32)((CanObjectId)&(0xFFF>>16)));

#endif

    /*set the MSGVAL bit to indicate message object valid*/

    set_bit(CANIF1ARB2,15);

    /* 5 ) CANIFnMCTL Register  */

    /*specify the size of the data frame in bytes.*/

    CANIF1MCTL|=((Config->Lenght)&0xF);

    /*End of Buffer => Single message object*/

    set_bit(CANIF1MCTL,7);

    /*Receive Interrupt Enable*/

    set_bit(CANIF1MCTL,10);

    /*Disable the use of remote frame */

    reset_bit(CANIF1MCTL,9);

    /*Starting the Can In Reading state*/

    /* 6 ) Choosing the memory object to be recieved in*/

    CANIF1CRQ|=(Config->Hth)&(0x3F);

    /*Wait for busy bit to clear*/

    while(get_bit(CANIF1CRQ,15)==1);

    /*****************************/

#if CanControllerBaseAddress == Controller1_BaseAddress

    Can_Controller0_Statues = CAN_CS_STOPPED;

#elif CanControllerBaseAddress == Controller2_BaseAddress

    Can_Controller1_Statues = CAN_CS_STOPPED;

#endif
}
/*************************/














//This function de-initializes the module.
FUNC(void, Can_CODE) Can_DeInit( void ){


    if(Can_Controller0_Statues==CAN_CS_STARTED){
#if CanDevErrorDetect==STD_ON

        /*implementation of the DeT*/
        Det_ReportError(Can_MODULE_ID, 0, Can_DeInit_ID, CAN_E_TRANSITION);
#endif
    }else{
        /*setting the INIT bit in the CAN Control (CANCTL) register stopping all bus activities */

        set_bit(CANCTL,0); //Initialization started

        while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

        /*****************************************************************************************************/

        /*Initialize all the memory objects*/

        set_bit(CANIF1CMSK,7);      //WRND
        set_bit(CANIF1CMSK,5);      //ARB
        set_bit(CANIF1CMSK,4);      //Control

        /*CLear registers for Initilizations*/

        CANIF1ARB2=0;
        CANIF1MCTL=0;

        /***********************************/

        // Loop through to program all 32 message objects

        VAR(static uint8,Can_CODE)MessageCounter=0;

        for(MessageCounter=1;MessageCounter<=32;MessageCounter++){

            while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

            // Initiate programming the message object

            CANIF1CRQ=MessageCounter;

        }

        /*Make sure that the interrupt and new data flags are updated for the message objects.*/

        set_bit(CANIF1CMSK,2);      //NewData
        set_bit(CANIF1CMSK,3);      //Clear the interrupt bending bit

        // Loop through to program all 32 message objects

        for(MessageCounter=1;MessageCounter<=32;MessageCounter++){

            /*CAN Data registers initially is zero so looping through memory objects will just clear them*/

            while(get_bit(CANIF1CRQ,15)==1);    //Wait for busy bit to clear

            // Initiate programming the message object

            CANIF1CRQ=MessageCounter;
            CANIF1MSK1=0;
            CANIF1MSK2=0;
            CANIF1DA1=0;
            CANIF1DA2=0;
            CANIF1DB1=0;
            CANIF1DB2=0;
        }
        /*****************************************************************************************************/

        Can_Controller0_Statues = CAN_CS_STOPPED;

        set_bit(CANCTL,0); //Initialization started
        set_bit(CANCTL,6);  //Configuration Change Enable

        //De-Initialize Baud rate and timing

        CANBIT|=((0x1)&0x3F);   //Resetting the Prescaler in the BRP section of the register, BRP Initial value ix 0x1

        //Resetting Time Segment after Sample Point

        CANBIT|=((0x2)&0x7000);       //TSEG2 Initial value is 0x2

        //Setting Time Segment before Sample Point

        CANBIT|=((0x3)&0xF00);       //TSEG1 Initial value is 0x3

        //Setting   (Re)Synchronization Jump Width

        CANBIT|=((0x0)&0xC0);       //SJW Initial value is 0x0

        /***********************************/
        /*De-initialize message objects as not valid*/

        reset_bit(CANIF1ARB2,15);   //MSGVAL = 0
        reset_bit(CANIF2ARB2,15);   //MSGVAL = 0

        /***********************************/

        reset_bit(CANIF1ARB2,15);   //MSGVAL = 0
        reset_bit(CANIF2ARB2,15);   //MSGVAL = 0

        /*Exit the Initialization state*/

        reset_bit(CANCTL,0); //Normal oberation

        /***********************************/

        Can_Controller0_Statues = CAN_CS_UNINIT;
    }
}
/*************************/














/*This service shall set the baud rate configuration of the CAN controller(Can_Controller0,Can_Controller1)
   BaudRateConfigID references a baud rate configuration by ID  CanControllerBaudRateConfigID
   Uniquely identifies a specific baud rate configuration. This ID is used by SetBaudrate API.
   If the call of Can_SetBaudrate will cause a re-initialization of the CAN
    Controller the CAN controller must be in state STOPPED when this function is
    called
 */

#if CanSetBaudrateApi==STD_ON

FUNC(Std_ReturnType,Can_CODE) Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )	{


#if CanDevErrorDetect ==STD_ON
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        /*implementation of the DeT*/

        Det_ReportError(Can_MODULE_ID, 0, Can_SetBaudrate_ID, CAN_E_UNINIT);

    }

    else if(BaudRateConfigID>Max_BaudeRateIDs){

        Det_ReportError(Can_MODULE_ID, 0, Can_SetBaudrate_ID, CAN_E_PARAM_BAUDRATE);

    }
    else if(Controller>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_SetBaudrate_ID, CAN_E_PARAM_CONTROLLER);

    }
#endif



    if(Can_Controller0_Statues== CAN_CS_STOPPED){

#if CanControllerBaseAddress ==  Controller1_BaseAddress

        VAR(uint16,Can_CODE)Counter=0;

        VAR(uint32,Can_CODE) Prescaler;

        switch(Controller){

        case Can_Controller0:

            set_bit(CANCTL,0); //Initialization started
            set_bit(CANCTL,6);  //Configuration Change Enable

            // Change Baud rate and timing

            //this for loop to find the configurations of the given BaudeRate ID

            for(Counter=0;((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID)&&Counter<Max_BaudeRateIDs);Counter++){

                //Counter after the for loop is the the number of the structure that hold the baud rate configuration
            }

            //if the given BaudRate Configuration ID isn't correct return error

            if((Counter==Max_BaudeRateIDs-1)&&((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID))){

                return E_NOT_OK;
            }

            //calculating the Prescaler for the baud rate

            Prescaler = (CanCpuClockRef/(BaudeRate_Config[Counter].BaudRate*1024));

            CANBIT|=((Prescaler-1)&0x3F);   //Setting the Prescaler in the BRP section of the register, BRP= Baud rate prescaler - 1

            //Setting Time Segment after Sample Point

            CANBIT|=(((BaudeRate_Config[Counter].TSEG2)-1)&0x7000);       //TSEG2=Time Segment after Sample Point-1   in quants

            //Setting Time Segment before Sample Point

            CANBIT|=(((BaudeRate_Config[Counter].TSEG1)-1)&0xF00);       //TSEG1=Time Segment after Sample Point-1  in quants

            //Setting   (Re)Synchronization Jump Width

            CANBIT|=(((BaudeRate_Config[Counter].SJW)-1)&0xC0);       //SJW=(Re)Synchronization Jump Width-1     in quants

            Can_Controller0_Statues== CAN_CS_STARTED;

            break;
        case Can_Controller1:

            set_bit(CANCTL_Controller2,0); //Initialization started
            set_bit(CANCTL_Controller2,6);  //Configuration Change Enable

            // Change Baud rate and timing

            //this for loop to find the configurations of the given BaudeRate ID

            for(Counter=0;((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID)&&Counter<Max_BaudeRateIDs);Counter++){

                //Counter after the for loop is the the number of the structure that hold the baud rate configuration

            }

            //if the given BaudRate Configuration ID isn't correct return error

            if((Counter==Max_BaudeRateIDs-1)&&((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID))){

                return E_NOT_OK;
            }

            //calculating the Prescaler for the baud rate

            Prescaler = (CanCpuClockRef/(BaudeRate_Config[Counter].BaudRate*1024));

            CANBIT_Controller2|=((Prescaler-1)&0x3F);   //Setting the Prescaler in the BRP section of the register, BRP= Baud rate prescaler - 1

            //Setting Time Segment after Sample Point

            CANBIT_Controller2|=(((BaudeRate_Config[Counter].TSEG2)-1)&0x7000);       //TSEG2=Time Segment after Sample Point-1   in quants

            //Setting Time Segment before Sample Point

            CANBIT_Controller2|=(((BaudeRate_Config[Counter].TSEG1)-1)&0xF00);       //TSEG1=Time Segment after Sample Point-1  in quants

            //Setting   (Re)Synchronization Jump Width

            CANBIT_Controller2|=(((BaudeRate_Config[Counter].SJW)-1)&0xC0);       //SJW=(Re)Synchronization Jump Width-1     in quants

            Can_Controller1_Statues== CAN_CS_STARTED;

            break;
        default:

            return E_NOT_OK;

            break;
        }
#elif CanControllerBaseAddress == Controller2_BaseAddress
        VAR(uint16,Can_CODE)Counter=0;
        VAR(uint8,Can_CODE) Prescaler;
        switch(Controller){
        case Can_Controller0:
            set_bit(CANCTL,0); //Initialization started
            set_bit(CANCTL,6);  //Configuration Change Enable
            // Change Baud rate and timing
            //this for loop to find the configurations of the given BaudeRate ID

            for(Counter=0;((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID)&&Counter<Max_BaudeRateIDs);Counter++){
                //Counter after the for loop is the the number of the structure that hold the baud rate configuration
            }
            //if the given BaudRate Configuration ID isn't correct return error
            if((Counter==Max_BaudeRateIDs-1)&&((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID))){
                return E_NOT_OK;
            }
            //calculating the Prescaler for the baud rate
            Prescaler = (CanCpuClockRef/(BaudeRate_Config[Counter].BaudRate*1024));
            CANBIT|=((Prescaler-1)&0x3F);   //Setting the Prescaler in the BRP section of the register, BRP= Baud rate prescaler - 1
            //Setting Time Segment after Sample Point
            CANBIT|=(((BaudeRate_Config[Counter].TSEG2)-1)&0x7000);       //TSEG2=Time Segment after Sample Point-1   in quants
            //Setting Time Segment before Sample Point
            CANBIT|=(((BaudeRate_Config[Counter].TSEG1)-1)&0xF00);       //TSEG1=Time Segment after Sample Point-1  in quants
            //Setting   (Re)Synchronization Jump Width
            CANBIT|=(((BaudeRate_Config[Counter].SJW)-1)&0xC0);       //SJW=(Re)Synchronization Jump Width-1     in quants
            Can_Controller0_Statues== CAN_CS_STARTED;
            break;
        case Can_Controller1:
            set_bit(CANCTL_Controller1,0); //Initialization started
            set_bit(CANCTL_Controller1,6);  //Configuration Change Enable
            // Change Baud rate and timing
            //this for loop to find the configurations of the given BaudeRate ID

            for(Counter=0;((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID)&&Counter<Max_BaudeRateIDs);Counter++){
                //Counter after the for loop is the the number of the structure that hold the baud rate configuration
            }
            //if the given BaudRate Configuration ID isn't correct return error
            if((Counter==Max_BaudeRateIDs-1)&&((BaudeRate_Config[Counter].BaudRateConfigIDs!=BaudRateConfigID))){
                return E_NOT_OK;
            }
            //calculating the Prescaler for the baud rate
            Prescaler = (CanCpuClockRef/(BaudeRate_Config[Counter].BaudRate*1024));
            CANBIT_Controller1|=((Prescaler-1)&0x3F);   //Setting the Prescaler in the BRP section of the register, BRP= Baud rate prescaler - 1
            //Setting Time Segment after Sample Point
            CANBIT_Controller1|=(((BaudeRate_Config[Counter].TSEG2)-1)&0x7000);       //TSEG2=Time Segment after Sample Point-1   in quants
            //Setting Time Segment before Sample Point
            CANBIT_Controller1|=(((BaudeRate_Config[Counter].TSEG1)-1)&0xF00);       //TSEG1=Time Segment after Sample Point-1  in quants
            //Setting   (Re)Synchronization Jump Width
            CANBIT_Controller1|=(((BaudeRate_Config[Counter].SJW)-1)&0xC0);       //SJW=(Re)Synchronization Jump Width-1     in quants
            Can_Controller1_Statues== CAN_CS_STARTED;

            break;
        default: return E_NOT_OK;break;
        }
#endif
        return E_OK;                    //Service request accepted, setting of (new) baud rate started
    }else{
        return E_NOT_OK ;                //Service request not accepted
    }
    return E_NOT_OK;
}

/*************************/


#endif













//This function performs software triggered state transitions of the CAN controller State machine..
FUNC(Std_ReturnType,Can_CODE) Can_SetControllerMode( uint8 Controller, Can_ControllerStateType Transition ){

    /*the state transition value CAN_CS_STARTED, the function Can_SetControllerMode shall re-initialize the CAN
      controller with the same controller configuration set previously used by functions Can_SetBaudrate or Can_Init.*/

#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_UNINIT);

    }else if(Controller>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_PARAM_CONTROLLER);

    }



#endif

    switch (Controller){

    case Can_Controller0:

        switch(Transition){

        case CAN_CS_STARTED:

            /*Check if the Controller in stopped state or not*/

            if(Can_Controller0_Statues!=CAN_CS_STOPPED){
#if CanDevErrorDetect ==STD_ON

                /*Error in the wanted transaction*/
                Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
            }else{

                /*Exit the Initialization state*/

                reset_bit(CANCTL,0); //change to normal operation to start the Can controller


                /*Changing the state */

                Can_Controller0_Statues=Transition;

                /*Enable the interrupts*/

                Can_EnableControllerInterrupts(Can_Controller0);

                /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                CanIf_ControllerModeIndication(CanControllerId,Transition);

            }
            break;

        case CAN_CS_STOPPED:

            /*Check if the Controller in stopped state or not*/

            if(Can_Controller0_Statues!=(CAN_CS_SLEEP||CAN_CS_STARTED)){

#if CanDevErrorDetect ==STD_ON

                /*Error in the wanted transaction*/
                Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
            }else{

                /*Enter the Initialization state*/

                set_bit(CANCTL,0); //change to Initialization operation to Stop the Can controller


                /*Changing the state */

                Can_Controller0_Statues=Transition;

                /*Disable the interrupts*/
                Can_DisableControllerInterrupts(Can_Controller0);

                /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                CanIf_ControllerModeIndication(CanControllerId,Transition);

            }

            break;

        case CAN_CS_SLEEP:

            /*Check if the Controller in stopped state or not*/

            if(Can_Controller0_Statues!=(CAN_CS_STOPPED)){

#if CanDevErrorDetect ==STD_ON

                /*Error in the wanted transaction*/
                Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
            } else{


                /*TO DO  Implement the sleep mode but for now make it as stop*/

                /***********************************************************/
                /*Enter the Initialization state*/

                set_bit(CANCTL,0); //change to Initialization operation to Stop the Can controller

                /*Disable the interrupts*/

                Can_DisableControllerInterrupts(Can_Controller0);

                /***********************************************************/

                /*Changing the state */

                Can_Controller0_Statues=Transition;

                /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                CanIf_ControllerModeIndication(CanControllerId,Transition);

            }

            break;

        case CAN_CS_UNINIT:


            /*Check if the Controller in stopped state or not*/

            if(Can_Controller0_Statues!=(CAN_CS_SLEEP||CAN_CS_STOPPED)){

#if CanDevErrorDetect ==STD_ON

                /*Error in the wanted transaction*/
                Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
            }else{

                Can_DeInit();


                /*Disable the interrupts*/

                Can_DisableControllerInterrupts(Can_Controller0);

                /*Changing the state */

                Can_Controller0_Statues=Transition;

                /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                CanIf_ControllerModeIndication(CanControllerId,Transition);

            }
            break;
        default:

        }

        return E_OK;

        break;

        case Can_Controller1:

            switch(Transition){

            case CAN_CS_STARTED:

                /*Check if the Controller in stopped state or not*/

                if(Can_Controller0_Statues!=CAN_CS_STOPPED){

#if CanDevErrorDetect ==STD_ON

                    /*Error in the wanted transaction*/
                    Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
                }else{

                    /*Exit the Initialization state*/

                    reset_bit(CANCTL,0); //change to normal operation to start the Can controller

                    /*Enable the interrupts*/

                    Can_EnableControllerInterrupts(Can_Controller1);

                    /*Changing the state */

                    Can_Controller1_Statues=Transition;

                    /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                    CanIf_ControllerModeIndication(CanControllerId,Transition);

                }
                break;

            case CAN_CS_STOPPED:

                /*Check if the Controller in stopped state or not*/

                if(Can_Controller1_Statues!=(CAN_CS_SLEEP||CAN_CS_STARTED)){

#if CanDevErrorDetect ==STD_ON

                    /*Error in the wanted transaction*/
                    Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
                }else{

                    /*Enter the Initialization state*/

                    set_bit(CANCTL,0); //change to Initialization operation to Stop the Can controller

                    /*Disable the interrupts*/
                    Can_DisableControllerInterrupts(Can_Controller1);

                    /*Changing the state */

                    Can_Controller0_Statues=Transition;

                    /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                    CanIf_ControllerModeIndication(CanControllerId,Transition);

                }

                break;

            case CAN_CS_SLEEP:

                /*Check if the Controller in stopped state or not*/

                if(Can_Controller0_Statues!=(CAN_CS_STOPPED)){

#if CanDevErrorDetect ==STD_ON

                    /*Error in the wanted transaction*/
                    Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
                }else{


                    /*TO DO  Implement the sleep mode but for now make it as stop*/

                    /***********************************************************/
                    /*Enter the Initialization state*/

                    set_bit(CANCTL,0); //change to Initialization operation to Stop the Can controller

                    /*Disable the interrupts*/

                    Can_DisableControllerInterrupts(Can_Controller1);

                    /***********************************************************/

                    /*Changing the state */

                    Can_Controller1_Statues=Transition;

                    /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                    CanIf_ControllerModeIndication(CanControllerId,Transition);

                }

                break;

            case CAN_CS_UNINIT:


                /*Check if the Controller in stopped state or not*/

                if(Can_Controller1_Statues!=(CAN_CS_SLEEP||CAN_CS_STOPPED)){

#if CanDevErrorDetect ==STD_ON

                    /*Error in the wanted transaction*/
                    Det_ReportError(Can_MODULE_ID, 0, Can_SetControllerMode_ID, CAN_E_TRANSITION);

#endif
                }else{

                    Can_DeInit();


                    /*Disable the interrupts*/

                    Can_DisableControllerInterrupts(Can_Controller0);

                    /*Changing the state */

                    Can_Controller1_Statues=Transition;

                    /*after successful transmission of state use the callback function CanIf_ControllerModeIndication()*/

                    CanIf_ControllerModeIndication(CanControllerId,Transition);

                }
                break;
            default:

            }


            return E_OK;

            break;

            default:

                return E_NOT_OK;

    }
    return E_NOT_OK;
}

/*************************/

//This function checks if a wakeup has occurred for the given controller.

#if CanWakeupFunctionalityAPI==True

FUNC(Std_ReturnType,Can_CODE) Can_CheckWakeup( uint8 ControllerId){


#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_CheckWakeup_ID, CAN_E_UNINIT);

    }else if(ControllerId>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_CheckWakeup_ID, CAN_E_PARAM_CONTROLLER);

    }


#endif


    switch(ControllerId){

    case Can_Controller0:

        if((Can_Controller0_Statues!=CAN_CS_UNINIT)&&(Can_Controller0_Statues!=CAN_CS_SLEEP)){

            //EcuM_SetWakeupEvent();

            return E_OK;

        }else{

            return E_NOT_OK;

        }

        break;

    case Can_Controller1:

        if((Can_Controller1_Statues!=CAN_CS_UNINIT)&&(Can_Controller1_Statues!=CAN_CS_SLEEP)){

            //EcuM_SetWakeupEvent();

            return E_OK;

        }else{

            return E_NOT_OK;
        }

        break;
    }

    return E_NOT_OK;
}


#endif





//This service reports about the current status of the requested CAN controller.
FUNC(Std_ReturnType,Can_CODE) Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr ){

    #if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerMode_ID, CAN_E_UNINIT);

    }else if(Controller>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerMode_ID, CAN_E_PARAM_CONTROLLER);

    }else if(ControllerModePtr==0){
        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerMode_ID, CAN_E_PARAM_POINTER);

    }



#endif

    switch(Controller){

    case Can_Controller0:

        *ControllerModePtr=Can_Controller0_Statues;

        break;

    case Can_Controller1:

        *ControllerModePtr=Can_Controller1_Statues;

        break;
    default: return E_NOT_OK;break;
    }

    return E_NOT_OK;
}

/*************************/














/**********************************/
//This function is called by CanIf to pass a CAN message to CanDrv for transmission.
FUNC(void,Can_CODE) Can_EnableControllerInterrupts( uint8 Controller ){

#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_EnableControllerInterrupts_ID, CAN_E_UNINIT);

    }else if(Controller>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_EnableControllerInterrupts_ID, CAN_E_PARAM_CONTROLLER);

    }



#endif
    Can_InterruptsCounter--;

    if(Can_InterruptsCounter==0){

        switch(Controller){

        case Can_Controller0:

            if(Can_Controller0_Statues==CAN_CS_STARTED){

                /*Enable interrupts*/
                set_bit(CANCTL_Controller1,1);      //General Can interrupt
                set_bit(CANCTL_Controller1,3);      //Error Interrupt
                set_bit(CANCTL_Controller1,2);      //Statues Interrupt

                /***********NVIC********/

                set_bit(NVIC_EN1,7);

                /**********************************/
            }
            break;
        case Can_Controller1:

            if(Can_Controller1_Statues==CAN_CS_STARTED){

                /*Enable interrupts*/
                set_bit(CANCTL_Controller2,1);      //General Can interrupt
                set_bit(CANCTL_Controller2,3);      //Error Interrupt
                set_bit(CANCTL_Controller2,2);      //Statues Interrupt

                /***********NVIC********/

                set_bit(NVIC_EN1,8);

                /**********************************/
            }
            break;
        default: break;

        }
    }
}






VAR(PduIdType,Can_CODE) Tx_PduID;

//Can_HwHandleType Hth Number of the memory object   1->32
FUNC(Std_ReturnType,Can_CODE) Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo ){

#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_Write_ID, CAN_E_UNINIT);

    }else if(Hth>32){       //max number of memory objects in the Tiva c

        Det_ReportError(Can_MODULE_ID, 0, Can_Write_ID, CAN_E_PARAM_HANDLE);

    }else if(PduInfo->length>8){

        Det_ReportError(Can_MODULE_ID, 0, Can_Write_ID, CAN_E_PARAM_DATA_LENGTH);

    }else if (PduInfo==0){

        Det_ReportError(Can_MODULE_ID, 0, Can_Write_ID, CAN_E_PARAM_POINTER);

    }



#endif


    Tx_PduID=PduInfo->id;
    /*if controller is stopped then don't start the writing*/

    if(Can_Controller0_Statues==CAN_CS_STOPPED){

        return E_NOT_OK;

    }

    VAR(uint8,CanCODE)                  Counter;        //Counter for the loop
    VAR(uint32,CanCODE)                 Temp   ;        //contain the Data before send it to registers
    P2VAR(uint32,CanCODE,Can_VAR_INIT) CANIF2DA1_Address=&CANIF2DA1_Addr; //Variable contain the the First address of the Data registers


    if(Can_Controller0_Statues==CAN_CS_STARTED){

        /*Initialize message objects as not valid*/

        reset_bit(CANIF2ARB2,15);   //MSGVAL = 0

        /****************************************/

        /*Wait on the hardware transmit handler*/

        Mutex_Wait(Can_HTHMutex);

        while(get_bit(CANIF2CRQ,15)==1);    //Wait for busy bit to clear

        /* 1 ) In the CAN IFn Command Mask (CANIFnCMASK) register:*/

        //Set the WRNRD bit to specify a Write on the memory object to intialize it

        set_bit(CANIF2CMSK,7);

        /*Specify which bits to transfer using the and bits DATAA DATAB*/

        /*Set the Pin DATAA to allow the use Data A registers*/

        set_bit(CANIF2CMSK,1);

        /*Set the Pin DATAB to allow the use Data B registers*/

        set_bit(CANIF2CMSK,0);

        /*Set the Control access pin*/

        set_bit(CANIF2CMSK,4);

        /* This the initial state for the registers.*/

        CANIF2ARB1=0;
        CANIF2ARB2=0;
        CANIF2MCTL=0;
        CANIF2MSK1=0;
        CANIF2MSK2=0;

        /*Set the Transmit Request bit*/

        set_bit(CANIF2MCTL,8);

        /*Set the DIR to set the direction as Transmit.*/

        set_bit(CANIF2ARB2,13);

        //specify whether to transfer the IDMASK, DIR, and MXTD of the message object into the CAN IFn

        reset_bit(CANIF2CMSK,6);

        /*Specify whether to transfer the ID, DIR, XTD, and MSGVAL of the message object into the interface registers*/

        reset_bit(CANIF2CMSK,5);

        /*Specify whether to clear the bit in the CANIFnMCTL register using the INTPND CLRINTPND bit*/

        reset_bit(CANIF2CMSK,3);

        /*Specify whether to clear the bit in the CANNWDAn register using the bit NEWDAT NEWDAT*/

        reset_bit(CANIF2CMSK,2);

        /******************************************************************************************************/

        /* 2 ) Specify which of the bits are used for acceptance filtering*/

#if CanIdType==STANDARD //IDs is 11bit

        CANIF2MSK1=0;
        CANIF2MSK2=((uint32)((CanHwFilterMask)&(0x7FF))<<2);

#elif   CanIdType==EXTENDED     //IDs is 29bit

        CANIF2MSK1|=((uint32)((CanHwFilterMask)&(0xFFFF)));
        CANIF2MSK2|=((uint32)((CanHwFilterMask)&(0x1FFF>>16)));
#endif

        /*Enable the Use of Acceptance Mask*/

        set_bit(CANIF2MCTL,12);

        /*Enable the Access to mask bits*/

        set_bit(CANIF2CMSK,6);

        /*Enable the Access Arbitration Bits*/

        set_bit(CANIF2CMSK,5);

        /******************************************************************************************************/

        /* 3 ) Use the MXTD and MDIR bits to specify whether to use XTD and DIR for acceptance filtering.*/

        /*The extended identifier bit XTD is used for acceptance filtering.*/

        reset_bit(CANIF2MSK2,15);

        /*The message direction bit DIR is used for acceptance filtering.*/

        reset_bit(CANIF2MSK2,14);

        /******************************************************************************************************/

        /* 4 ) Set the Id and the Type as receive memory object*/

#if CanIdType==STANDARD //IDs is 11bit

        reset_bit(CANIF2ARB2,14);
        CANIF2ARB2|=((uint32)((PduInfo->id)&(0x1FFF))<<2);

#elif   CanIdType==EXTENDED     //IDs is 29bit

        set_bit(CANIF2ARB2,14);
        CANIF2ARB1|=((uint32)((PduInfo->id)&(0xFFFF)));
        CANIF2ARB2|=((uint32)((PduInfo->id)&(0xFFF>>16)));
#endif

        /*set the MSGVAL bit to indicate message object valid*/

        set_bit(CANIF2ARB2,15);
        /******************************************************************************************************/

        /* 5 ) CANIFnMCTL Register  */

        /*specify the size of the data frame in bytes.*/

        CANIF2MCTL|=((PduInfo->length)&0xF);

        /*End of Buffer => Single message object*/

        set_bit(CANIF2MCTL,7);

        /*Transmit Interrupt Enable*/

        set_bit(CANIF2MCTL,11);

        /*Disable the use of remote frame */

        reset_bit(CANIF2MCTL,9);

        /******************************************************************************************************/

        /* 7 ) Loading the data the SDU (Service Data Unit)*/

        for(Counter = 0; Counter < PduInfo->length; )
        {
            /*Write the data 16 bits at a time since this is how the registers are aligned in memory.*/

            /* Store the first byte.*/

            Temp=PduInfo->sdu[Counter++] ;  //use the Data with Index Counter then add 1 to it

            /* Only read the second byte if needed.*/

            if(Counter < PduInfo->length)

            {
                Temp |= PduInfo->sdu[Counter++] << 8;       //use the Data with the added Index then add 1 to it

            }

            //Write DATA_A1 then increase the address of it by one which is 32bit to access DATA_A2

            (*(volatile uint32*)(CANIF2DA1_Address++))=Temp;
        }


        /* 8 ) Choosing the memory object to be send*/

        CANIF2CRQ|=(Hth)&(0x3F);

        /*Wait for busy bit to clear*/

        while(get_bit(CANIF1CRQ,15)==1);

        /*Release on the hardware transmit handler*/

        Mutex_Signal(Can_HTHMutex);

        return E_OK;

    }else{

        return E_NOT_OK;
    }

    return E_NOT_OK;
}
/*************************/




FUNC(void,Can_CODE) Can_DisableControllerInterrupts( uint8 Controller ){

    /*When Can_DisableControllerInterrupts has been called several times,
      Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled.*/

#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_DisableControllerInterrupts_ID, CAN_E_UNINIT);

    }else if(Controller>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_DisableControllerInterrupts_ID, CAN_E_PARAM_CONTROLLER);

    }



#endif

    Can_InterruptsCounter++;

    /*Disable all interrupts*/

    switch(Controller){

    case Can_Controller0:

        if(Can_Controller0_Statues==CAN_CS_STARTED){

            reset_bit(CANCTL_Controller1,1);

            /***********NVIC********/

            set_bit(NVIC_DIS1,7);

            /**********************************/
        }
        break;

    case Can_Controller1:

        if(Can_Controller1_Statues==CAN_CS_STARTED){

            reset_bit(CANCTL_Controller2,1);

            /***********NVIC********/
            set_bit(NVIC_DIS1,8);
            /**********************************/

        }
        break;
    default: break;

    }
}







//This service obtains the error state of the CAN controller.

FUNC(Std_ReturnType,Can_CODE) Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType*ErrorStatePtr ){

#if CanDevErrorDetect ==STD_ON

    /*implementation of the DET*/
    if(Can_Controller0_Statues==CAN_CS_UNINIT){

        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerErrorState_ID, CAN_E_UNINIT);

    }else if(ControllerId>Can_Controller1){

        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerErrorState_ID, CAN_E_PARAM_CONTROLLER);

    }else if(ErrorStatePtr==0){
        Det_ReportError(Can_MODULE_ID, 0, Can_GetControllerErrorState_ID, CAN_E_PARAM_POINTER);

    }



#endif

    switch(ControllerId){

    case Can_Controller0:

        if(Error_Flag==TRUE){

            if(get_bit(Can_StatuesReg,7)){

                (*ErrorStatePtr)=CAN_ERRORSTATE_BUSOFF;

            }else if(get_bit(Can_StatuesReg,5)){

                (*ErrorStatePtr)=CAN_ERRORSTATE_PASSIVE;

            }else{

                (*ErrorStatePtr)=CAN_ERRORSTATE_ACTIVE;
            }
        }

        else{

            (*ErrorStatePtr)=CAN_ERRORSTATE_ACTIVE;
        }

        break;
    case Can_Controller1:

        if(Error_Flag==TRUE){

            if(get_bit(Can_StatuesReg,7)){

                (*ErrorStatePtr)=CAN_ERRORSTATE_BUSOFF;

            }else if(get_bit(Can_StatuesReg,5)){

                (*ErrorStatePtr)=CAN_ERRORSTATE_PASSIVE;
            }else{

                (*ErrorStatePtr)=CAN_ERRORSTATE_ACTIVE;
            }
        }
        else{

            (*ErrorStatePtr)=CAN_ERRORSTATE_ACTIVE;
        }
        break;
    }
    return E_OK;
}














/*********************************************Scheduled functions***********************************************************/
//This function performs the polling of TX confirmation when CAN_TX_PROCESSING is set to POLLING.
#if (CanTxProcessing==POLLING)||(CanTxProcessing==MIXED)
/*Variables to forward to CAN IF*/
VAR(extern uint8,CanCODE)              PDU_Flags;
VAR(extern Can_HwType,CanCODE)         Mailbox;
VAR(extern PduInfoType,CanCODE)        PduInfo;

FUNC(void,Can_CODE) Can_MainFunction_Write( void ){
    if(get_bit(Can_StatuesReg,3)==1){
        TX_Flag=TRUE;
        CanLPduReceiveCalloutFunction();
    }else{
        TX_Flag=FALSE;
    }
}

#endif






//This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
#if (CanRxProcessing==POLLING)||(CanRxProcessing==MIXED)
FUNC(void,Can_CODE) Can_MainFunction_Read( void ){
    if(get_bit(Can_StatuesReg,4)==1){
        CanIf_RxIndication(&MailboxPtr,&PduInfoPtr);
        RX_Flag=TRUE;
    }else{
        RX_Flag=FALSE;
    }
}
#endif




//This function performs the polling of bus-off events that are configured statically as 'to be polled'.
#if CanBusoffProcessing==POLLING
FUNC(void,Can_CODE) Can_MainFunction_BusOff( void ){
    if(get_bit(Can_StatuesReg,7)==1){
        BusOff_Flag=TRUE;
    }else{
        BusOff_Flag=FLASE;
    }
}
#endif








//This function performs the polling of wake-up events that are configured statically as 'to be polled'.
#if CanWakeupProcessing == POLLING
FUNC(void,Can_CODE) Can_MainFunction_Wakeup( void ){
    if((Can_Controller0_Statues!=CAN_CS_UNINIT)&&(Can_Controller0_Statues!=CAN_CS_SLEEP)){
        Wakeup_Flag=TRUE;
    }else{
        Wakeup_Flag=FALSE;
    }
}
#endif

//This function performs the polling of CAN controller mode transitions.
FUNC(void,Can_CODE) Can_MainFunction_Mode( void ){

}

/***************************************************************************************************************************/
