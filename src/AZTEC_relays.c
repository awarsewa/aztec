//
//  AZTEC_relays.cpp
//  aztec
//
//  Created by Alexander Warsewa on 29.10.17.
//

#include "AZTEC_relays.h"
#include "ball_count_unit.h"
#include "bonus_unit.h"
#include "coin_unit.h"
#include "credit_unit.h"
#include "player_unit.h"
#include "score_motor.h"

// Relays are called 'red' relays if they connect to the power line (labelled red in the schematic)
// that can be shut down by reset, tilt or game over relays
relay* red_relays[NUM_RED_RELAYS] = {
    toRelayPtr(&relay_doubleBonus),
    toRelayPtr(&relay_ballRelease),
    toRelayPtr(&relay_A),
    toRelayPtr(&relay_Z),
    toRelayPtr(&relay_T),
    toRelayPtr(&relay_E),
    toRelayPtr(&relay_C),
    toRelayPtr(&relay_ballIndex),
    toRelayPtr(&relay_outhole),
    toRelayPtr(&relay_gameOverTrip),
    toRelayPtr(&relay_centerTarget),
    toRelayPtr(&relay_change),
    toRelayPtr(&relay_setUp),
    toRelayPtr(&relay_advance),
    toRelayPtr(&relay_bonusUnitStepUp),
    toRelayPtr(&relay_5000),
    toRelayPtr(&relay_10Pt),
    toRelayPtr(&relay_100Pt),
    toRelayPtr(&relay_1000Pt),
    toRelayPtr(&relay_10000Pt),
    toRelayPtr(&relay_extraBall)
};

relay0 relay_dummy = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    { NULL }
};

relay0 shooterCoil = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (4 << RELAY_INDEX_REGISTER) | (SHOOTER_COIL << RELAY_INDEX_PIN),
    0,
    { NULL }
};


struct mb1 sw_relayDoubleBonusA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_doubleBonus) } };
struct mb1 sw_relayDoubleBonusC = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayBonus1B) } };
struct mb1 sw_relayDoubleBonusC2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, {toSwitchPtr(&sw_relayBonus1B) } };
struct mb2 sw_relayDoubleBonusD = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                    { toSwitchPtr(&relay_1000Pt), toSwitchPtr(&relay_10000Pt) } };

relay4 relay_doubleBonus = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (3 << RELAY_INDEX_REGISTER) | (DOUBLE_BONUS << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayDoubleBonusA),
        toSwitchPtr(&sw_relayDoubleBonusC),
        toSwitchPtr(&sw_relayDoubleBonusC2),
        toSwitchPtr(&sw_relayDoubleBonusD)
    }
};

struct mb1 sw_relayShooterA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_shooter) } };
struct mb1 sw_relayShooterB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&shooterCoil) } };
struct mb1 sw_relayShooterC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_10000Pt) } };
struct mb1 sw_relayShooterD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayShooterE = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb1 sw_relayShooterF = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_doubleBonus) } };

// TODO: back to simulated
relay relay_shooter = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (6 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        // A: In hold circuit to this re. (E14)
        toSwitchPtr(&sw_relayShooterA),
        // B: Pulses shooter coil through switch on CAM4 (E14)
        toSwitchPtr(&sw_relayShooterB),
        // C: In circuit to 10.000 pt. re. (E20)
        toSwitchPtr(&sw_relayShooterC),
        // D: In circuit to 100 pt. re. (E20)
        toSwitchPtr(&sw_relayShooterD),
        // E: Runs score motor (C4)
        toSwitchPtr(&sw_relayShooterE),
        // F: Energizes double bonus re. through switch on Z re. (E17)
        toSwitchPtr(&sw_relayShooterF)
    }
};


struct mb1 sw_relayBonus2A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonus) } };
struct mb1 sw_relayBonus2B = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_outhole) } };
struct mb1 sw_relayBonus2C = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relaySetUpB) } };

relay3 relay_bonus2 = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (3 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        // 2A: Hold circuit for this re. (E14)
        toSwitchPtr(&sw_relayBonus2A),
        // 2B: In series with bonus unit zero pos. sw., energizes outhole re. (E14)
        toSwitchPtr(&sw_relayBonus2B),
        // 2C: In series with switch 1B on set-up re. (E19)
        toSwitchPtr(&sw_relayBonus2C)
    }
};

struct mb1 sw_relayBonus1A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayDoubleBonusD) } };
struct mb1 sw_relayBonus1B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonusUnitReset) } };
struct mb1 sw_relayBonus1C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayResetA) } };
struct mb1 sw_relayBonus1D = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb1 sw_relayBonus2  = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_bonus2) } };

