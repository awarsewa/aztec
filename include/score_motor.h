//
//  score_motor.h
//  aztec
//
//  Created by Alexander Warsewa on 21.01.16.
//
//

#ifndef aztec_score_motor_h
#define aztec_score_motor_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "relay.h"

// Actual motor speed was a little too fast...
//#define MOTOR_STEP_TIME     347
#define MOTOR_STEP_TIME 	520

#define MOTOR_DISK_POS_CAM_INDEX        0x00
#define MOTOR_DISK_POS_CAM_INDEX_OFF    0x01
#define MOTOR_DISK_POS_CAM_1            0x02
#define MOTOR_DISK_POS_CAM_1_OFF        0x03
#define MOTOR_DISK_POS_CAM_2            0x04
#define MOTOR_DISK_POS_CAM_2_OFF        0x05
#define MOTOR_DISK_POS_CAM_3            0x06
#define MOTOR_DISK_POS_CAM_3_OFF        0x07
#define MOTOR_DISK_POS_CAM_4            0x08
#define MOTOR_DISK_POS_CAM_4_OFF        0x09
#define MOTOR_DISK_POS_CAM_5            0x0A
#define MOTOR_DISK_POS_CAM_5_OFF        0x0B

#define scoreMotorStart()       TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10)
#define scoreMotorStop()        TCCR1B = 0; TCNT1 = 0
 
struct motor {
    relay1 motor;
    relay4 CAM_index;
    relay5 CAM_1;
    relay4 CAM_2;
    relay4 CAM_3;
    relay5 CAM_4;
    relay2 CAM_5;
    relay3 impulse;
};

extern struct motor scoreMotor;
extern struct mb2 sw_scoreMotorCAM_IndexA;
extern struct mb2 sw_scoreMotorCAM_1B;
extern struct mb1 sw_scoreMotorCAM_2B;
extern struct mb4 sw_scoreMotorCAM_1C;
extern struct mb1 sw_scoreMotorCAM_3B;
extern struct mb1 sw_scoreMotorCAM_4B;
extern struct mb1 sw_scoreMotorImpulseA; 


extern trigger scoreMotorTrigger;
void scoreMotorTriggerFunc(uint8_t state);

ISR(TIMER1_COMPA_vect);
extern volatile uint8_t motorFlag;

extern void recursionBreaker(struct mb* sw, uint8_t state);

void scoreMotorTurn();


#endif
