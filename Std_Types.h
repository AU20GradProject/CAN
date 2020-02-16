#ifndef STD_TYPES_H_ /*Guard for the Std_Types.h*/
#define STD_TYPES_H_

#include "Platform_Types.h"
#include "Compiler.h"


/* Std_ReturnType*/ 	// 	0=> E_OK 		1=>E_NOT_OK

typedef uint8 Std_ReturnType;
/* we should notic that E_OK (only) is defined in the OSEK and to avoid name clashes
 and redefinition problems it should be implemanted as follows*/
 #ifndef STATUSTYPEDEFINED 
 #define STATUSTYPEDEFINED
 #define E_OK 0x00u 
 typedef unsigned char StatusType; /* OSEK compliance */ 
 #endif
 #define E_NOT_OK 0x01u	//outside the If because it'snt implemented in the OSEK

 				/************************************************/	

/*Std_VersionInfoType*/

/*This type shall be used to request the version of a BSW module
 using the <Module name>_GetVersionInfo() function.*/
typedef struct{
	uint16  vendorID;
	uint16  moduleID;
	uint8   sw_major_version;
	uint8   sw_minor_version;
	uint8   sw_patch_version;
}Std_VersionInfoType;

				/************************************************/	

/*STD_HIGH, STD_LOW*/

#define STD_HIGH 0x01u /* Physical state 5V or 3.3V */ 
#define STD_LOW 0x00u /* Physical state 0V */

				/************************************************/					

/*STD_ACTIVE, STD_IDLE*/

#define STD_ACTIVE 0x01u /* Logical state active */ 
#define STD_IDLE 0x00u /* Logical state idle */

				/************************************************/					

/*STD_ON, STD_OFF*/

#define STD_ON 0x01u
#define STD_OFF 0x00u

				/************************************************/	

#endif /*End STD_TYPES_H_*/