// TODO: back to simulated
relay5 relay_bonus = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        // 1A: In series with switch D on double bonus re. (D19)
        toSwitchPtr(&sw_relayBonus1A),
        // 1B: In circuit to bonus unit reset re. (E13)
        toSwitchPtr(&sw_relayBonus1B),
        // 1C: In hold circuit to reset re. (C5)
        toSwitchPtr(&sw_relayBonus1C),
        // 1D: Runs score motor (C4)
        toSwitchPtr(&sw_relayBonus1D),
        // Helper: energizes bonus re. 2
        toSwitchPtr(&sw_relayBonus2)
    }
};


relay0 relay_ballRelease = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (3 << RELAY_INDEX_REGISTER) | (BALL_RELEASE << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_GAME_OVER,
    { NULL }
};

struct mb2 sw_creditExtraballAdj = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT  | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0, { toSwitchPtr(&relay_creditUnitStepUp), toSwitchPtr(&relay_extraBall) }};
struct mb1 sw_featureAdjCons = { SWITCH_TYPE_MAKE | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayAB)} };



struct mb1 sw_centerJetBumper = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayChangeD) } };
struct mb1 sw_rightJetBumper = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayChangeE) } };
struct mb1 sw_leftJetBumper = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayChangeE) } };

struct mb2 sw_rightTopRollOver = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&sw_relayChangeA), toSwitchPtr(&relay_1000Pt) } };
struct mb2 sw_leftTopRollOver = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&sw_relayChangeA2), toSwitchPtr(&relay_1000Pt) } };
struct mb2 sw_leftTopTarget = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&sw_relayChangeA), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_rightBottomRollOverIn = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_5000) } };
struct mb1 sw_leftBottomRollOverIn = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_5000) } };
struct mb3 sw_rightBottomRollOverOut = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 3, 1, { toSwitchPtr(&relay_advance), toSwitchPtr(&sw_relaySetUpD), toSwitchPtr(&sw_relayChangeB) } };
struct mb3 sw_leftBottomRollOverOut = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 3, 1, { toSwitchPtr(&relay_advance), toSwitchPtr(&sw_relaySetUpD2), toSwitchPtr(&sw_relayChangeC) } };
struct mb1 sw_leftRightTopRollOverButton = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_100Pt) } };
struct mb1 sw_centerTopRollOverButton = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_advance) } };

struct mb1 sw_leftLaneRollOver = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayBallIndexF) } };
struct mb1 sw_leftLaneRollOver2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_doubleBonus) } };
struct mb1 sw_spinner = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayAF) } };

struct mb1 sw_shooter = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_shooter) } };
struct mb1 sw_outhole = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonus) } };

struct mb1 sw_creditButton = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_creditUnitZeroBk) } };

struct mb1 sw_noMatchAlt = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_change)}};

// coin switches
struct mb1 sw_5ct = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_coin) } };
struct mb1 sw_10ct = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_10ct) } };
struct mb2 sw_25ct = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                        { toSwitchPtr(&sw_relay25ct1D), toSwitchPtr(&relay_25ct) } };

struct mb1 sw_drumUnitZeroBk = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayResetA) } };

struct mb2 sw_targetC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&relay_C), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayCA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_C) } };
struct mb2 sw_relayCB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_featureAdjCons), toSwitchPtr(&relay_dummy) } };
struct mb2 sw_relayCC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_relayCenterTargetB), toSwitchPtr(&sw_relayShooterC) } };
struct mb2 sw_relayCD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_relayShooterD), toSwitchPtr(&sw_relayCenterTarget2B) } };

relay4 relay_C = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (C_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayCA),
        toSwitchPtr(&sw_relayCB),
        toSwitchPtr(&sw_relayCC),
        toSwitchPtr(&sw_relayCD)
    }
};


struct mb2 sw_targetE = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&relay_E), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayEA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_E) } };
struct mb1 sw_relayEB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayCB) } };
struct mb2 sw_relayEC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_relayCenterTargetB), toSwitchPtr(&sw_relayShooterC) } };
struct mb1 sw_relayED = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayCD) } };

relay4 relay_E = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (E_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayEA),
        toSwitchPtr(&sw_relayEB),
        toSwitchPtr(&sw_relayEC),
        toSwitchPtr(&sw_relayED)
    }
};

