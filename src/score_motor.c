//
//  score_motor.c
//  aztec
//
//  Created by Alexander Warsewa on 21.01.16.
//
//

#include "score_motor.h"
#include "AZTEC_relays.h"
#include "bonus_unit.h"
#include "coin_unit.h"
#include "credit_unit.h"
#include "player_unit.h"

volatile uint8_t motorFlag = 0;

// Motor 'relay'
struct mb1 sw_scoreMotorMotor = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&scoreMotorTrigger) } };

// CAM Index
// TODO: fill in coin re.
struct mb2 sw_scoreMotorCAM_IndexA = {  SWITCH_TYPE_MAKE | SWITCH_STATE_CLOSED | SWITCH_POWERED, 2, 1,
                                    { toSwitchPtr(&sw_creditButton), toSwitchPtr(&sw_relayCoinE) } };
struct mb2 sw_scoreMotorCAM_IndexB = {  SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_ALTERNATE | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                                        { toSwitchPtr(&sw_relayPlayerResetA), toSwitchPtr(&sw_relayAC) } };
struct mb3 sw_scoreMotorCAM_IndexC = {  SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_ALTERNATE | SWITCH_POWERED, 1 | (2 << SWITCH_INDEX_ALTERNATE), 1,
                                        { toSwitchPtr(&sw_relayResetA), toSwitchPtr(&sw_shooter), toSwitchPtr(&sw_bonusUnitZeroPos) } };
struct mb2 sw_scoreMotorCAM_IndexD = {  SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_ALTERNATE | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                                        { toSwitchPtr(&(scoreMotor.motor)), toSwitchPtr(&relay_dummy) } };

// CAM1
struct mb1 sw_scoreMotorCAM_1A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayZC) } };
struct mb2 sw_scoreMotorCAM_1B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 2, 0, 
                                 { toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relayResetF) } };
struct mb4 sw_scoreMotorCAM_1C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 4, 0,
                                    {   toSwitchPtr(&sw_playerUnitD), toSwitchPtr(&sw_playerUnitC),
                                        toSwitchPtr(&sw_playerUnitB), toSwitchPtr(&sw_coinUnitWiperD6_1) } };

struct mb1 sw_scoreMotorCAM_1D = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayCenterTargetA) } };
struct mb3 sw_scoreMotorCAM_1E = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 2 | (1 << SWITCH_INDEX_ALTERNATE), 1, { toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relayBallIndexE),  toSwitchPtr(&relay_dummy) } };
// CAM2
struct mb1 sw_scoreMotorCAM_2A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayTC) } };
struct mb1 sw_scoreMotorCAM_2B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&sw_relayPlayerResetC) } };
struct mb1 sw_scoreMotorCAM_2C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&relay_dummy) } };
struct mb2 sw_scoreMotorCAM_2D = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 2, 1,
                                    { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy) } };

// CAM3
struct mb1 sw_scoreMotorCAM_3A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayEC) } };
struct mb1 sw_scoreMotorCAM_3B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_bonusUnitStepUp) } };
struct mb1 sw_scoreMotorCAM_3C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_coinUnitWiperC8_1) } };
struct mb3 sw_scoreMotorCAM_3D = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 3, 1,
                                    { toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_scoreMotorCAM_2D), toSwitchPtr(&relay_dummy)} };

// CAM4
struct mb1 sw_scoreMotorCAM_4A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayCC) } };
struct mb1 sw_scoreMotorCAM_4B = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_ballRelease) } };
struct mb1 sw_scoreMotorCAM_4C = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_coinUnitWiperC8_2) } };
struct mb2 sw_scoreMotorCAM_4D = { SWITCH_TYPE_BREAK | SWITCH_STATE_CLOSED | SWITCH_POWERED, 2, 1,
                                    { toSwitchPtr(&sw_scoreMotorCAM_3D), toSwitchPtr(&relay_dummy) } };
struct mb2 sw_scoreMotorCAM_4E = { SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 1 | (1 << SWITCH_INDEX_ALTERNATE), 1,
                                    { toSwitchPtr(&sw_relayBallIndexA), toSwitchPtr(&sw_relayShooterB) } };

