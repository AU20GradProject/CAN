#ifndef CAN_CFG_H_
#define CAN_CFG_H_
#include "Can_Definitions.h"
/*********************Can General*******************************/
//Pre-compile time

#define 	CanDevErrorDetect	STD_OFF	// boolean
/*Switches the development error detection and notification on or off.
 true: detection and notification is enabled.
 false: detection and notification is disabled*/


#define 	CanIndex	((uint8)15u)		//Range 0-255
/*Specifies the Instance Id of this module instance.
 If only one instance is present it shall have the Id 0.*/


#define CanLPduReceiveCalloutFunction       CANMessageGet
/*parameter defines existence and name of a callout function which called after a successful
reception of a received CAN Rx L-PDU.If this parameter is omitted no callout shall take place.
 */

#define CanMainFunctionBusoffPeriod			//Range 0-inf
/*This parameter describes the period for cyclic call to Can_MainFunction_Busoff. Unit is seconds.*/

#define CanMainFunctionModePeriod		//Range 0-inf
/*This parameter describes the period for cyclic call to Can_MainFunction_Mode. Unit is seconds*/

#define  CanMainFunctionWakeupPeriod			//Range 0-inf
/*This parameter describes the period for cyclic call to Can_MainFunction_Wakeup. Unit is seconds*/

#define CanMultiplexedTransmission		STD_OFF		// Boolean
/*Specifies if multiplexed transmission shall be supported.ON or OFF*/

#define	CanPublicIcomSupport			STD_ON		//Boolean
/*Selects support of Pretended Network features in Can driver.
True: Enabled 
False: Disabled*/

#define CanSetBaudrateApi	STD_ON			//Boolean
/*The support of the Can_SetBaudrate API is optional.
If parameter is true the Can_SetBaudrate API shall be supported. Otherwise it's not supported.*/

#define	CanTimeoutDuration				//Float
/*Specifies the maximum time for blocking function until a timeout is detected. Unit is seconds.*/

#define 	CanVersionInfoApi	STD_OFF		//Boolean
/*Switches the Can_GetVersionInfo() API ON or OFF.*/

#define CanOsCounterRef				((uint32)80000000)//type Reference to [ OsCounter ]
/*This parameter contains a reference to the OsCounter, which is used by the CAN driver.*/

#define	CanSupportTTCANRef			//Reference to [ CanIfPrivateCfg ]
/*The parameter refers to CanIfSupportTTCAN parameter in the CAN Interface Module configuration*/
/****************************************************/


/*********************CanController*******************************/

#define CanBusoffProcessing			INTERRUPT			//type enum	INTERRUPT,POLLING
/*Enables / disables API Can_MainFunction_BusOff() for handling busoff events in polling mode.*/

#define	CanControllerActivation			STD_ON			//Boolean
/*Defines if a CAN controller is used in the configuration.*/

#define	CanControllerBaseAddress	0x40040000/*((uint32) 0x40040000u)*/       //CAN0: 0x40040xxx CAN1: 0x40041xxx		//Range  0=>4294967295
/*Specifies the CAN controller base address.*/

#define	CanControllerId				((uint8)30u)			//Range 0=>255
/*This parameter provides the controller ID which is unique in a given CAN Driver.*/

#define	CanRxProcessing				INTERRUPT					//type enum	INTERRUPT,POLLING,MIXED
/*Enables / disables API Can_MainFunction_Read() for handling PDU reception*/

#define	CanTxProcessing				INTERRUPT					//type enum	INTERRUPT,POLLING.MIXED
/*Enables / disables API Can_MainFunction_Write() for handling PDU transmission events in polling mode*/

#define CanWakeupFunctionalityAPI	True					//Boolean
/* Adds / removes the service Can_CheckWakeup() from the code.
 True: Can_CheckWakeup can be used.
 False: Can_CheckWakeup cannot be used.*/

#define CanWakeupProcessing				INTERRUPT			//type enum	INTERRUPT,POLLING
/*Enables / disables API Can_MainFunction_Wakeup() for handling wakeup events in polling mode.*/

#define	CanControllerDefaultBaudrate	512//Reference to [ CanControllerBaudrateConfig ]
/*Reference to baudrate configuration container configured for the Can Controller.*/
#define McuClockReferencePoint  ((uint32) 80000000)
#define	CanCpuClockRef		McuClockReferencePoint	//Reference to [ McuClockReferencePoint ]
/*Reference to the CPU clock configuration, which is set in the MCU driver configuration*/	

#define	CanWakeupSourceRef				//Symbolic name reference to [ EcuMWakeupSource ]
/*This parameter contains a reference
 to the Wakeup Source for this controller as defined in the ECU State Manager.*/
/****************************************************/


/*********************CanControllerBaudrateConfig*******************************/

#define CanControllerBaudRate		((uint32)1300)			//Range 0..2000
/*Specifies the baudrate of the controller in kbps.*/

#define	CanControllerBaudRateConfigID	((uint16)1u)		//Range 0 .. 65535
/*Uniquely identifies a specific baud rate configuration. This ID is used by SetBaudrate API.*/ 

#define CanControllerPropSeg			((uint8)2u)		//Range 0 .. 255
/*Specifies propagation delay in time quantas.*/ 

#define	CanControllerSeg1				((uint8)3u)		//Range 0 .. 255
/*Specifies phase segment 1 in time quantas.*/

#define	CanControllerSeg2				((uint8)1u)		//Range 0 .. 255
/*Specifies phase segment 2 in time quantas.*/

#define CanControllerSyncJumpWidth		((uint8)1u)		//Range 0 .. 255
/*Specifies the synchronization jump width for the controller in time quantas.*/
/****************************************************/


