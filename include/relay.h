//
//  releay.h
//  aztec
//
//  Created by Alexander Warsewa on 21.01.16.
//
//

#ifndef aztec_relay_h
#define aztec_relay_h

#include <stddef.h>
#include <inttypes.h>
#include <avr/io.h>
#include "conf_aztec.h"
#include "SPI.h"

#define SWITCH_TYPE_UNUSED      0x00
#define SWITCH_TYPE_BREAK       0x01
#define SWITCH_TYPE_MAKE        0x02
#define SWITCH_TYPE_MAKE_BREAK  0x03
#define RELAY_TYPE_REAL_COIL    0x04
#define RELAY_TYPE_SIMULATED    0x05
#define SWITCH_TYPE_TRIGGER     0x06

#define SWITCH_MASK_TYPE        0x0F
#define SWITCH_INDEX_TYPE       0

#define SWITCH_STATE_OPEN       0x00
#define SWITCH_STATE_CLOSED     0x10
#define SWITCH_STATE_DEFAULT    0x00
#define SWITCH_STATE_ALTERNATE  0x10
#define SWITCH_MASK_STATE       0x10
#define SWITCH_INDEX_STATE      4

#define SWITCH_POWERED          0x20
#define SWITCH_NOT_POWERED      0x00
#define SWITCH_MASK_POWERED     0x20 
#define SWITCH_INDEX_POWERED    5

#define SWITCH_INDEX_ALTERNATE   4

#define RELAY_STATE_ON          0x10
#define RELAY_STATE_OFF         0x00
#define RELAY_MASK_STATE        0x10
#define RELAY_INDEX_STATE       4

#define RELAY_OFF_ON_GAME_OVER  0x80
#define RELAY_OFF_ON_TILT       0x40
#define RELAY_MASK_HOLD_COUNT   0x3F

#define TRIGGER_STATE_ON        0x10
#define TRIGGER_STATE_OFF       0x00
#define TRIGGER_MASK_STATE      0x10
#define TRIGGER_INDEX_STATE     4

#define RELAY_MASK_SWITCHES     0xE0
#define RELAY_INDEX_SWITCHES    5

#define RELAY_MASK_REGISTER     0x0F
#define RELAY_INDEX_REGISTER    0
#define RELAY_MASK_PIN          0xF0
#define RELAY_INDEX_PIN         4


#define NUM_OUTPUT_REGS         5

#define TRUE                    1
#define FALSE                   0

#define toRelayPtr(SW)      ((relay *)(SW))
#define toTriggerPtr(SW)    ((trigger *)(SW))
#define toSwitchPtr(SW)     ((struct mb*)(SW))

#define shiftReg_store(SR)  AZTEC_OUTPUT_PORT |= SR.storePin; AZTEC_OUTPUT_PORT &= ~SR.storePin
#define shiftReg_write(SR)  SPIwrite(SR.data); while(SPIbusy()); 

#define sw_getType(sw)              (sw->info & SWITCH_MASK_TYPE)
#define sw_getState(sw)             (sw->info & SWITCH_MASK_STATE)
#define sw_getNConnected(sw)        (sw->info2 & 0x0F)
#define sw_getNAlternate(sw)        ((sw->info2 & 0xF0) >> SWITCH_INDEX_ALTERNATE)
#define sw_setState(sw, state)      (sw->info = (sw->info & ~SWITCH_MASK_STATE) | state)
#define sw_isPowered(sw)            (sw->info & SWITCH_MASK_POWERED)
#define sw_powerOn(sw)              (sw->info |= (1 << SWITCH_INDEX_POWERED))
#define sw_powerOff(sw)             (sw->info &= ~(1 << SWITCH_INDEX_POWERED))

#define rel_getState(rel)           (rel->info & RELAY_MASK_STATE)
#define rel_setState(rel, state)    (rel->info = (rel->info & ~RELAY_MASK_STATE) | state)
#define rel_nSwitches(rel)          ((rel->info & RELAY_MASK_SWITCHES) >> RELAY_INDEX_SWITCHES)
#define rel_getOutReg(rel)          (rel->info2 & RELAY_MASK_REGISTER)
#define rel_getOutPin(rel)          ((rel->info2 & RELAY_MASK_PIN) >> RELAY_INDEX_PIN)
#define rel_isRealCoil(rel)         ((rel->info & SWITCH_MASK_TYPE) == RELAY_TYPE_REAL_COIL)
#define rel_isOn(rel)               ((rel->info & RELAY_MASK_STATE) == RELAY_STATE_ON)
#define rel_getHoldCount(rel)       ((rel->holdCount) & RELAY_MASK_HOLD_COUNT)
#define rel_isOffOnGameOver(rel)    ((rel->holdCount) & RELAY_OFF_ON_GAME_OVER)
#define rel_isOffOnTilt(rel)        (rel->holdCount & RELAY_OFF_ON_TILT)

struct mb {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[9];
};


struct mb1 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[1];
};

struct mb2 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[2];
};

struct mb3 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[3];
};

struct mb4 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[4];
};

struct mb5 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[5];
};

struct mb7 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[7];
};

struct mb9 {
    uint8_t info;
    uint8_t info2;
    uint8_t holdCount;
    struct mb* connected[9];
};

/*
 * Relay data type (separate type for each number of switches)
 */
typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[6];
} relay;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[5];
} relay5;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[4];
} relay4;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[3];
} relay3;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[2];
} relay2;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[1];
} relay1;

typedef struct {
    uint8_t  info;
    uint8_t  info2;
    uint8_t holdCount;
    struct mb* switches[1];
} relay0;

typedef struct {
    uint8_t info;
    void (*triggerFunc)(uint8_t);
} trigger;

typedef struct {
    uint8_t data;
    uint8_t storePin;
} shiftReg;


extern uint8_t tiltFlag;
extern uint8_t gameOverFlag;
extern uint8_t resetFlag;

extern shiftReg outRegs[NUM_OUTPUT_REGS];

void relay_init(void);

// Connect / disconnect switches
void switchOn(struct mb *sw);
void switchOff(struct mb *sw);

// Trace switches sourcing power to their destination coil
void switchTraceOn(struct mb *sw);
void switchTraceOff(struct mb *sw);

#endif
