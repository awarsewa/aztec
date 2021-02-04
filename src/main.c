/* Name: main.c
 * Author: Alex
 * Copyright: 2016 by Alexander Warsewa
 * License:
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "SPI.h"
#include "AZTEC_relays.h"
#include "score_motor.h"
#include "ball_count_unit.h"
#include "player_unit.h"
#include "credit_unit.h"
#include "coin_unit.h"
#include "bonus_unit.h"
#include "queue.h"

#define enableZeroCrossInt()    GIFR |= (1 << INTF0); GICR |= (1 << INT0)
#define disableZeroCrossInt()   GICR &= ~(1 << INT0)
#define zeroCrossIntActive()    (GICR & (1 << INT0))

#define isBitSet(bit, reg)      ((1 << (bit)) & reg)
#define setBit(bit, reg)        reg |= (1 << (bit))
#define clearBit(bit, reg)      reg &= ~(1 << (bit))

#define isActive(src)           (AZTEC_EVENT_PIN & src)

/*
#define SWITCH_INFO_DEPTH       60

struct swInfo
{
    struct mb* on;
    struct mb* off;
    uint8_t    n_on;
    uint8_t    n_off;
    uint8_t    traceFlag;
};

struct swInfo switchInfo[SWITCH_INFO_DEPTH];
queue  swInfoQueue;
*/

//TODO: check zero crossing interrupt with oscilloscope

/*
 * AC zero crossing interrupt
 * Used for switching off inductive loads
 * Interrupt is enabled when a coil is to be switched off
 * Corresponding register is then latched (with the coil's output set to low before)
 * When called, interrupt is disabled right away to not generate too much overload
 */
/*
ISR(INT0_vect)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        // write data to register
        shiftReg_write(latchSR);
        // latch selected register
        shiftReg_store(latchSR);
        
        // disable interrupt
        disableZeroCrossInt();
    }
}*/

ISR(TIMER2_OVF_vect)
{
    static uint8_t statCount;
    
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        if(++statCount == 30){
            PORTD ^= (1 << PD7);
            statCount = 0;
        }
    }
}

void init_hardware(void)
{
    // Set all pins of input port to tri-state inputs
    AZTEC_INPUT_DDR = 0x00;
    AZTEC_INPUT_PORT = 0x00;
    
    AZTEC_OUTPUT_DDR = (1 << AZTEC_SPI_RCK1) | (1 << AZTEC_SPI_RCK2) | (1 << AZTEC_SPI_RCK3) | \
    (1 << AZTEC_SPI_RCK4) | ( 1 << AZTEC_SPI_RCK5) | (1 << AZTEC_SPI_MOSI) | \
    (1 << AZTEC_SPI_SCK);
    AZTEC_OUTPUT_PORT = 0x00;
    
    
    AZTEC_CONTROL_DDR = (1 << AZTEC_OUTPUT_ENABLE) | (1 << AZTEC_CLOCK_ENABLE) | (1 << AZTEC_PARALLEL_LOAD) | (1 << AZTEC_SERIAL_CLEAR);
    AZTEC_CONTROL_PORT = (1 << AZTEC_OUTPUT_ENABLE) | (1 << AZTEC_CLOCK_ENABLE) | (1 << AZTEC_PARALLEL_LOAD) | (1 << AZTEC_SERIAL_CLEAR);
    
    // Set all pins of event port to function as inputs (tri-state)
    AZTEC_EVENT_DDR = 0x00;
    AZTEC_EVENT_PORT = 0x00;
    
    // Auxiliary inputs are on control port
    
    // Set INT0 and INT1 to trigger on rising edge
    MCUCR = (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10);
    
    
    // TODO: init SPI
    SPCR = (1 << SPE) | (1 << MSTR) | (0 << CPOL) | (0 <<  CPHA);
    // enable fast mode
    SPSR = (1 << SPI2X);
    
    // Init relay.c (init queue)
    relay_init();

    outRegs[0].storePin = (1 << AZTEC_SPI_RCK1);
    outRegs[1].storePin = (1 << AZTEC_SPI_RCK2);
    outRegs[2].storePin = (1 << AZTEC_SPI_RCK3);
    outRegs[3].storePin = (1 << AZTEC_SPI_RCK4);
    outRegs[4].storePin = (1 << AZTEC_SPI_RCK5);
    
    // clear shift registers
    AZTEC_CONTROL_PORT &= ~(1 << AZTEC_SERIAL_CLEAR);
    _delay_ms(1);
    AZTEC_CONTROL_PORT |=  (1 << AZTEC_SERIAL_CLEAR);
    
    
    DDRD |= (1 << PD7);
    // Enable Timer2 for toggling the status LED
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);
    TIMSK2 = (1 << TOIE2);
    
    //set motor speed to ~32 rpm
    OCR1A = MOTOR_STEP_TIME;
	TCCR1A = 0x00;
    TIFR1 |= (1 << OCF1A); TIMSK1 |= (1 << OCIE1A);
}