struct mb2 sw_targetT = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&relay_T), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayTA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_T) } };
struct mb2 sw_relayTB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
                        { toSwitchPtr(&sw_relayZF), toSwitchPtr(&sw_relayZB) } };
struct mb2 sw_relayTC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_relayCenterTargetB), toSwitchPtr(&sw_relayShooterC) } };
struct mb1 sw_relayTD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayED) } };

relay4 relay_T = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (T_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayTA),
        toSwitchPtr(&sw_relayTB),
        toSwitchPtr(&sw_relayTC),
        toSwitchPtr(&sw_relayTD)
    }
};

struct mb2 sw_targetZ = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, { toSwitchPtr(&relay_Z), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayZA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_Z) } };
struct mb3 sw_relayZB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 3, 0,
                        { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relayEB) } };
struct mb2 sw_relayZC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                        { toSwitchPtr(&sw_relayCenterTargetB), toSwitchPtr(&sw_relayShooterC) } };
struct mb1 sw_relayZD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayTD) } };
struct mb2 sw_relayZF = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 2, 0,
    { toSwitchPtr(&sw_relayShooterF), toSwitchPtr(&relay_dummy) } };
struct mb2 sw_relayZF2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
    { toSwitchPtr(&sw_relayShooterF), toSwitchPtr(&sw_leftLaneRollOver2) } };

relay relay_Z = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (6 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (Z_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayZA),
        toSwitchPtr(&sw_relayZB),
        toSwitchPtr(&sw_relayZC),
        toSwitchPtr(&sw_relayZD),
        toSwitchPtr(&sw_relayZF),
        toSwitchPtr(&sw_relayZF2)
    }
};

struct mb2 sw_centerTopRollOver = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2 ,1, { toSwitchPtr(&relay_A), toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayAA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_A) } };
struct mb1 sw_relayAB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_setUp) } };
struct mb2 sw_relayAC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
                        { toSwitchPtr(&sw_relayCenterTargetB), toSwitchPtr(&sw_relayShooterC) } };
struct mb1 sw_relayAD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayZD) } };
struct mb2 sw_relayAF = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                        { toSwitchPtr(&relay_100Pt), toSwitchPtr(&relay_1000Pt) } };

relay5 relay_A = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (A_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayAA),
        toSwitchPtr(&sw_relayAB),
        toSwitchPtr(&sw_relayAC),
        toSwitchPtr(&sw_relayAD),
        toSwitchPtr(&sw_relayAF)
    }
};


struct mb1 sw_relayCoinSetUpA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_coinSetUp) } };
struct mb2 sw_relayCoinSetUpB = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy) } };
relay2 relay_coinSetUp = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relayCoinSetUpA),
        toSwitchPtr(&sw_relayCoinSetUpB)
    }
};

struct mb1 sw_relayBallIndexA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 2, { toSwitchPtr(&relay_ballIndex) } };
struct mb2 sw_relayBallIndexB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                                { toSwitchPtr(&sw_scoreMotorCAM_1C), toSwitchPtr(&sw_scoreMotorCAM_2B) } };
struct mb1 sw_relayBallIndexC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayExtraBallA) } };
struct mb1 sw_relayBallIndexD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayBonus1A) } };
struct mb1 sw_relayBallIndexE = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayCoinSetUpA) } };
struct mb2 sw_relayBallIndexF = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_advance) } };

relay relay_ballIndex = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (6 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayBallIndexA),
        toSwitchPtr(&sw_relayBallIndexB),
        toSwitchPtr(&sw_relayBallIndexC),
        toSwitchPtr(&sw_relayBallIndexD),
        toSwitchPtr(&sw_relayBallIndexE),
        toSwitchPtr(&sw_relayBallIndexF)
    }
};

struct mb1 sw_relayOutholeA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_outhole) } };
struct mb7 sw_relayOutholeB = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 6 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                                {
                                    toSwitchPtr(&sw_relayAA), toSwitchPtr(&sw_relayZA),
                                    toSwitchPtr(&sw_relayTA), toSwitchPtr(&sw_relayEA),
                                    toSwitchPtr(&sw_relayCA), toSwitchPtr(&sw_relayDoubleBonusA),
                                    toSwitchPtr(&sw_relayExtraBallD)
                                } };
struct mb7 sw_relayOutholeC = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 5 | (2 << SWITCH_INDEX_ALTERNATE), 1,
                                {   toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relay10PtB),
                                    toSwitchPtr(&sw_relay100PtB), toSwitchPtr(&sw_relay1000PtB),
                                    toSwitchPtr(&sw_scoreMotorCAM_3B), toSwitchPtr(&sw_scoreMotorCAM_4B) } };
