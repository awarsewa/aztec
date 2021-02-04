#ifndef _ball_count_unit_h_
#define _ball_count_unit_h_

#include "relay.h"

#define BALL_COUNT_UNIT_REGISTER 	1
#define BALL_COUNT_UNIT_REG_MASK	0x0F

#define BALL_COUNT_UNIT_DISK_POS_1	1
#define BALL_COUNT_UNIT_DISK_POS_2	2
#define BALL_COUNT_UNIT_DISK_POS_3	3
#define BALL_COUNT_UNIT_DISK_POS_4	4
#define BALL_COUNT_UNIT_DISK_POS_5	5
#define BALL_COUNT_UNIT_DISK_POS_6	6

extern uint8_t ballCountUnitDiskPos;

extern struct mb1 sw_3_5_ballAjd;
extern struct mb1 sw_ballCountUnitEOS;
extern struct mb1 sw_ballCountUnitZeroBk;
extern struct mb1 sw_ballCountUnitZeroBk2;
extern struct mb1 sw_bonusUnit50000;

extern relay1 relay_ballCountUnitStepUp;
extern relay1 relay_ballCountUnitReset;
extern trigger ballCountUnitStepUpTrigger;
extern trigger ballCountUnitResetTrigger;
void ballCountUnitStepUpTriggerFunc(uint8_t state);
void ballCountUnitResetTriggerFunc(uint8_t state);

#endif