/*********************CanHardwareObject*******************************/
/*	HRH=> Hardware recieve handel 
	HTH=> Hardware transmite handel
	HOH=> Hardware object handle
 */
#define CanHandleType						BASIC		//enum BASIC,FULL
/*Specifies the type (Full-CAN or Basic-CAN) of a hardware object.*/

#define CanHardwareObjectUsesPolling   False				//Boolean
/*Enables polling of this hardware object.*/

#define CanHwObjectCount			((uint16)u)		//Range 0 .. 65535
/*In case of a HRH this parameter defines the number of elements in the hardware FIFO or the number of 
shadow buffers,
 in case of a HTH it defines the number of hardware objects used for multiplexed transmission */

#define CanIdType	STANDARD						//enum EXTENDED(29bit),STANDARD(11bit)
/*Specifies whether the IdValue is of type
- standard identifier
- extended identifier 
- mixed mode
ImplementationType: Can_IdType*/

#define CanObjectId				((uint16)25u)		//Range 0 .. 65535
/*Holds the handle ID of HRH or HTH.*/

#define	CanObjectType						//enum RECEIVE,TRANSMIT		
/*Specifies if the HardwareObject is used as Transmit or as Receive object*/

#define CanTriggerTransmitEnable			//Boolean
/*This parameter defines if or if not Can supports the trigger-transmit API for this handle.*/

#define	CanControllerRef					//Reference to [ CanController ]
/*Reference to CAN Controller to which the HOH is associated to.*/

#define	CanMainFunctionRWPeriodRef			//Reference to [ CanMainFunctionRWPeriods ]
/*Reference to CanMainFunctionPeriod*/
/****************************************************/


/*********************CanHwFilter*******************************/
#define CanHwFilterCode				((uint32)5u)			//Range  0=>4294967295
/*Specifies (together with the filter mask) the identifiers range that passes the hardware filter.*/
#if (CanIdType == EXTENDED)
#define	CanHwFilterMask				((uint32)0u)			//29 bit Range  0=>4294967295
#elif (CanIdType == STANDARD)
#define CanHwFilterMask             ((uint16)0u)            //11 bit
#endif
/*Describes a mask for hardware-based filtering of CAN identifiers. The CAN identifiers of
 incoming messages are masked with the appropriate CanFilterMaskValue. Bits holding a 0 mean don't care,
 i.e. do not compare the message's identifier in the respective bit position.
The mask shall be build by filling with leading 0. 
In case of CanIdType EXTENDED or MIXED a 29 bit mask shall be build.
 In case of CanIdType STANDARD a 11 bit mask shall be build*/
/****************************************************/



/*********************CanMainFunctionRWPeriods*******************************/
#define CanMainFunctionPeriod				//Float  0.. inf
/*This parameter describes the period for cyclic call to Can_MainFunction_Read or Can_MainFunction_Write*/

/****************************************************/


/*********************CanIcomConfig*******************************/
#define CanIcomConfigId			((uint8)13u)		//Range 1 .. 256
/*This parameter identifies the ID of the ICOM configuration.*/

#define	CanIcomWakeOnBusOff			False			//Boolean
/*This parameter defines that the MCU shall wake if the bus off is detected or not.*/
/****************************************************/


/*********************CanIcomGeneral*******************************/
#define	CanIcomLevel	CAN_ICOM_LEVEL_ONE			//enum CAN_ICOM_LEVEL_ONE,CAN_ICOM_LEVEL_TWO
/*Defines the level of Pretended Networking.*/

#define	CanIcomVariant	CAN_ICOM_VARIANT_NONE		//enum CAN_ICOM_VARIANT_HW,CAN_ICOM_VARIANT_NONE,CAN_ICOM_VARIANT_SW
/*Defines the variant, which is supported by this CanController*/
/****************************************************/


/*********************CanIcomRxMessage*******************************/
#define CanIcomCounterValue		((uint16)u)		//Range 1 .. 65536
/*defines that the MCU shall wake if the message with the ID is received n times on the communication 
channel.*/

#define CanIcomMessageId		((uint32)u)			//Range  0=>536870912
/*This parameter defines the message ID the wakeup causes of this CanIcomRxMessage are configured for*/

#define CanIcomMessageIdMask	((uint32)u)			//Range  0=>536870912
/*Describes a mask for filtering of CAN identifiers. 
The CAN identifiers of incoming messages are masked with this CanIcomMessageIdMask*/

#define CanIcomMissingMessageTimerValue				//Float		[-INF .. INF]
/*This parameter defines that the MCU shall wake if the message with the ID is not received for a 
specific time in s on the
communication channel*/

#define CanIcomPayloadLengthError					//Boolean
/*This parameter defines that the MCU shall wake if a payload error occurs*/
/****************************************************/



/*********************CanIcomRxMessageSignalConfig*******************************/
#define	CanIcomSignalMask		((uint64)u)			//Range  0=>18446744073709551615
/*This parameter shall be used to mask a signal in the payload of a CAN message*/

#define CanIcomSignalOperation			//enum AND,EQUAL,GREATER,SMALLER,XOR
/*This parameter defines the operation, which shall be used to verify the signal value creates a wakeup 
condition*/

#define CanIcomSignalValue		((uint64)u)			//Range  0=>18446744073709551615
/*This parameter shall be used to define a signal value which shall be compared (CanIcomSignalOperation) 
with the masked*/

#define CanIcomSignalRef		//Reference to [ ComSignal ]
/*This parameter defines a reference to the signal which shall be checked additional to the message id 
(CanIcomMessageId).*/

/****************************************************/

#endif