struct mb2 sw_relayOutholeD = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                                { toSwitchPtr(&sw_relayBallIndexA), toSwitchPtr(&(scoreMotor.motor)) } };

relay4 relay_outhole = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_GAME_OVER,
    {
        // A: In hold circuit to this relay (C14)
        toSwitchPtr(&sw_relayOutholeA),
        // B: Opens in hold circuit to 'A', 'Z', 'T', 'E' and 'C' relays  (C16)
        //    and closes to ball release coil and bonus unit step up coil (C6)
        toSwitchPtr(&sw_relayOutholeB),
        // C: Opens in hold circuit to tilt re. (C15)
        //    and closes in series with switch 'D' on extra ball re. (C6)
        toSwitchPtr(&sw_relayOutholeC),
        // D: Opens in hold circuit to ball index re. (C15) and closes to run score motor (C3)
        toSwitchPtr(&sw_relayOutholeD)
    }
};


void gameOverATriggerFunc(uint8_t state)
{
    uint8_t i;
    if(state == TRIGGER_STATE_ON){
        gameOverFlag = 1;
        for(i = 0; i < NUM_RED_RELAYS; i++){
            if(rel_isOffOnGameOver(red_relays[i]))
            	switchOff(toSwitchPtr(red_relays[i]));
        }
    } else {
        gameOverFlag = 0;
    }
}

trigger trig_gameOverA = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &gameOverATriggerFunc };

struct mb1 sw_relayGameOverB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitReset) }  };
struct mb2 sw_relayGameOverC1 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0,
                                { toSwitchPtr(&relay_reset), toSwitchPtr(&sw_scoreMotorImpulseA) } };
struct mb1 sw_relayGameOverC2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_scoreMotorCAM_1B) } };
struct mb2 sw_relayGameOverD = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy) } };
    
relay5 relay_gameOver = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
    (1 << RELAY_INDEX_REGISTER) | (GAME_OVER << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&trig_gameOverA),
        toSwitchPtr(&sw_relayGameOverB),
        toSwitchPtr(&sw_relayGameOverC1),
        toSwitchPtr(&sw_relayGameOverC2),
        toSwitchPtr(&sw_relayGameOverD)
    }
};


void gameOverTripATriggerFunc(uint8_t state)
{
    if(state == TRIGGER_STATE_OFF)
        return;

    switchOn(toSwitchPtr(&relay_gameOver));
}

trigger trig_relayGameOverTripA = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &gameOverTripATriggerFunc };

relay1 relay_gameOverTrip = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&trig_relayGameOverTripA)
    }
};

void gameOverLatchATriggerFunc(uint8_t state)
{
    if(state == TRIGGER_STATE_OFF)
        return;

    switchOff(toSwitchPtr(&relay_gameOver));
}
trigger trig_relayGameOverLatchA = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &gameOverLatchATriggerFunc };
relay1 relay_gameOverLatch = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&trig_relayGameOverLatchA)
    }
};


struct mb1 sw_relayPlayerResetA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_playerReset) } };
struct mb1 sw_relayPlayerResetB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };
struct mb2 sw_relayPlayerResetC = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                    { toSwitchPtr(&relay_playerUnitStepUp), toSwitchPtr(&relay_playerUnitReset) } };
relay3 relay_playerReset = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (3 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relayPlayerResetA),
        toSwitchPtr(&sw_relayPlayerResetB),
        toSwitchPtr(&sw_relayPlayerResetC)
    }
};

void resetDTriggerFunc(uint8_t state)
{
    uint8_t i;
    
    if(state == TRIGGER_STATE_ON){
        resetFlag = 1;
    } else {
        resetFlag = 0;
        for(i = 0; i < NUM_RED_RELAYS; i++){
            if(rel_isOffOnGameOver(red_relays[i]) && rel_getHoldCount(red_relays[i]))
            	switchOn(toSwitchPtr(red_relays[i]));
        }
    }
}
trigger trig_resetD = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &resetDTriggerFunc };

struct mb3 sw_relayResetA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 3, 1,
    { toSwitchPtr(&relay_reset), toSwitchPtr(&sw_relayGameOverC2),
        toSwitchPtr(&sw_scoreMotorImpulseA) } };