// CAM5
struct mb1 sw_scoreMotorCAM_5A = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayDoubleBonusC) } };
struct mb9 sw_scoreMotorCAM_5B = {
    SWITCH_TYPE_MAKE_BREAK | SWITCH_STATE_DEFAULT | SWITCH_POWERED, 8 | (1 << SWITCH_INDEX_ALTERNATE), 1,
    {
        toSwitchPtr(&sw_scoreMotorCAM_4D),
        // TODO: insert remaining 25¢ adj. 
        toSwitchPtr(&relay_dummy),
        toSwitchPtr(&sw_relayCoinA),
        toSwitchPtr(&sw_creditUnitResetEOS),
        toSwitchPtr(&sw_relayShooterA),
        toSwitchPtr(&sw_relayOutholeA),
        toSwitchPtr(&sw_relayCenterTarget2A),
        toSwitchPtr(&sw_relay5000A),
        toSwitchPtr(&sw_coinUnitWiperC8_3)
    }
};

struct mb1 sw_scoreMotorImpulseA = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_NOT_POWERED, 1, 0, { toSwitchPtr(&relay_No123Reset) } };
struct mb1 sw_scoreMotorImpulseB = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 1, 1, { toSwitchPtr(&sw_relayDoubleBonusC2) } };
struct mb5 sw_scoreMotorImpulseC = { SWITCH_TYPE_MAKE | SWITCH_STATE_OPEN | SWITCH_POWERED, 5, 1,
                                    { toSwitchPtr(&relay_dummy), toSwitchPtr(&relay_dummy), toSwitchPtr(&sw_relayBallIndexD),
                                        toSwitchPtr(&sw_relay5000B), toSwitchPtr(&sw_relayAD) } };

struct motor scoreMotor = {
    // Motor relay
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (1 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            // Calls a function to run / stop the motor
            toSwitchPtr(&sw_scoreMotorMotor)
        }
    },
    
    // CAM Index
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_ON | (4 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            // A: In circuit to credit unit reset and coin lockout re. (C3)
            toSwitchPtr(&sw_scoreMotorCAM_IndexA),
            // B: Pulses 100 / 1000 pt. re. (C20), Closes in hold circuit of player reset re. (C6-7)
            toSwitchPtr(&sw_scoreMotorCAM_IndexB),
            // C1: Pulses No. 1/2/3 reset re. (C5), Pulses shooter re. (D13)
            toSwitchPtr(&sw_scoreMotorCAM_IndexC),
            // D: 6 / 1 plays 25¢ (C7), runs score motor (C3)
            toSwitchPtr(&sw_scoreMotorCAM_IndexD)
        }
    },
    // CAM-1
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            toSwitchPtr(&sw_scoreMotorCAM_1A),
            toSwitchPtr(&sw_scoreMotorCAM_1B),
            toSwitchPtr(&sw_scoreMotorCAM_1C),
            toSwitchPtr(&sw_scoreMotorCAM_1D),
            toSwitchPtr(&sw_scoreMotorCAM_1E)
        }
    },
    // CAM-2
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            toSwitchPtr(&sw_scoreMotorCAM_2A),
            toSwitchPtr(&sw_scoreMotorCAM_2B),
            toSwitchPtr(&sw_scoreMotorCAM_2C),
            toSwitchPtr(&sw_scoreMotorCAM_2D)
        }
    },
    // CAM-3
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (4 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            // A: Pulses 1000 pt. or 10.000 pt. re.
            toSwitchPtr(&sw_scoreMotorCAM_3A),
            // B: Pulses bonus unit step up re.
            toSwitchPtr(&sw_scoreMotorCAM_3B),
            toSwitchPtr(&sw_scoreMotorCAM_3C),
            toSwitchPtr(&sw_scoreMotorCAM_3D)
        }
    },
    // CAM-4
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (5 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            // A: In series with C re. (C20)
            toSwitchPtr(&sw_scoreMotorCAM_4A),
            // B: Pulses ball release coil (C14-15)
            toSwitchPtr(&sw_scoreMotorCAM_4B),
            // C: Pulses 3rd player match (C7)
            toSwitchPtr(&sw_scoreMotorCAM_4C),
            // D: 4 plays 25¢ (C2)
            toSwitchPtr(&sw_scoreMotorCAM_4D),
            // E: Opens in hold circuit of ball release re. (), pulses shooter coil ()
            toSwitchPtr(&sw_scoreMotorCAM_4E)
        }
    },
    // CAM-5
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (2 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            // A: Pulses bonus unit reset re. (C13)
            toSwitchPtr(&sw_scoreMotorCAM_5A),
            // B:
            toSwitchPtr(&sw_scoreMotorCAM_5B)
        }
    },
    // Impulse
    {
        RELAY_TYPE_SIMULATED | RELAY_STATE_OFF | (3 << RELAY_INDEX_SWITCHES),
        (0 << RELAY_INDEX_REGISTER) | (0 << RELAY_INDEX_PIN),
        0,
        {
            toSwitchPtr(&sw_scoreMotorImpulseA),
            toSwitchPtr(&sw_scoreMotorImpulseB),
            toSwitchPtr(&sw_scoreMotorImpulseC)
        }
    }
};