void aztec_init(void)
{
    // Init to state before game over (5 balls in play)
    ballCountUnitDiskPos = BALL_COUNT_UNIT_DISK_POS_5;
    switchOff(toSwitchPtr(&sw_ballCountUnitZeroBk));
    switchOff(toSwitchPtr(&sw_ballCountUnitZeroBk2));
    // Trigger ball count unit step up to induce game over
    switchOn(toSwitchPtr(&relay_ballCountUnitStepUp));
    _delay_ms(50);
    switchOff(toSwitchPtr(&relay_ballCountUnitStepUp));
    
    // TODO:
    // read the adjust switch settings
    
    // setup connections of sw_5ct, sw_10ctm ... accordingly
    // including score motor CAMs (see A2 in the schematic)
    
    // read in credit unit state a.s.o from auxiliary input (nr. 5)
    
}

void processEvent(uint8_t src, uint8_t event, uint8_t flank)
{
    struct mb* sw = NULL;
    
    switch(src){
       case AZTEC_EVENT_SRC_GS1:
            switch(event){
                case L_LANE_ROLL_OVER:
                    if(flank)
                        switchOn(toSwitchPtr(&sw_leftLaneRollOver));
                    else
                        switchOff(toSwitchPtr(&sw_leftLaneRollOver));
                    sw = toSwitchPtr(&sw_leftLaneRollOver2);
                    break;
                case E_TARGET:
                    sw = toSwitchPtr(&sw_targetE);
                    break;
                case Z_TARGET:
                    sw = toSwitchPtr(&sw_targetZ);
                    break;
                case CENTER_JET_BUMPER:
                    sw = toSwitchPtr(&sw_centerJetBumper);
                    break;
                case R_JET_BUMPER:
                    sw = toSwitchPtr(&sw_rightJetBumper);
                    break;
                case L_JET_BUMPER:
                    sw = toSwitchPtr(&sw_leftJetBumper);
                    break;
                case CENTER_TOP_ROLL_OVER:
                    sw = toSwitchPtr(&sw_centerTopRollOver);
                    break;
                case R_TOP_ROLL_OVER:
                    sw = toSwitchPtr(&sw_rightTopRollOver);
                    break;
                case L_TOP_ROLL_OVER:
                    sw = toSwitchPtr(&sw_leftTopRollOver);
                    break;
                case CENTER_TARGET:
                    sw = toSwitchPtr(&sw_centerTarget);
                    break;
                case L_TOP_TARGET:
                    sw = toSwitchPtr(&sw_leftTopTarget);
                    break;
                case L_BOT_ROLL_OVER_IN:
                    sw = toSwitchPtr(&sw_leftBottomRollOverIn);
                    break;
                case L_BOT_ROLL_OVER_OUT:
                    sw = toSwitchPtr(&sw_leftBottomRollOverOut);
                    break;
                case L_R_TOP_ROLL_OVER_BUTTON:
                    sw = toSwitchPtr(&sw_leftRightTopRollOverButton);
                    break;
                case CENTER_TOP_ROLL_OVER_BUTTON:
                    sw = toSwitchPtr(&sw_centerTopRollOverButton);
                    break;
                case SPINNER:
                    sw = toSwitchPtr(&sw_spinner);
                    break;
                default:
                    break;
            }
            break;
        case AZTEC_EVENT_SRC_GS2:
            switch(event){
                case SHOOTER_SWITCH:
                    sw = toSwitchPtr(&sw_shooter);
                    _delay_ms(150);
                    break;
                case OUTHOLE_SWITCH:
                    sw = toSwitchPtr(&sw_outhole);
                    break;
                case CREDIT_BUTTON:
                    sw = toSwitchPtr(&sw_creditButton);
                    //sw = toSwitchPtr(&relay_ballRelease);
                    break;
                // When player scores 10 pts. no. match alternator switch triggers change re.
                case NUMBER_MATCH_S_U:
                    if(flank){
                        sw = toSwitchPtr(&sw_noMatchAlt);
                        if(sw_getState(toSwitchPtr(&sw_noMatchAlt)) == SWITCH_STATE_CLOSED)
                            flank = 0;
                        switchOn(toSwitchPtr(&relay_10Pt));
                        switchOff(toSwitchPtr(&relay_10Pt));
                    }
                    break;
                case C_TARGET:
                    sw = toSwitchPtr(&sw_targetC);
                    break;
                case T_TARGET:
                    sw = toSwitchPtr(&sw_targetT);
                    break;
                case R_BOT_ROLL_OVER_IN:
                    sw = toSwitchPtr(&sw_rightBottomRollOverIn);
                    break;
                case R_BOT_ROLL_OVER_OUT:
                    sw = toSwitchPtr(&sw_rightBottomRollOverOut);
                    break;
                /*
                case SWITCH_5_CT:
                    sw = toSwitchPtr(&sw_5ct);
                    break;
                case SWITCH_10_CT:
                    sw = toSwitchPtr(&sw_10ct);
                    break;
                case SWITCH_25_CT:
                    sw = toSwitchPtr(&sw_25ct);
                    break;
                */
                default:
                    break;
            }
            break;
        case AZTEC_EVENT_SRC_GS3:
            switch(event){
                case DRUM_UNIT_ZERO_BK_SWS:
                    sw = toSwitchPtr(&sw_drumUnitZeroBk);
                    flank = flank ? 0 : 1;
                    break;

                //case CREDIT_UNIT_ZERO_BK:
                //    sw = toSwitchPtr(&sw_creditUnitZeroBk);
                //    flank = flank ? 0 : 1;
                //    break;
                case CREDIT_UNIT_RESET_EOS:
                    sw = toSwitchPtr(&sw_creditUnitResetEOS);
                    break;
                default:
                    break;
            }
            break;
        // got 5¢
        case AZTEC_EVENT_SRC_BUT1:
            sw = toSwitchPtr(&sw_5ct);
            break;
        // got 10¢
        case AZTEC_EVENT_SRC_BUT2:
            sw = toSwitchPtr(&sw_10ct);
            break;
        // got 25¢
        case AZTEC_EVENT_SRC_BUT3:
            sw = toSwitchPtr(&sw_25ct);
            break;
        default:
            break;
    }
    if(sw != NULL){
        if(flank)
            switchOn(sw);
        else
            switchOff(sw);
    }
}

