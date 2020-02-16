#include "Can.h"
#include"CanIf_Cbk.h"

VAR( uint32,Can_CODE) Can_StatuesReg=0;                 //use variable because this register is read sensitive
VAR( boolean,Can_CODE) Error_Flag=FALSE;                //Error Flag
VAR(static uint32,Can_CODE) NumReceivedMsgs=0;          //Number of the Received messages
VAR(static uint32,Can_CODE) NumTransmittedMsgs=0;       //Number of the Transmitted messages
VAR( boolean,Can_CODE) ReceivedMsgsPendingFlg=FALSE;    //received message is pending Flag.
VAR(extern Can_ConfigType,Can_CODE) Configurations;     //the input data to the Can_Init
VAR(extern PduIdType,Can_CODE) Tx_PduID;                //the ID of the PDU of the sent data

FUNC(void, Can_CODE) CANMessageGet(void);


FUNC(void, Can_CODE) CANIntHandler( void){

    /* Read the CAN interrupt status to find the cause of the interrupt*/
    Can_StatuesReg = CANSTS;

    /* If the cause is a controller status interrupt, is an error*/
    if((get_bit(Can_StatuesReg,6)||get_bit(Can_StatuesReg,7))==1)
    {
        // Set a flag to indicate some errors may have occurred.

        Error_Flag = TRUE;
    }

    // Check if the cause of the interrupt is  receiving messages.

    else if(get_bit(Can_StatuesReg,4) == 1)
    {
        /*  the RX interrupt occurred on message object, and the message reception is complete.*/
        /*Clear the message object interrupt.*/

        /* if what happened is a statues interrupt*/
        if(CANINT==0x00008000){

            // to clear the interrupt just read the CANSTS register

            CANSTS;
        }else
        {
            /*what happened was not a statues interrupts*/

            //change the interrupt pending state by setting CLRINTPND bit.
            CANIF1CMSK=0x00000008;
            // Send the clear pending interrupt command to the CAN controller.
            CANIF1CRQ|=(Configurations.Hth)&(0x3F);
        }
        /*Increment a counter to keep track of how many messages have been received.*/

        NumReceivedMsgs++;

        /*Set flag to indicate received message is pending.*/

        ReceivedMsgsPendingFlg = TRUE;

        /* Since a message was received, clear any error flags.*/

        Error_Flag = FALSE;

        /*this function Forward the Data to the Can IF vif the function CanIf_RxIndication*/
        CanLPduReceiveCalloutFunction();
    }

    // Check if the cause of the interrupt is  successfully Transmit a messages.

    else if(get_bit(Can_StatuesReg,3) == 1){

        /*  the TX interrupt occurred on message object, and the message Transmisssion is complete.*/

        /*Clear the message object interrupt.*/

        reset_bit(CANSTS,3);

        /*Increment a counter to keep track of how many messages have been received.*/

        NumTransmittedMsgs++;

        /* Since a message was received, clear any error flags.*/

        Error_Flag = FALSE;

        /*Infrom the CAN IF of a valid Transmission and send the ID of the sent PDU */
        CanIf_TxConfirmation(Tx_PduID);
    }

    /*Otherwise, something unexpected caused the interrupt.  This should never happen.*/

    else

    {

    }
}






/******************************************************************************************************************************/
/*Variables to forward to CAN IF*/
VAR(uint8,CanCODE)              SDU_Data[8]; //the total data
VAR(uint8,CanCODE)              SDU_MetaData; //the meta data

VAR(uint8,CanCODE)              PDU_Flags;
VAR(Can_HwType,CanCODE)         Mailbox;
VAR(PduInfoType,CanCODE)        PduInfo;

/* Read the message from the CAN. The interrupt clearing
   flag is not set because this interrupt was already cleared in  the interrupt handler.*/

