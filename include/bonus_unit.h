//
//  bonus_unit.h
//  aztec
//
//  Created by Alexander Warsewa on 02.11.17.
//

#ifndef _bonus_unit_h_
#define _bonus_unit_h_

#include "relay.h"

#define BONUS_UNIT_REGISTER             3
#define BONUS_UNIT_REG_MASK             0x3F

#define BONUS_UNIT_DISK_POS_0           0
#define BONUS_UNIT_DISK_POS_5000        1
#define BONUS_UNIT_DISK_POS_10000       2
#define BONUS_UNIT_DISK_POS_15000       3
#define BONUS_UNIT_DISK_POS_20000       4
#define BONUS_UNIT_DISK_POS_25000       5
#define BONUS_UNIT_DISK_POS_30000       6
#define BONUS_UNIT_DISK_POS_35000       7
#define BONUS_UNIT_DISK_POS_40000       8
#define BONUS_UNIT_DISK_POS_45000       9
#define BONUS_UNIT_DISK_POS_50000       10

extern uint8_t bonusUnitDiskPos;

extern struct mb2 sw_bonusUnitZeroPos;

extern relay2 relay_bonusUnitStepUp;
extern relay1 relay_bonusUnitReset;
extern trigger bonusUnitStepUpTrigger;
extern trigger bonusUnitResetTrigger;
void bonusUnitStepUpTriggerFunc(uint8_t state);
void bonusUnitResetTriggerFunc(uint8_t state);

#endif /* _bonus_unit_h_ */