void sanityCheckGS1(uint8_t event)
{
    if(event != NO_EVENT)
        return;
    if(sw_getState(toSwitchPtr(&sw_centerTarget)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_centerTarget));
    if(sw_getState(toSwitchPtr(&sw_targetE)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_targetE));
    if(sw_getState(toSwitchPtr(&sw_targetZ)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_targetZ));
    if(sw_getState(toSwitchPtr(&sw_centerJetBumper)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_centerJetBumper));
    if(sw_getState(toSwitchPtr(&sw_rightJetBumper)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_rightJetBumper));
    if(sw_getState(toSwitchPtr(&sw_leftJetBumper)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftJetBumper));
    if(sw_getState(toSwitchPtr(&sw_centerTopRollOver)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_centerTopRollOver));
    if(sw_getState(toSwitchPtr(&sw_rightTopRollOver)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_rightTopRollOver));
    if(sw_getState(toSwitchPtr(&sw_leftTopRollOver)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftTopRollOver));
    if(sw_getState(toSwitchPtr(&sw_leftTopTarget)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftTopTarget));
    if(sw_getState(toSwitchPtr(&sw_leftBottomRollOverIn)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftBottomRollOverIn));
    if(sw_getState(toSwitchPtr(&sw_leftBottomRollOverOut)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftBottomRollOverOut));
    if(sw_getState(toSwitchPtr(&sw_leftRightTopRollOverButton)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_leftRightTopRollOverButton));
    if(sw_getState(toSwitchPtr(&sw_centerTopRollOverButton)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_centerTopRollOverButton));
    if(sw_getState(toSwitchPtr(&sw_leftLaneRollOver)) == SWITCH_STATE_CLOSED){
        switchOff(toSwitchPtr(&sw_leftLaneRollOver));
        switchOff(toSwitchPtr(&sw_leftLaneRollOver2));
    }
    if(sw_getState(toSwitchPtr(&sw_spinner)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_spinner));
    }