trigger scoreMotorTrigger = { SWITCH_TYPE_TRIGGER | TRIGGER_STATE_OFF, &scoreMotorTriggerFunc };

void scoreMotorTriggerFunc(uint8_t state){
    if(state == TRIGGER_STATE_ON){
        scoreMotorStart();
    } else {
        scoreMotorStop();
    }
}

// Score motor
// runs with ~32 RPM
ISR(TIMER1_COMPA_vect)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        if(motorFlag)
            return;
        
        motorFlag = 1;
        TIFR1 |= (1 << OCF1A);
    }
}

void scoreMotorTurn()
{
    static uint8_t motorDiskPos = MOTOR_DISK_POS_CAM_INDEX;
    
    if(motorDiskPos >= MOTOR_DISK_POS_CAM_5_OFF)
        motorDiskPos = MOTOR_DISK_POS_CAM_INDEX;
    else
        ++motorDiskPos;
    
    switch(motorDiskPos){
        case MOTOR_DISK_POS_CAM_INDEX:
            switchOn(toSwitchPtr(&(scoreMotor.CAM_index)));
            break;
        case MOTOR_DISK_POS_CAM_INDEX_OFF:
            switchOff(toSwitchPtr(&(scoreMotor.CAM_index)));
            switchOn(toSwitchPtr(&(scoreMotor.impulse)));
            break;
        case MOTOR_DISK_POS_CAM_1:
            switchOff(toSwitchPtr(&(scoreMotor.impulse)));
            switchOn(toSwitchPtr(&(scoreMotor.CAM_1)));
            break;
        case MOTOR_DISK_POS_CAM_1_OFF:
            switchOn(toSwitchPtr(&(scoreMotor.impulse)));
            switchOff(toSwitchPtr(&(scoreMotor.CAM_1)));
            break;
        case MOTOR_DISK_POS_CAM_2:
            switchOff(toSwitchPtr(&(scoreMotor.impulse)));
            switchOn(toSwitchPtr(&(scoreMotor.CAM_2)));
            break;
        case MOTOR_DISK_POS_CAM_2_OFF:
            switchOn(toSwitchPtr(&(scoreMotor.impulse)));
            switchOff(toSwitchPtr(&(scoreMotor.CAM_2)));
            break;
        case MOTOR_DISK_POS_CAM_3:
            switchOff(toSwitchPtr(&(scoreMotor.impulse)));
            switchOn(toSwitchPtr(&(scoreMotor.CAM_3)));
            break;
        case MOTOR_DISK_POS_CAM_3_OFF:
            switchOff(toSwitchPtr(&(scoreMotor.CAM_3)));
            switchOn(toSwitchPtr(&(scoreMotor.impulse)));
            break;
        case MOTOR_DISK_POS_CAM_4:
            switchOff(toSwitchPtr(&(scoreMotor.impulse)));
            switchOn(toSwitchPtr(&(scoreMotor.CAM_4)));
            break;
        case MOTOR_DISK_POS_CAM_4_OFF:
            switchOn(toSwitchPtr(&(scoreMotor.impulse)));
            switchOff(toSwitchPtr(&(scoreMotor.CAM_4)));
            break;
        case MOTOR_DISK_POS_CAM_5:
            switchOff(toSwitchPtr(&(scoreMotor.impulse)));
            switchOn(toSwitchPtr(&(scoreMotor.CAM_5)));
            break;
        case MOTOR_DISK_POS_CAM_5_OFF:
            switchOff(toSwitchPtr(&(scoreMotor.CAM_5)));
            break;
        default:
            break;
    }
}
