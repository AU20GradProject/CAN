#ifndef DET_H

#define DET_H



#include "Std_Types.h"

/*********************************************************************/


/* Published information */
#define Can_MODULE_ID 0x015u
#define Can_VENDOR_ID 0x002Bu

#define Can_AR_RELEASE_MAJOR_VERSION 0x01u
#define Can_AR_RELEASE_MINOR_VERSION 0x00u
#define Can_AR_RELEASE_REVISION_VERSION 0x00u

#define Can_SW_MAJOR_VERSION 0x04u
#define Can_SW_MINOR_VERSION 0x03u
#define Can_SW_PATCH_VERSION 0x01u


/*********************************************************************/
FUNC(Std_ReturnType, Can_CODE ) Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId ) ;





#endif // DET_H