void sanityCheckGS2(uint8_t event)
{
    if(event != NO_EVENT)
        return;
    if(sw_getState(toSwitchPtr(&sw_shooter)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_shooter));
    if(sw_getState(toSwitchPtr(&sw_outhole)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_outhole));
    if(sw_getState(toSwitchPtr(&sw_creditButton)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_creditButton));
    if(sw_getState(toSwitchPtr(&sw_targetC)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_targetC));
    if(sw_getState(toSwitchPtr(&sw_targetT)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_targetT));
    if(sw_getState(toSwitchPtr(&sw_rightBottomRollOverIn)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_rightBottomRollOverIn));
    if(sw_getState(toSwitchPtr(&sw_rightBottomRollOverOut)) == SWITCH_STATE_CLOSED)
        switchOff(toSwitchPtr(&sw_rightBottomRollOverOut));
}

int main(void)
{
    uint8_t event;
    uint8_t currentEventGS1 = NO_EVENT;
    uint8_t higherPrioEventGS1 = NO_EVENT;
    uint8_t currentEventGS2 = NO_EVENT;
    uint8_t higherPrioEventGS2 = NO_EVENT;

    uint8_t eventRegGS3 = 0x00;
    uint8_t i = 0;
    
    motorFlag = 0;
    
    //disable watchdog
    //MCUCSR = 0;
    wdt_disable();
    
    cli();
    
    // init hardware
    init_hardware();
    
    // Init AZTEC relays
    //init_aztec_relays();
    
    //queue_init(&swInfoQueue, (uint8_t*)switchInfo, sizeof(struct swInfo), SWITCH_INFO_DEPTH);
    
    SPIwrite(0x00);
    while(SPIbusy());
    shiftReg_store(outRegs[0]);
    shiftReg_store(outRegs[1]);
    shiftReg_store(outRegs[2]);
    shiftReg_store(outRegs[3]);
    shiftReg_store(outRegs[4]);
    
    //enable shift register output
    AZTEC_CONTROL_PORT &= ~(1 << AZTEC_OUTPUT_ENABLE);
    
    // globally enable interrupts
    sei();
    
    // Init to game over state
    aztec_init();

    for(;;){
        
        // Event handling for GS1
        if(!isActive(AZTEC_EVENT_SRC_GS1) && currentEventGS1 != NO_EVENT){
            processEvent(AZTEC_EVENT_SRC_GS1, currentEventGS1, 0);
            currentEventGS1 = NO_EVENT;
        }
        if(isActive(AZTEC_EVENT_SRC_GS1) && (AZTEC_INPUT_PIN & 0x0F) != higherPrioEventGS1){
            // End higher prio event
            if(higherPrioEventGS1 != NO_EVENT){
                processEvent(AZTEC_EVENT_SRC_GS1, higherPrioEventGS1, 0);
                higherPrioEventGS1 = NO_EVENT;
            }
            if((AZTEC_INPUT_PIN & 0xF0) != currentEventGS1){
                // Wait a few ms until event becomes stable
                // GSx is slower to decay than the Qxx
                _delay_ms(10);
                // Still active?
                if(isActive(AZTEC_EVENT_SRC_GS1)){
                    // Get event
                    event = (uint8_t)(AZTEC_INPUT_PIN & 0x0F);
                    // Got anything new?
                    if(event > currentEventGS1){
                        higherPrioEventGS1 = event;
                        processEvent(AZTEC_EVENT_SRC_GS1, higherPrioEventGS1, 1);
                        // Else we got a new event
                    } else {
                        currentEventGS1 = event;
                        processEvent(AZTEC_EVENT_SRC_GS1, currentEventGS1, 1);
                    }
                    _delay_ms(25);
                }
            }
        }
        sanityCheckGS1(currentEventGS1);
        
        // Event handling for GS2
        if(!isActive(AZTEC_EVENT_SRC_GS2) && currentEventGS2 != NO_EVENT){
            processEvent(AZTEC_EVENT_SRC_GS2, currentEventGS2, 0);
            currentEventGS2 = NO_EVENT;
        }
        if(isActive(AZTEC_EVENT_SRC_GS2) && (AZTEC_INPUT_PIN & 0xF0) != higherPrioEventGS2){
            // End higher prio event
            if(higherPrioEventGS2 != NO_EVENT){
                processEvent(AZTEC_EVENT_SRC_GS2, higherPrioEventGS2, 0);
                higherPrioEventGS2 = NO_EVENT;
            }
            // Wait a few ms until event becomes stable
            // GSx is slower to decay than the Qxx
            if((AZTEC_INPUT_PIN & 0xF0) != currentEventGS2){
                _delay_ms(10);
                // Still active?
                if(isActive(AZTEC_EVENT_SRC_GS2)){
                    // Get event
                    event = (uint8_t)(AZTEC_INPUT_PIN & 0xF0);
                    // Got anything new?
                    if(event > currentEventGS2){
                        higherPrioEventGS2 = event;
                        processEvent(AZTEC_EVENT_SRC_GS2, higherPrioEventGS2, 1);
                        // Else we got a new event
                    } else {
                        currentEventGS2 = event;
                        processEvent(AZTEC_EVENT_SRC_GS2, currentEventGS2, 1);
                    }
                    _delay_ms(25);
                }
            }
        }
        sanityCheckGS2(currentEventGS2);
        
        // Detect changes on auxiliare inputs (5th IC)
        // NOTE: events can be shadowed by higher priority inputs...
        // Using a state register should help...
        if(isActive(AZTEC_EVENT_SRC_GS3)){
            if(!isBitSet(AZTEC_AUX_INPUT_PIN & AZTEC_AUX_INPUT_MASK, eventRegGS3)){
                _delay_us(200);
                
                // Get current auxiliary event
                event = AZTEC_AUX_INPUT_PIN & AZTEC_AUX_INPUT_MASK;
                
                // End all higher priority events
                for(i = event + 1; i < 8; i++){
                    if(isBitSet(i, eventRegGS3)){
                        clearBit(i, eventRegGS3);
                        processEvent(AZTEC_EVENT_SRC_GS3, i, 0);
                    }
                }
                // Activate current event
                setBit(event, eventRegGS3);
                processEvent(AZTEC_EVENT_SRC_GS3, event, 1);
            }
            // A shadowing event ended?
            if((1 << (AZTEC_AUX_INPUT_PIN & AZTEC_AUX_INPUT_MASK)) < eventRegGS3){
                _delay_us(200);
                event = AZTEC_AUX_INPUT_PIN & AZTEC_AUX_INPUT_MASK;
                for(i = event + 1; i < 8; i++){
                    if(isBitSet(i, eventRegGS3)){
                        clearBit(i, eventRegGS3);
                        processEvent(AZTEC_EVENT_SRC_GS3, i, 0);
                    }
                }
            }
        }
        
        // If there is any auxiliary event flag set but there there is no trigger
        if(eventRegGS3 && !isActive(AZTEC_EVENT_SRC_GS3)){
            // End all active events
            for(i = 0; i < 8; i++){
                if(isBitSet(i, eventRegGS3)){
                    clearBit(i, eventRegGS3);
                    processEvent(AZTEC_EVENT_SRC_GS3, i, 0);
                }
            }
        }

        if(motorFlag){
            motorFlag = 0;
            //_delay_ms(25);
            scoreMotorTurn();
        }

    }
    return 0;   /* never reached */
}
