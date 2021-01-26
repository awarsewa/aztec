#ifndef _player_unit_h_
#define _player_unit_h_

#include "relay.h"

#define PLAYER_UNIT_DISK_POS_INDEX		    0
#define PLAYER_UNIT_DISK_POS_1				1
#define PLAYER_UNIT_DISK_POS_2				2
#define PLAYER_UNIT_DISK_POS_3				3

extern uint8_t playerUnitDiskPos;

extern struct mb1 sw_playerUnitD;
extern struct mb1 sw_playerUnitC;
extern struct mb1 sw_playerUnitB; 

extern struct mb1 sw_playerUnitZeroBk;
extern struct mb1 sw_playerUnitZeroBk2;

extern relay1 relay_playerUnitStepUp;
extern relay1 relay_playerUnitReset;
extern trigger playerUnitStepUpTrigger;
extern trigger playerUnitResetTrigger;
void playerUnitStepUpTriggerFunc(uint8_t state);
void playerUnitResetTriggerFunc(uint8_t state);

extern void recursionBreaker(struct mb* sw, uint8_t state);

#endif