struct mb1 sw_relayResetB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonus) } };
struct mb1 sw_relayResetC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_playerUnitReset) } };
struct mb2 sw_relayResetE = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                            { toSwitchPtr(&sw_scoreMotorCAM_IndexA), toSwitchPtr(&(scoreMotor.motor)) } };
struct mb5 sw_relayResetF = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (4 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_coinUnitStepUp), toSwitchPtr(&relay_coinUnitReset),
                                  toSwitchPtr(&relay_gameOverLatch), toSwitchPtr(&relay_ballCountUnitReset),
                                  toSwitchPtr(&sw_relayResetC) } };
relay relay_reset = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (6 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        // A1: In hold circuit to this re.
        toSwitchPtr(&sw_relayResetA),
        toSwitchPtr(&sw_relayResetB),
        toSwitchPtr(&sw_relayResetC),
        toSwitchPtr(&trig_resetD),
        toSwitchPtr(&sw_relayResetE),
        toSwitchPtr(&sw_relayResetF)
    }
};

struct mb1 sw_relayCoinA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_coin) } };
struct mb2 sw_relayCoinB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, 
														{ toSwitchPtr(&sw_relayGameOverC1), toSwitchPtr(&sw_scoreMotorCAM_1B) } };
struct mb2 sw_relayCoinC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1, 
														{ toSwitchPtr(&sw_playerUnitZeroBk2), toSwitchPtr(&sw_ballCountUnitZeroBk2) } };
struct mb1 sw_relayCoinD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb1 sw_relayCoinE = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_dummy) } };

relay5 relay_coin = {
		RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
		(0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
		0, 
		{
				toSwitchPtr(&sw_relayCoinA),
				toSwitchPtr(&sw_relayCoinB),
				toSwitchPtr(&sw_relayCoinC),
				toSwitchPtr(&sw_relayCoinD),
				toSwitchPtr(&sw_relayCoinE)
		}
};


struct mb1 sw_centerTarget = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_centerTarget) } };
struct mb1 sw_relayCenterTarget2A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_centerTarget) } };
struct mb1 sw_relayCenterTarget2B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_100Pt) } };
relay2 relay_centerTarget2 = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relayCenterTarget2A),
        toSwitchPtr(&sw_relayCenterTarget2B)
    }
};

struct mb1 sw_relayCenterTargetA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonusUnitStepUp) } };
struct mb1 sw_relayCenterTargetB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relayCenterTargetC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb1 sw_relayCenterTargetD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relaySetUpA) } };
struct mb1 sw_relayCenterTarget2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_centerTarget2) } };
relay5 relay_centerTarget = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayCenterTargetA),
        toSwitchPtr(&sw_relayCenterTargetB),
        toSwitchPtr(&sw_relayCenterTargetC),
        toSwitchPtr(&sw_relayCenterTargetD),
        toSwitchPtr(&sw_relayCenterTarget2)
    }
};

struct mb1 sw_relayChangeA = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_advance) } };
struct mb1 sw_relayChangeA2 = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_advance) } };
struct mb2 sw_relayChangeB = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_10000Pt), toSwitchPtr(&sw_bonusUnit50000) }};
struct mb2 sw_relayChangeC = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&sw_bonusUnit50000), toSwitchPtr(&relay_10000Pt) }};
struct mb2 sw_relayChangeD = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_1000Pt), toSwitchPtr(&relay_100Pt) } };
struct mb2 sw_relayChangeE = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_NOT_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 0,
                                { toSwitchPtr(&relay_100Pt), toSwitchPtr(&relay_1000Pt) } };

relay relay_change = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (6 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (CHANGE_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayChangeA),
        toSwitchPtr(&sw_relayChangeA2),
        toSwitchPtr(&sw_relayChangeB),
        toSwitchPtr(&sw_relayChangeC),
        toSwitchPtr(&sw_relayChangeD),
        toSwitchPtr(&sw_relayChangeE)
    }
};


struct mb1 sw_relaySetUpA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0,
                            { toSwitchPtr(&relay_extraBall) } };
struct mb1 sw_relaySetUpB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_creditExtraballAdj) } };
struct mb1 sw_relaySetUpD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayChangeC) } };
struct mb1 sw_relaySetUpD2 = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayChangeB) } };

relay4 relay_setUp = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (SETUP_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relaySetUpA),
        toSwitchPtr(&sw_relaySetUpB),
        toSwitchPtr(&sw_relaySetUpD),
        toSwitchPtr(&sw_relaySetUpD2)
    }
};


struct mb1 sw_relayAdvanceA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_advance) } };
struct mb1 sw_relayAdvanceB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_bonusUnitStepUp) } };

