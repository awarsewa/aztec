//
//  credit_unit.c
//  aztec
//
//  Created by Alexander Warsewa on 17.11.17.
//

#include "credit_unit.h"
#include "AZTEC_relays.h"
#include "ball_count_unit.h"
#include "coin_unit.h"
#include "player_unit.h"

uint8_t creditUnitCount = 0;

struct mb1 sw_creditUnitResetEOS = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_coin) } };
struct mb4 sw_creditUnitZeroBk = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 4, 0,
    {
        toSwitchPtr(&sw_coinUnitLastPosBk),
        toSwitchPtr(&sw_relayGameOverB),
        toSwitchPtr(&sw_ballCountUnitZeroBk),
        toSwitchPtr(&sw_playerUnitZeroBk)
    }
};
struct mb1 sw_creditUnitOpenAtLastPos = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitStepUp) } };
struct mb1 sw_creditUnitOpenAtLastPos2 = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };

relay0 relay_creditUnitReset = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (CREDIT_RESET << RELAY_INDEX_PIN),
    0,
    { NULL }
};

relay1 relay_creditUnitStepUp = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (CREDIT_STEP_UP << RELAY_INDEX_PIN),
    0,
    {
    	toSwitchPtr(&relay_knocker)
    }
};


