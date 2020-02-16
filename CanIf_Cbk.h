/*
 * CanIf_Cbk.h
 *
 *  Created on: Feb 14, 2020
 *      Author: OWNER
 */

#ifndef CANIF_CBK_H_
#define CANIF_CBK_H_



FUNC(void,Can_CODE)CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);

FUNC(void,Can_CODE)CanIf_ControllerModeIndication(uint8 ControllerId,Can_ControllerStateType ControllerMode);

FUNC(void,Can_CODE)CanIf_TxConfirmation(PduIdType CanTxPduId);


#endif /* CANIF_CBK_H_ */
