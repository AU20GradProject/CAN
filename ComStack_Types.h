#ifndef	COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_

#include "Std_Types.h"
#include "ComStack_Cfg.h"
 /*PduIdType*/	
 
//Variables of this type serve as a unique identifier of a PDU within a software module
#if (PduIdmax == PduIdmax_uint8 )
typedef uint8 PduIdType;
#elif  (PduIdmax == PduIdmax_uint16 )
typedef uint16 PduIdType;
#endif
/******************************/

/*PduLengthType*/		

//Variables of this type serve as length information of a PDU in bytes 
#if (PduLengthmax == PduLengthmax_unit8 )
typedef uint8 PduLengthType;
#elif (PduLengthmax == PduLengthmax_unit16 )
typedef uint16 PduLengthType;
#elif  (PduLengthmax == PduLengthmax_unit32 )
typedef uint32 PduLengthType;
#endif

/******************************/

/*PduInfoType*/

//Variables of this type shall be used to store the basic information about a PDU of any type.
typedef struct{
uint8*			SduDataPtr;//Pointer to the SDU(Service data unit) (i.e. payload data) of the PDU.
uint8*			MetaDataPtr;//Pointer to the meta data (e.g. CAN ID, socket ID, diagnostic addresses) of the PDU.
PduLengthType	SduLength;//Length of the SDU(Service data unit) in bytes
}PduInfoType;

/******************************/

/*PNCHandleType*/

typedef uint8 PNCHandleType;	//Used to store the identifier of a partial network cluster.

/******************************/
		
/*TPParameterType*/		//Specify the parameter to which the value has to be changed (BS or STmin).
#define	TP_STMIN	0x00u // Separation Time
#define	TP_BS		0x01u // Block Size
#define	TP_BC		0x02u // The Band width control parameter used in FlexRay transport protocol module.

/******************************/

/*BufReq_ReturnType*/		//Variables of this type shall be used to store the result of a buffer request.
#define BUFREQ_OK		   0x00u //Buffer request accomplished successful.
#define BUFREQ_E_NOT_OK    0x01u //Buffer request not successful. Buffer cannot be accessed. 
#define BUFREQ_E_BUSY      0x02u //Temporarily no buffer available. It's up the requester to retry request for a certain time.
#define BUFREQ_E_OVFL	   0x03u //No Buffer of the required length can be provided. 

/******************************/

/*TpDataStateType*/			//Variables of this type shall be used to store the state of TP (transport protocol)buffer.
typedef uint8 TpDataStateType;
#define TP_DATACONF			0x00u //TP_DATACONF indicates that all data, that have been copied so far, are confirmed and can be removed from the TP buffer. Data copied by this API call are excluded and will be confirmed later.
#define TP_DATARETRY        0x01u //TP_DATARETRY indicates that this API call shall copyalready copied data in order to recover from an error. In this case TxTpDataCnt specifies the offset of the first byte to be copied by the API call.
#define TP_CONFPENDING      0x02u //TP_CONFPENDING indicates that the previously copied data must remain in the TP.

/******************************/

/*RetryInfoType*/		//Variables of this type shall be used to store the information about Tp buffer handling.

typedef struct{
TpDataStateType	TpDataState;	//The enum type to be used to store the state of Tp buffer.
PduLengthType	TxTpDataCnt;	//Offset from the current position which identifies the number of bytes to be retransmitted
}RetryInfoType;

/******************************/

/*NetworkHandleType*/	//Variables of the type NetworkHandleType shall be used to store the identifier of a communication channel.

typedef uint8	NetworkHandleType	;

/******************************/

/*IcomConfigIdType*/	//IcomConfigIdType defines the configuration ID. An ID of 0 is the default configuration. An ID greater than 0 shall identify a configuration for Pretended Networking. There is more than 1 configuration possible.

typedef uint8 IcomConfigIdType ;

/******************************/

/*IcomSwitch_ErrorType*/	//IcomSwitch_ErrorType defines the errors which can occur when activating or deactivating Pretended Networking.

#define ICOM_SWITCH_E_OK		0x00u	//The activation of Pretended Networking was successful.	
#define ICOM_SWITCH_E_FAILED	0x01u	//The activation of Pretended Networking was not successful.

/******************************/

#endif
