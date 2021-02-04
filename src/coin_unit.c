#include "coin_unit.h"
#include "AZTEC_relays.h"
#include "ball_count_unit.h"
#include "credit_unit.h"

uint8_t coinUnitDiskPos = COIN_UNIT_DISK_POS_INDEX;

struct mb1 sw_coinUnitWiperD6_1 = { SWITCH_TYPE_MAKE | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballCountUnitStepUp) } };
struct mb1 sw_coinUnitWiperD6_2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballCountUnitStepUp) } };
struct mb1 sw_coinUnitWiperD6_3 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballCountUnitStepUp) } };

struct mb1 sw_coinUnitWiperC8_1 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };
struct mb1 sw_coinUnitWiperC8_2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };
struct mb1 sw_coinUnitWiperC8_3 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };

struct mb1 sw_coinUnitLastPosBk = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitReset) } };

void coinUnitStepUpTriggerFunc(uint8_t state)
{
    if(state == TRIGGER_STATE_OFF)
        return;
    if(coinUnitDiskPos >= COIN_UNIT_DISK_POS_3)
    	return;
    
    ++coinUnitDiskPos;

    outRegs[COIN_UNIT_REGISTER].data &= ~COIN_UNIT_REG_MASK;
    switch(coinUnitDiskPos){
        case COIN_UNIT_DISK_POS_1:
        	outRegs[COIN_UNIT_REGISTER].data |= (1 << CAN_PLAY_1_2);
            switchOff(toSwitchPtr(&sw_coinUnitWiperD6_1));
            switchOn(toSwitchPtr(&sw_coinUnitWiperD6_2));
            switchOn(toSwitchPtr(&sw_coinUnitWiperC8_1));
            break;
        case COIN_UNIT_DISK_POS_2:
        	outRegs[COIN_UNIT_REGISTER].data |= (1 << CAN_PLAY_3);
            switchOff(toSwitchPtr(&sw_coinUnitWiperD6_2));
            switchOn(toSwitchPtr(&sw_coinUnitWiperD6_3));
            switchOn(toSwitchPtr(&sw_coinUnitWiperC8_2));
            break;
        case COIN_UNIT_DISK_POS_3:
        	outRegs[COIN_UNIT_REGISTER].data |= (1 << CAN_PLAY_4);
            switchOff(toSwitchPtr(&sw_coinUnitWiperD6_3));
            switchOn(toSwitchPtr(&sw_coinUnitWiperC8_3));
            switchOn(toSwitchPtr(&sw_coinUnitLastPosBk));
            break;
        default:
            break;
    }
    shiftReg_write(outRegs[COIN_UNIT_REGISTER]);
    shiftReg_store(outRegs[COIN_UNIT_REGISTER]);
}

void coinUnitResetTriggerFunc(uint8_t state)
{
    if(!state || !coinUnitDiskPos)
        return;
    coinUnitDiskPos = COIN_UNIT_DISK_POS_INDEX;

    switchOff(toSwitchPtr(&sw_coinUnitWiperD6_2));
    sw_powerOff(toSwitchPtr(&sw_coinUnitWiperD6_2));
    sw_coinUnitWiperD6_2.holdCount = 0;
    switchOff(toSwitchPtr(&sw_coinUnitWiperD6_3));
    sw_powerOff(toSwitchPtr(&sw_coinUnitWiperD6_3));
    sw_coinUnitWiperD6_3.holdCount = 0;
    switchOff(toSwitchPtr(&sw_coinUnitWiperC8_1));
    switchOff(toSwitchPtr(&sw_coinUnitWiperC8_2));
    switchOff(toSwitchPtr(&sw_coinUnitWiperC8_3));
    sw_powerOff(toSwitchPtr(&sw_coinUnitWiperD6_1));
	sw_coinUnitWiperD6_1.holdCount = 0;
    switchOn(toSwitchPtr(&sw_coinUnitWiperD6_1));

    switchTraceOff(toSwitchPtr(&relay_ballCountUnitStepUp));


    outRegs[COIN_UNIT_REGISTER].data &= ~COIN_UNIT_REG_MASK;
    shiftReg_write(outRegs[COIN_UNIT_REGISTER]);
	shiftReg_store(outRegs[COIN_UNIT_REGISTER]);
}

trigger coinUnitStepUpTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &coinUnitStepUpTriggerFunc };
trigger coinUnitResetTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &coinUnitResetTriggerFunc };

relay1 relay_coinUnitStepUp = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&coinUnitStepUpTrigger)
    }
};

relay1 relay_coinUnitReset = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&coinUnitResetTrigger)
    }
};