relay2 relay_advance = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 < RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayAdvanceA),
        toSwitchPtr(&sw_relayAdvanceB)
    }
};


struct mb1 sw_relay5000A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_5000) } };
struct mb1 sw_relay5000B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_1000Pt) } };
struct mb1 sw_relay5000C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };

relay3 relay_5000 = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (3 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relay5000A),
        toSwitchPtr(&sw_relay5000B),
        toSwitchPtr(&sw_relay5000C)
    }
};


relay0 relay_10000Pt = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (RELAY_10000_PTS << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        NULL
    }
};

struct mb1 sw_relay1000PtB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayExtraBallC) } };
struct mb1 sw_relay1000Pt2B = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relay100Pt2B) } };

relay2 relay_1000Pt = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (RELAY_1000_PTS << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relay1000PtB),
        toSwitchPtr(&sw_relay1000Pt2B)
    }
};

struct mb1 sw_relay100PtB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayExtraBallC) } };
struct mb1 sw_relay100Pt2B = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relay10Pt2C) } };

relay2 relay_100Pt = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (RELAY_100_PTS << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relay100PtB),
        toSwitchPtr(&sw_relay100Pt2B)
    }
};

struct mb1 sw_relay10PtB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayExtraBallC) } };
struct mb1 sw_relay10Pt2C = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayExtraBallA) } };

relay2 relay_10Pt = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relay10PtB),
        toSwitchPtr(&sw_relay10Pt2C)
    }
};

relay0 relay_No123Reset = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (NO_1_2_3_RESET << RELAY_INDEX_PIN),
    0,
    {
        NULL
    }
};

struct mb2 sw_relayExtraBallA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
                                { toSwitchPtr(&relay_extraBall), toSwitchPtr(&relay_dummy)} };
struct mb1 sw_relayExtraBallC = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballIndex) } };
struct mb1 sw_relayExtraBallD = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayBallIndexB) } };

relay3 relay_extraBall = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (3 << RELAY_INDEX_SWITCHES),
    (2 << RELAY_INDEX_REGISTER) | (EXTRA_BALL_RELAY << RELAY_INDEX_PIN),
    0 + RELAY_OFF_ON_TILT + RELAY_OFF_ON_GAME_OVER,
    {
        toSwitchPtr(&sw_relayExtraBallA),
        toSwitchPtr(&sw_relayExtraBallC),
        toSwitchPtr(&sw_relayExtraBallD)
    }
};

relay0 relay_knocker = {
    RELAY_TYPE_REAL_COIL | RELAY_STATE_OFF | (0 << RELAY_INDEX_SWITCHES),
    (4 << RELAY_INDEX_REGISTER) | (KNOCKER << RELAY_INDEX_PIN),
    0,
    { NULL }
};



struct mb1 sw_relay10ctA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_10ct)} };
struct mb1 sw_relay10ctB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_creditUnitStepUp) } };
struct mb1 sw_relay10ctC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb2 sw_relay10ctD = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 2, 1,
                            { toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relayBallIndexE) } };
relay4 relay_10ct = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relay10ctA),
        toSwitchPtr(&sw_relay10ctB),
        toSwitchPtr(&sw_relay10ctC),
        toSwitchPtr(&sw_relay10ctD)
    }
};


struct mb1 sw_relay25ct1A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_coinSetUp) } };
struct mb1 sw_relay25ct1B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_creditUnitStepUp) } };
struct mb1 sw_relay25ct1C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_dummy) } };
struct mb1 sw_relay25ct1D = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(scoreMotor.motor)) } };
struct mb1 sw_relay25ct2A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&(relay_25ct)) } };

relay5 relay_25ct = {
    RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
    (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
    0,
    {
        toSwitchPtr(&sw_relay25ct1A),
        toSwitchPtr(&sw_relay25ct1B),
        toSwitchPtr(&sw_relay25ct1C),
        toSwitchPtr(&sw_relay25ct1D),
        toSwitchPtr(&sw_relay25ct2A)
    }
};

/*
void init_aztec_relays()
{
    int i;
    relay* rel;
 
    for(i = 0; i < NUM_AZTEC_RELAYS; i++){
        rel = aztec_relay[i];
        if(rel_isRealCoil(rel)){
            outRegs[rel_getOutReg(rel)].data |= (rel_isOn(rel) << rel_getOutPin(rel));
            shiftReg_write(outRegs[rel_getOutReg(rel)]);
            shiftReg_store(outRegs[rel_getOutReg(rel)]);
        }
    }
 
    asm("nop");
}*/
