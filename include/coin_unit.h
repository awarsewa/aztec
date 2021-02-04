//
//  coin_unit.h
//  aztec
//
//  Created by Alexander Warsewa on 02.11.17.
//

#ifndef _coin_unit_h_
#define _coin_unit_h_

#include "relay.h"

#define COIN_UNIT_REGISTER 	1
#define COIN_UNIT_REG_MASK	0x70

#define COIN_UNIT_DISK_POS_INDEX		    0
#define COIN_UNIT_DISK_POS_1				1
#define COIN_UNIT_DISK_POS_2				2
#define COIN_UNIT_DISK_POS_3				3

extern uint8_t coinUnitDiskPos;

extern struct mb1 sw_coinUnitWiperD6_1;
extern struct mb1 sw_coinUnitWiperD6_2;
extern struct mb1 sw_coinUnitWiperD6_3; 

extern struct mb1 sw_coinUnitWiperC8_1;
extern struct mb1 sw_coinUnitWiperC8_2;
extern struct mb1 sw_coinUnitWiperC8_3;

extern struct mb1 sw_coinUnitLastPosBk;

extern relay1 relay_coinUnitStepUp;
extern relay1 relay_coinUnitReset;
extern trigger coinUnitStepUpTrigger;
extern trigger coinUnitResetTrigger;
void coinUnitStepUpTriggerFunc(uint8_t state);
void coinUnitResetTriggerFunc(uint8_t state);

#endif /* _bonus_unit_h_ */
