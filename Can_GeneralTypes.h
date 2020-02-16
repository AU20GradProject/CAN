#ifndef	CAN_GENERALTYPES_H_
#define	CAN_GENERALTYPES_H_
/*Can_ConfigType*/

/*This is the type of the external data structure containing the overall initialization
 data for the CAN driver and SFR(specific function register)settings affecting all controllers.
 Furthermore it contains pointers to controller configuration structures.
 The contents of the initialization data structure are CAN hardware specific.*/


/**************/
/*Needed Defines */
#define Max_BaudeRateIDs    ((uint16)10)
typedef struct{
    uint16 BaudRateConfigIDs;   //Baude Rate ID
    uint16 BaudRate;            //Baud rate in Kbps
    uint8  TSEG2 ;              //Time Segment after Sample Pointing in quanta
    uint8  TSEG1 ;              //Time Segment before Sample Pointing in quanta
    uint8  SJW   ;              //(Re)Synchronization Jump Width in quanta

}BaudeRate_ConfigType;
/*****************/
typedef struct{
    uint8 Can_Controller;       //number of the can controller Can_Controller0,Can_Controller1
    uint8   Lenght;             //lenght of data in bytes
    uint8   Hth;                //number of the memory object to recieve in
}Can_ConfigType;

/**********************************/
/*Can_IdType*/

/*Represents the Identifier of L-PDU. The two most significant bits specify the frame type:
 00 CAN message with Standard CAN ID 
 01 CAN FD frame with Standard CAN ID       CAN Fd CAN (with Flexible Data-Rate) created by BOSCH
 10 CAN message with Extended CAN ID
 11 CAN FD frame with Extended CAN ID */
#if CanIdType==STANDARD //IDs is 11bit
typedef uint16 Can_IdType;
/*Standard32Bit 0..0x400007FF*/
#elif   CanIdType==EXTENDED     //IDs is 29bit
typedef uint32 Can_IdType;
/*Extended32Bit 0..0xDFFFFFFF*/
#endif
/**********************************/


/*Can_PduType*/
typedef struct {
    PduIdType	swPduHandle;        //ID of the PDU
    uint8		length;
    Can_IdType	id;
    uint8*		sdu;                //SDU Service Data unit
}Can_PduType;

/**********************************/

/*Can_HwHandleType*/

/*Represents the hardware object handles of a CAN hardware unit.
 For CAN hardware units with more than 255 HW objects use extended range.
 Memory objects in the Tiva C*/
typedef uint8 Can_HwHandleType;			//Standard 0..0x0FF
//typedef uint16 Can_HwHandleType;		//Extended 0..0xFFFF

/**********************************/

/*Can_HwType*/

/*This type defines a data structure which clearly provides an Hardware Object Handle
 including its corresponding CAN Controller and therefore CanDrv as well as the specific CanId*/ 

typedef struct{
    Can_IdType			CanId;	//Standard/Extended CAN ID of CAN L-PDU
    Can_HwHandleType	Hoh;	//ID of the corresponding Hardware Object Range
    uint8				ControllerId;	//ControllerId provided by CanIf clearly identify the corresponding controller
}Can_HwType;

/**********************************/

/*Can_ReturnType*/

/*Overlayed return value of Std_ReturnType for CAN driver API Can_Write()*/
#define CAN_BUSY 0x02u	
/* defined in the Std_ReturnType
#define E_OK 0x00u 
#define E_NOT_OK 0x01u
 */

/**********************************/
/*Can_ErrorStateType*/
typedef uint8 Can_ErrorStateType;

/*Error states of a CAN controller.*/
#define CAN_ERRORSTATE_ACTIVE		0x00u	//The CAN controller takes fully part in communication.
#define CAN_ERRORSTATE_PASSIVE      0x01u   //The CAN controller takes part in communication, but does not send active error frames.
#define CAN_ERRORSTATE_BUSOFF       0x02u   //The CAN controller does not take part in communication.

/**********************************/

/*Can_ControllerStateType*/
typedef uint8 Can_ControllerStateType;

/*States that are used by the several ControllerMode functions.*/
#define CAN_CS_UNINIT	0x00	//CAN controller state UNINIT.
#define CAN_CS_STARTED  0x01    //CAN controller state STARTED.
#define CAN_CS_STOPPED  0x02    //CAN controller state STOPPED.
#define CAN_CS_SLEEP    0x03    //CAN controller state SLEEP.

/**********************************/



#endif
