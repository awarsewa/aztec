//
//  credit_unit.h
//  aztec
//
//  Created by Alexander Warsewa on 17.11.17.
//

#ifndef credit_unit_h
#define credit_unit_h

#include "relay.h"

extern struct mb1 sw_creditUnitResetEOS;
extern struct mb4 sw_creditUnitZeroBk;
extern struct mb1 sw_creditUnitOpenAtLastPos;
extern struct mb1 sw_creditUnitOpenAtLastPos2;

extern relay0 relay_creditUnitReset;
extern relay1 relay_creditUnitStepUp;

#endif /* credit_unit_h */