FUNC(void, Can_CODE) CANMessageGet(void){

    /*initialize the pointers in the data to be send to avoid wild pointers*/

    PduInfo.SduDataPtr=&SDU_Data;
    PduInfo.MetaDataPtr=&SDU_MetaData;

    /*Variables used in the cunction*/

    VAR(uint8,CanCODE)                  Counter;        //Counter for the loop
    VAR(uint32,CanCODE)                 Temp   ;        //contain the value of the register temporary
    P2VAR(uint32,CanCODE,Can_VAR_INIT) CANIF2DA1_Address=&CANIF2DA1_Addr; //Variable contain the the First address of the Data registers


    /*Variables to contain the value of the IF registers*/
    VAR( uint32,Can_CODE)   CANIF2MaskReg1   = 0;
    VAR( uint32,Can_CODE)   CANIF2MaskReg2   = 0;
    VAR( uint32,Can_CODE)   CANIF2ArbReg1    = 0;
    VAR( uint32,Can_CODE)   CANIF2ArbReg2    = 0;
    VAR( uint32,Can_CODE)   CANIF2MsgCtrReg  = 0;

    /*This is always a read to the Message object as this call is setting a message object.*/
    /*Set up the request for data from the message object.*/

    set_bit(CANIF2CMSK,1);          //Access Data Byte 0 to 3 DATAA
    set_bit(CANIF2CMSK,0);          //Access Data Byte 4 to 7 DATAB
    set_bit(CANIF2CMSK,4);          //Access Control Bits
    set_bit(CANIF2CMSK,6);          //Access Mask Bits
    set_bit(CANIF2CMSK,5);          //Access Arbitration Bits

    /*Transfer the message object to the message object specified by the number of the memory object*/

    CANIF2CRQ|=(Configurations.Hth)&(0x3F);

    /* Wait for busy bit to clear*/

    while(get_bit(CANIF2CRQ,15)==1);

    /*Read out the IF Registers. in case something change it while reading*/

    CANIF2MaskReg1   = CANIF2MSK1;
    CANIF2MaskReg2   = CANIF2MSK2;
    CANIF2ArbReg1    = CANIF2ARB1;
    CANIF2ArbReg2    = CANIF2ARB2;
    CANIF2MsgCtrReg  = CANIF2MCTL;
    /*Initialize the flags as No flag value*/

    PDU_Flags= MSG_OBJ_NO_FLAGS;

    /*Get the identifier out of the register, the format depends on size of the mask*/

    if(get_bit(CANIF2ArbReg2,14)==1){

        /* Set the 29 bit version of the Identifier for this message object.*/

        Mailbox.CanId=(((CANIF2ArbReg2 & 0x00001FFF)<<16)|CANIF2ArbReg1);

        PDU_Flags= MSG_OBJ_EXTENDED_ID;

    }else{

        /* The Identifier is an 11 bit value.*/

        Mailbox.CanId=(((CANIF2ArbReg2 & 0x00001FFF)>>2));
    }

    /*verify whether there's data loss or not*/

    if(get_bit(CANIF2MsgCtrReg ,14)==1){

        PDU_Flags=MSG_OBJ_DATA_LOST;

    }

    /* See if there is new data available. */

    if(get_bit(CANIF2MsgCtrReg,15)==1){


        /* Get the amount of data needed to be read.*/


        PduInfo.SduLength= CANIF2MsgCtrReg & 0x0000000F;        //Read the DLC Data Length Code


        /* Read out the data from the CAN registers.*/

        /*Loop to Copy 2 bytes per iteration*/

        for(Counter = 0; Counter < PduInfo.SduLength; )

        {

            /* Read out the data 16 bits at a time since this is how the registers are aligned in memory.*/


            Temp = (*(volatile uint32*)(CANIF2DA1_Address++));


            //Read DATA_A1 then increase the address of it by one which is 32bit to access DATA_A2

            /* Store the first byte.*/


            PduInfo.SduDataPtr[Counter++] = (uint8)Temp;


            /* Only read the second byte if needed.*/

            if(Counter < PduInfo.SduLength)

            {

                PduInfo.SduDataPtr[Counter++] = (uint8)(Temp >> 8);

            }
        }

        /* Now clear out the new data flag.*/

        reset_bit(CANIF2CMSK,15);

        /* Transfer the message object to the message object specified by Configurations.Hth.*/
        CANIF2CRQ|=(Configurations.Hth)&(0x3F);

        /* Wait for busy bit to clear*/

        while(get_bit(CANIF2CRQ,15)==1);
        /* Indicate that there is new data in this message.*/

        PDU_Flags =  MSG_OBJ_NEW_DATA;
    }else       //there's no new Data
    {
    }

    /*Preparing Data to be Forwarded to CAN IF*/

    Mailbox.ControllerId=CanControllerId;
    Mailbox.Hoh=Configurations.Hth;
    PduInfo.MetaDataPtr=&PDU_Flags;

    /*Forwarding the Data to the CAN IF*/

    CanIf_RxIndication(&Mailbox,&PduInfo);
}
/******************************************************************************************************************************/
/*should be deleted after connecting with CANIF*/
FUNC(void,Can_CODE)CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr){
    uint8  x=5;
}

FUNC(void,Can_CODE)CanIf_ControllerModeIndication(uint8 ControllerId,Can_ControllerStateType ControllerMode){
    uint8    x=5;
}
FUNC(void,Can_CODE)CanIf_TxConfirmation(PduIdType CanTxPduId){
    uint8    x=5;
}
