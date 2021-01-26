#include "player_unit.h"
#include "AZTEC_relays.h"
#include "ball_count_unit.h"
#include "coin_unit.h"
#include "credit_unit.h"


uint8_t playerUnitDiskPos = PLAYER_UNIT_DISK_POS_INDEX;

struct mb1 sw_playerUnitZeroBk = { SWITCH_TYPE_BREAK | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitReset) } };
struct mb1 sw_playerUnitZeroBk2 = { SWITCH_TYPE_BREAK | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_gameOverTrip)} };

struct mb1 sw_playerUnitD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_coinUnitWiperD6_2) } };
struct mb1 sw_playerUnitC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_coinUnitWiperD6_3) } };
struct mb1 sw_playerUnitB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballCountUnitStepUp) } };

void playerUnitStepUpTriggerFunc(uint8_t state)
{
		if(state == TRIGGER_STATE_OFF)
			return;
		
		++playerUnitDiskPos;
		switch(playerUnitDiskPos){
				case PLAYER_UNIT_DISK_POS_1:
						recursionBreaker(toSwitchPtr(&sw_playerUnitZeroBk), 0);
						recursionBreaker(toSwitchPtr(&sw_playerUnitZeroBk2), 0);
						recursionBreaker(toSwitchPtr(&sw_playerUnitD), 1);
						break;
				case PLAYER_UNIT_DISK_POS_2:
						recursionBreaker(toSwitchPtr(&sw_playerUnitD), 0);
						recursionBreaker(toSwitchPtr(&sw_playerUnitC), 1);
						break;
				case PLAYER_UNIT_DISK_POS_3:
						recursionBreaker(toSwitchPtr(&sw_playerUnitC), 0);
						recursionBreaker(toSwitchPtr(&sw_playerUnitB), 1);
						break;
				default:
					break;
		}
}

void playerUnitResetTriggerFunc(uint8_t state)
{
		if(!state || !playerUnitDiskPos)
			return;
	
		playerUnitDiskPos = PLAYER_UNIT_DISK_POS_INDEX;

		recursionBreaker(toSwitchPtr(&sw_playerUnitZeroBk), 1);
		recursionBreaker(toSwitchPtr(&sw_playerUnitZeroBk2), 1);
		recursionBreaker(toSwitchPtr(&sw_playerUnitB), 0);
		sw_powerOff(toSwitchPtr(&sw_playerUnitB));
		sw_playerUnitB.holdCount = 0;
		recursionBreaker(toSwitchPtr(&sw_playerUnitC), 0);
		sw_powerOff(toSwitchPtr(&sw_playerUnitC));
		sw_playerUnitC.holdCount = 0;
		recursionBreaker(toSwitchPtr(&sw_playerUnitD), 0);
		sw_powerOff(toSwitchPtr(&sw_playerUnitD));
		sw_playerUnitD.holdCount = 0;

		switchTraceOff(toSwitchPtr(&relay_ballCountUnitStepUp));
}

trigger playerUnitStepUpTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &playerUnitStepUpTriggerFunc };
trigger playerUnitResetTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &playerUnitResetTriggerFunc };

relay1 relay_playerUnitStepUp = {
		RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
		(0 << RELAY_INDEX_REGISTER) | (PLAYER_STEP_UP << RELAY_INDEX_PIN),
		0,
		{
				toSwitchPtr(&playerUnitStepUpTrigger)
		}
};

relay1 relay_playerUnitReset = {
		RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
		(0 << RELAY_INDEX_REGISTER) | (PLAYER_RESET << RELAY_INDEX_PIN),
		0,
		{
				toSwitchPtr(&playerUnitResetTrigger)
		}
};
