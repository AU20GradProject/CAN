/*
 * Can_Definitions.h
 *
 *  Created on: Jan 29, 2020
 *      Author: OWNER
 */

#ifndef CAN_DEFINITIONS_H_
#define CAN_DEFINITIONS_H_


/*****************Definitions***********************/
#define NumberOfHTH                             ((uint8)0x1u)       //Max number of hardware transmission handler

//Configuration Definitions
#define INTERRUPT                                   0x01u
#define POLLING                                     0x02u
#define MIXED                                       0x03u
#define False  0x00u
#define True   0x01u
#define STANDARD    0x15u
#define EXTENDED    0x20u

/*************************/

// Miscellaneous defines for Message ID Types// Miscellaneous defines for Message ID Types

//
//! This indicates that transmit interrupts are enabled.
//
#define MSG_OBJ_TX_INT_ENABLE   0x00000001

//
//! This indicates that receive interrupts are enabled.
//
#define MSG_OBJ_RX_INT_ENABLE   0x00000002

//
//! This indicates that a message object is using an extended identifier.
//
#define MSG_OBJ_EXTENDED_ID     0x00000004

//
//! This indicates that a message object is using filtering based on the
//! object's message identifier.
//
#define MSG_OBJ_USE_ID_FILTER   0x00000008

//
//! This indicates that new data was available in the message object.
//
#define MSG_OBJ_NEW_DATA        0x00000080

//
//! This indicates that data was lost since this message object was last
//! read.
//
#define MSG_OBJ_DATA_LOST       0x00000100

//
//! This indicates that a message object uses or is using filtering
//! based on the direction of the transfer.  If the direction filtering is
//! used, then ID filtering must also be enabled.
//
#define MSG_OBJ_USE_DIR_FILTER  (0x00000010 | MSG_OBJ_USE_ID_FILTER)

//
//! This indicates that a message object uses or is using message
//! identifier filtering based on the extended identifier.  If the extended
//! identifier filtering is used, then ID filtering must also be enabled.
//
#define MSG_OBJ_USE_EXT_FILTER  (0x00000020 | MSG_OBJ_USE_ID_FILTER)

//
//! This indicates that a message object is a remote frame.
//
#define MSG_OBJ_REMOTE_FRAME    0x00000040

//
//! This indicates that this message object is part of a FIFO structure and
//! not the final message object in a FIFO.
//
#define MSG_OBJ_FIFO            0x00000200

//
//! This indicates that a message object has no flags set.
//
#define MSG_OBJ_NO_FLAGS        0x00000000

//*****************************************************************************
#endif /* CAN_DEFINITIONS_H_ */
