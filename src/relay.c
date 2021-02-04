//
//  relay.c
//  aztec
//
//  Created by Alexander Warsewa on 21.01.16.
//
//

#include "relay.h"

uint8_t gameOverFlag = 0;
uint8_t tiltFlag = 0;
uint8_t resetFlag = 0;

shiftReg outRegs[NUM_OUTPUT_REGS];

inline void relay_init(void)
{
    uint8_t i;
    
    for(i = 0; i < NUM_OUTPUT_REGS; i++)
        outRegs[i].data = 0x00;
}

void switchOn(struct mb* sw)
 {
    relay *rel = NULL;
    trigger *trig = NULL;
    uint8_t i = 0;
    
    if(sw == NULL)
        return;
    
    switch(sw_getType(sw)){
        case RELAY_TYPE_REAL_COIL:
        case RELAY_TYPE_SIMULATED:
            rel = toRelayPtr(sw);
            // Nothing happens if relay is already on
            if(rel_getState(rel) == RELAY_STATE_ON)
                break;
            else {
                // Nothing happens if relay is shut down by tilt or game over re.
                if(tiltFlag && rel_isOffOnTilt(rel))
                    break;
                if(gameOverFlag && rel_isOffOnGameOver(rel))
                    break;
                if(resetFlag && rel_isOffOnGameOver(rel))
                    break;
                
                // Set relay state to on
                rel_setState(rel, RELAY_STATE_ON);
            
                // Power relay if it has a real counterpart
                if(rel_isRealCoil(rel)){
                    outRegs[rel_getOutReg(rel)].data |= (1 << rel_getOutPin(rel));
                    shiftReg_write(outRegs[rel_getOutReg(rel)]);
                    shiftReg_store(outRegs[rel_getOutReg(rel)]);
                }
                // Switch on all the relay's switches
                for(i = 0; i < rel_nSwitches(rel); i++)
                    switchOn(rel->switches[i]);
            }
            break;
        case SWITCH_TYPE_MAKE:
            // nothing happens if switch was already on
            if(sw_getState(sw) == SWITCH_STATE_OPEN){

                sw_setState(sw, SWITCH_STATE_CLOSED);
                //if switch sources power, trace its connection to other switches
                if(sw_isPowered(sw)){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
            
        case SWITCH_TYPE_BREAK:
            if(sw_getState(sw) == SWITCH_STATE_CLOSED){
                sw_setState(sw, SWITCH_STATE_OPEN);
                if(sw_isPowered(sw)){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOff(sw->connected[i]);
                }
            }
            break;
            
        case SWITCH_TYPE_MAKE_BREAK:
            if(sw_getState(sw) == SWITCH_STATE_DEFAULT){
                sw_setState(sw, SWITCH_STATE_ALTERNATE);
                if(sw_isPowered(sw)){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOff(sw->connected[i]);
                    for(i = sw_getNConnected(sw); i < (sw_getNConnected(sw)+sw_getNAlternate(sw)); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
        
        // Trigger switches call a function (i.e. to start/stop the score motor)
        case SWITCH_TYPE_TRIGGER:
            trig = toTriggerPtr(sw);
            if(sw_getState(sw) == TRIGGER_STATE_OFF){
                sw_setState(sw, TRIGGER_STATE_ON);
                trig->triggerFunc(TRIGGER_STATE_ON);
            }
            break;
        
        default:
            break;
    }
}


void switchOff(struct mb* sw)
{
    relay  *rel = NULL;
    trigger* trig = NULL;
    uint8_t i = 0;
    
    if(sw == NULL)
        return;
    
    switch(sw_getType(sw)){
        case RELAY_TYPE_REAL_COIL:
        case RELAY_TYPE_SIMULATED: 
            rel = toRelayPtr(sw);
            if(rel_getState(rel) == RELAY_STATE_OFF)
                break;
            
            if((gameOverFlag && rel_isOffOnGameOver(rel)) ||
               (tiltFlag && rel_isOffOnTilt(rel)) ||
               (resetFlag && rel_isOffOnGameOver(rel)) ||
               (!rel_getHoldCount(rel)))
            {
                rel_setState(rel, RELAY_STATE_OFF);
                if(rel_isRealCoil(rel)){
                    outRegs[rel_getOutReg(rel)].data &= ~(1 << (rel_getOutPin(rel)));
                    shiftReg_write(outRegs[rel_getOutReg(rel)]);
                    shiftReg_store(outRegs[rel_getOutReg(rel)]);
                    //queue_push(&turnoffQ, &(rel->info2));
                }
                for(i = 0; i < rel_nSwitches(rel); i++)
                    switchOff(rel->switches[i]);
            }
            break;
            
        case SWITCH_TYPE_MAKE:
            // nothing happens if switch was already off
            if(sw_getState(sw) == SWITCH_STATE_CLOSED){
                sw_setState(sw, SWITCH_STATE_OPEN);
                //if switch sources power, trace its connection to other switches
                if(sw_isPowered(sw)){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOff(sw->connected[i]);
                }
            }
            break;
            
        case SWITCH_TYPE_BREAK:
            if(sw_getState(sw) == SWITCH_STATE_OPEN){
                sw_setState(sw, SWITCH_STATE_CLOSED);
                if(sw_isPowered(sw)){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
            
        case SWITCH_TYPE_MAKE_BREAK:
            if(sw_getState(sw) == SWITCH_STATE_ALTERNATE){
                sw_setState(sw, SWITCH_STATE_DEFAULT);
                if(sw_isPowered(sw)){
                    for(i = sw_getNConnected(sw); i < (sw_getNConnected(sw)+sw_getNAlternate(sw)); i++)
                        switchTraceOff(sw->connected[i]);
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
            
        case SWITCH_TYPE_TRIGGER:
            trig = toTriggerPtr(sw);
            if(sw_getState(sw) == TRIGGER_STATE_ON){
                sw_setState(sw, TRIGGER_STATE_OFF);
                trig->triggerFunc(TRIGGER_STATE_OFF);
            }
            break;
            
        default:
            break;
    }
}


void switchTraceOn(struct mb* sw)
{
    relay* rel = NULL;
    uint8_t i = 0;
    
    if(sw == NULL)
        return;
    
    switch(sw_getType(sw)){
        case RELAY_TYPE_REAL_COIL:
        case RELAY_TYPE_SIMULATED:
            rel = toRelayPtr(sw);
            rel->holdCount++;
            switchOn(sw);
            break;
        case SWITCH_TYPE_TRIGGER:
            switchOn(sw);
            break;
        case SWITCH_TYPE_MAKE:
        case SWITCH_TYPE_BREAK:
            if(sw->holdCount){
                sw->holdCount++;
            } else {
                sw->holdCount++;
                sw_powerOn(sw);
                if(sw_getState(sw) == SWITCH_STATE_CLOSED){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
        case SWITCH_TYPE_MAKE_BREAK:
            if(sw->holdCount){
                sw->holdCount++;
            } else {
                sw->holdCount++;
                sw_powerOn(sw);
                if(sw_getState(sw) == SWITCH_STATE_DEFAULT){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOn(sw->connected[i]);
                } else {
                    for(i = sw_getNConnected(sw); i < (sw_getNConnected(sw) + sw_getNAlternate(sw)); i++)
                        switchTraceOn(sw->connected[i]);
                }
            }
            break;
        default:
            break;
    }
}


void switchTraceOff(struct mb* sw)
{
    relay* rel = NULL;
    uint8_t i = 0;
    
    if(sw == NULL)
        return;
    
    switch(sw_getType(sw)){
        case RELAY_TYPE_REAL_COIL:
        case RELAY_TYPE_SIMULATED:
            rel = toRelayPtr(sw);
            if(rel_getHoldCount(rel))
                rel->holdCount--;
            switchOff(sw);
            break;
        case SWITCH_TYPE_TRIGGER:
            switchOff(sw);
            break;
        case SWITCH_TYPE_MAKE:
        case SWITCH_TYPE_BREAK:
            if(sw->holdCount)
                sw->holdCount--;
            
            if(!sw->holdCount){
                sw_powerOff(sw);

                if(sw_getState(sw) == SWITCH_STATE_CLOSED){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOff(sw->connected[i]);
                }
            }
            break;
        case SWITCH_TYPE_MAKE_BREAK:
            if(sw->holdCount)
                sw->holdCount--;
            
            if(!sw->holdCount){
                sw_powerOff(sw);
                if(sw_getState(sw) == SWITCH_STATE_DEFAULT){
                    for(i = 0; i < sw_getNConnected(sw); i++)
                        switchTraceOff(sw->connected[i]);
                } else {
                    for(i = sw_getNConnected(sw); i < (sw_getNConnected(sw) + sw_getNAlternate(sw)); i++)
                        switchTraceOff(sw->connected[i]);
                }
            }
            break;
        default:
            break;
    }
}