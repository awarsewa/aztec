//
//  bonus_unit.c
//  aztec
//
//  Created by Alexander Warsewa on 02.11.17.
//

#include "bonus_unit.h"
#include "AZTEC_relays.h"
#include "SPI.h"

uint8_t bonusUnitDiskPos = BONUS_UNIT_DISK_POS_0;

struct mb2 sw_bonusUnitZeroPos = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_ALTERNATE | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1, { toSwitchPtr(&sw_outhole), toSwitchPtr(&sw_relayBonus2B) } };
struct mb2 sw_bonusUnitOpenAtZero = { SWITCH_TYPE_BREAK | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
    { toSwitchPtr(&sw_relayResetB), toSwitchPtr(&sw_relayBonus2A) } };
struct mb1 sw_bonusUnitEndOfStroke = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayAdvanceA) }};
struct mb1 sw_bonusUnit50000 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relaySetUpB) } };

struct mb1 sw_relayBonusUnitStepUpA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&bonusUnitStepUpTrigger) } };

relay2 relay_bonusUnitStepUp = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayBonusUnitStepUpA),
        toSwitchPtr(&sw_bonusUnitEndOfStroke)
    }
};

struct mb1 sw_bonusUnitResetA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&bonusUnitResetTrigger)} };
	
relay1 relay_bonusUnitReset = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_bonusUnitResetA)
    }
};

trigger bonusUnitStepUpTrigger = {
    SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF,
    &bonusUnitStepUpTriggerFunc
};

trigger bonusUnitResetTrigger = {
    SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF,
    &bonusUnitResetTriggerFunc
};

void bonusUnitStepUpTriggerFunc(uint8_t state)
{
    if(!state || bonusUnitDiskPos == BONUS_UNIT_DISK_POS_50000)
        return;

    // Go one up
    ++bonusUnitDiskPos;
    
    // clear bonus unit part of shift register
    outRegs[BONUS_UNIT_REGISTER].data &= ~BONUS_UNIT_REG_MASK;
    
    // enable / disable lamps according to disk position (see EAGLE schematic for details)
    switch(bonusUnitDiskPos){
        case BONUS_UNIT_DISK_POS_5000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_1);
            switchOff(toSwitchPtr(&sw_bonusUnitZeroPos));
            switchOff(toSwitchPtr(&sw_bonusUnitOpenAtZero));
            break;
        case BONUS_UNIT_DISK_POS_10000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_2) | (1 << BONUS_3) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_15000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_2);
            break;
        case BONUS_UNIT_DISK_POS_20000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_3) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_25000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_3);
            break;
        case BONUS_UNIT_DISK_POS_30000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_2) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_35000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_4);
            break;
        case BONUS_UNIT_DISK_POS_40000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_2) | (1 << BONUS_3) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_45000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_50000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_2) | (1 << BONUS_3) | (1 << BONUS_4);
            switchOn(toSwitchPtr(&sw_bonusUnit50000));
            break;
        default:
            break;
    }
    
    // Write and store new shift register values
    shiftReg_write(outRegs[BONUS_UNIT_REGISTER]);
    shiftReg_store(outRegs[BONUS_UNIT_REGISTER]);
}

void bonusUnitResetTriggerFunc(uint8_t state)
{
    if(!state || bonusUnitDiskPos == BONUS_UNIT_DISK_POS_0)
        return;
  
    --bonusUnitDiskPos;
    
    outRegs[BONUS_UNIT_REGISTER].data &= ~BONUS_UNIT_REG_MASK;
    
    switch(bonusUnitDiskPos){
        case BONUS_UNIT_DISK_POS_0:
            switchOn(toSwitchPtr(&sw_bonusUnitZeroPos));
            switchOn(toSwitchPtr(&sw_bonusUnitOpenAtZero));
            break;
        case BONUS_UNIT_DISK_POS_5000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_1);
            break;
        case BONUS_UNIT_DISK_POS_10000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_2) | (1 << BONUS_3) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_15000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_2);
            break;
        case BONUS_UNIT_DISK_POS_20000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_3) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_25000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_3);
            break;
        case BONUS_UNIT_DISK_POS_30000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_2) | (1 << BONUS_4) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_35000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_4);
            break;
        case BONUS_UNIT_DISK_POS_40000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_GND) | (1 << BONUS_1) | (1 << BONUS_2) | (1 << BONUS_3) | (1 << BONUS_5);
            break;
        case BONUS_UNIT_DISK_POS_45000:
            outRegs[BONUS_UNIT_REGISTER].data |= (1 << BONUS_5);
            switchOff(toSwitchPtr(&sw_bonusUnit50000));
            break;
        default:
            break;
    }
    
    shiftReg_write(outRegs[BONUS_UNIT_REGISTER]);
    shiftReg_store(outRegs[BONUS_UNIT_REGISTER]);
}
