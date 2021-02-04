#include "ball_count_unit.h"
#include "AZTEC_relays.h"
#include "credit_unit.h"

uint8_t ballCountUnitDiskPos = BALL_COUNT_UNIT_DISK_POS_1;

struct mb1 sw_3_5_ballAdj = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_gameOverTrip) }};
struct mb1 sw_ballCountUnitZeroBk = { SWITCH_TYPE_BREAK | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitReset) } };
struct mb1 sw_ballCountUnitZeroBk2 = { SWITCH_TYPE_BREAK | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_gameOverTrip)} }; 
struct mb1 sw_ballCountUnitEOS = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_playerReset) } };

struct mb1 sw_relayBallCountUnitStepUpA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&ballCountUnitStepUpTrigger) } };

relay1 relay_ballCountUnitStepUp = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relayBallCountUnitStepUpA)
    }
};

struct mb1 sw_relayBallCountUnitResetA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&ballCountUnitResetTrigger)} };

relay1 relay_ballCountUnitReset = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relayBallCountUnitResetA)
    }
};

trigger ballCountUnitStepUpTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &ballCountUnitStepUpTriggerFunc };
trigger ballCountUnitResetTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &ballCountUnitResetTriggerFunc };

void ballCountUnitStepUpTriggerFunc(uint8_t state)
{
    if(!state || ballCountUnitDiskPos == BALL_COUNT_UNIT_DISK_POS_6)
        return;
    
    ++ballCountUnitDiskPos;
    outRegs[BALL_COUNT_UNIT_REGISTER].data &= ~(BALL_COUNT_UNIT_REG_MASK);
    
    switch(ballCountUnitDiskPos){
        case BALL_COUNT_UNIT_DISK_POS_2:
            outRegs[BALL_COUNT_UNIT_REGISTER].data |= (1 << BALL_IN_PLAY_1_2);
            switchOff(toSwitchPtr(&sw_ballCountUnitZeroBk));
            switchOff(toSwitchPtr(&sw_ballCountUnitZeroBk2));
            break;
        case BALL_COUNT_UNIT_DISK_POS_3:
            outRegs[BALL_COUNT_UNIT_REGISTER].data |= (1 << BALL_IN_PLAY_3);
            break;
        case BALL_COUNT_UNIT_DISK_POS_4:
            outRegs[BALL_COUNT_UNIT_REGISTER].data |= (1 << BALL_IN_PLAY_4);
            switchOn(toSwitchPtr(&sw_3_5_ballAdj));
            break;
        case BALL_COUNT_UNIT_DISK_POS_5:
            outRegs[BALL_COUNT_UNIT_REGISTER].data |= (1 << BALL_IN_PLAY_5);
            switchOff(toSwitchPtr(&sw_3_5_ballAdj));
            switchOn(toSwitchPtr(&sw_3_5_ballAdj));
            break;
        case BALL_COUNT_UNIT_DISK_POS_6:
            switchOff(toSwitchPtr(&sw_3_5_ballAdj));
            
            relay_gameOverTrip.holdCount++;
            switchOn(toSwitchPtr(&relay_gameOverTrip));
            break;
        default:
            break;
    }

    shiftReg_write(outRegs[BALL_COUNT_UNIT_REGISTER]);
    shiftReg_store(outRegs[BALL_COUNT_UNIT_REGISTER]);
    
    // Ball count unit E.O.S.
    switchOn(toSwitchPtr(&sw_ballCountUnitEOS));
    switchOff(toSwitchPtr(&sw_ballCountUnitEOS));
}

void ballCountUnitResetTriggerFunc(uint8_t state)
{
    if(!state || ballCountUnitDiskPos == BALL_COUNT_UNIT_DISK_POS_1)
        return;
  
    ballCountUnitDiskPos = BALL_COUNT_UNIT_DISK_POS_1;

    switchOff(toSwitchPtr(&sw_3_5_ballAdj));
    
	switchOn(toSwitchPtr(&sw_ballCountUnitZeroBk));
	switchOff(toSwitchPtr(&sw_ballCountUnitZeroBk2));

	if(relay_gameOverTrip.holdCount)
		relay_gameOverTrip.holdCount--;
	switchOff(toSwitchPtr(&relay_gameOverTrip));

	outRegs[BALL_COUNT_UNIT_REGISTER].data &= ~(BALL_COUNT_UNIT_REG_MASK);
	shiftReg_write(outRegs[BALL_COUNT_UNIT_REGISTER]);
	shiftReg_store(outRegs[BALL_COUNT_UNIT_REGISTER]);
}
