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
            switchOff(toSwitchPtr(&sw_playerUnitZeroBk));
            switchOff(toSwitchPtr(&sw_playerUnitZeroBk2));
            switchOn(toSwitchPtr(&sw_playerUnitD));
            break;
        case PLAYER_UNIT_DISK_POS_2:
            switchOff(toSwitchPtr(&sw_playerUnitD));
            switchOn(toSwitchPtr(&sw_playerUnitC));
            break;
        case PLAYER_UNIT_DISK_POS_3:
            switchOff(toSwitchPtr(&sw_playerUnitC));
            switchOn(toSwitchPtr(&sw_playerUnitB));
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

    switchOn(toSwitchPtr(&sw_playerUnitZeroBk));
    switchOn(toSwitchPtr(&sw_playerUnitZeroBk2));
    switchOff(toSwitchPtr(&sw_playerUnitB));
    sw_powerOff(toSwitchPtr(&sw_playerUnitB));
    sw_playerUnitB.holdCount = 0;
    switchOff(toSwitchPtr(&sw_playerUnitC));
    sw_powerOff(toSwitchPtr(&sw_playerUnitC));
    sw_playerUnitC.holdCount = 0;
    switchOff(toSwitchPtr(&sw_playerUnitD));
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
