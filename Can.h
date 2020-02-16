#ifndef CAN_H_
#define CAN_H_

#include"ComStack_Types.h"
#include"Can_GeneralTypes.h"
#include"Can_Cfg.h"
#include"Can_Reg.h"
#include "UTILES.h"
#include "Can_Definitions.h"
FUNC(void, Can_CODE) Can_Init( const Can_ConfigType* Config ); //This function initializes the module.


FUNC(void, Can_CODE) Can_DeInit( void );		//This function de-initializes the module.

#if CanSetBaudrateApi==STD_ON
FUNC(Std_ReturnType,Can_CODE) Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );
//This service shall set the baud rate configuration of the CAN controller.
#endif
FUNC(Std_ReturnType,Can_CODE) Can_SetControllerMode( uint8 Controller, Can_ControllerStateType Transition);
/*This function performs software triggered state
						transitions of the CAN controller State machine*/

#if CanWakeupFunctionalityAPI==True
FUNC(Std_ReturnType,Can_CODE) Can_CheckWakeup( uint8 ControllerId);
#endif
//This function checks if a wakeup has occurred for the given controller.



FUNC(Std_ReturnType,Can_CODE) Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* 
                                                     ControllerModePtr );
/*This service reports about the current status of the requested CAN controller.*/

FUNC(Std_ReturnType,Can_CODE) Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo );
//This function is called by CanIf to pass a CAN message to CanDrv for transmission.


FUNC(void,Can_CODE) Can_DisableControllerInterrupts( uint8 Controller );
//This function disable all Can interrupts is mainly used entering exclusive section



FUNC(void,Can_CODE) Can_EnableControllerInterrupts( uint8 Controller );
//This function Enable all Can interrupts to enable the interrupts it should be called the same number as
//Can_DisableControllerInterrupts



FUNC(Std_ReturnType,Can_CODE) Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType*ErrorStatePtr );
//This service obtains the error state of the CAN controller.




/***************************Scheduled functions*****************************************/
#if CanTxProcessing == POLLING
FUNC(void,Can_CODE) Can_MainFunction_Write( void );
//This function performs the polling of TX confirmation when CAN_TX_PROCESSING is set to POLLING.
#endif



#if CanRxProcessing==POLLING
FUNC(void,Can_CODE) Can_MainFunction_Read( void );
//This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
#endif



#if CanBusoffProcessing==POLLING
FUNC(void,Can_CODE) Can_MainFunction_BusOff( void );
//This function performs the polling of bus-off events that are configured statically as 'to be polled'.
#endif



#if CanWakeupProcessing == POLLING
FUNC(void,Can_CODE) Can_MainFunction_Wakeup( void );
//This function performs the polling of wake-up events that are configured statically as 'to be polled'.
#endif

FUNC(void,Can_CODE) Can_MainFunction_Mode( void );
//This function performs the polling of CAN controller mode transitions.

/*****************************************************************************************/
/*Can ISR*/
//FUNC(void, Can_CODE) CANIntHandler( void);







#endif
