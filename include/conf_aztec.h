/*
 *  conf_aztec.h
 *  aztec
 *
 *  Created by Alexander Warsewa on 09.01.16.
 *  Copyright 2016 SoapSoft All rights reserved.
 *
 */

#ifndef _CONF_AZTEC_H_
#define _CONF_AZTEC_H_

// Port for encoded inputs
#define AZTEC_INPUT_DDR         DDRA
#define AZTEC_INPUT_PORT	    PORTA
#define AZTEC_INPUT_PIN         PINA

#define AZTEC_AUX_INPUT_DDR     DDRC
#define AZTEC_AUX_INPUT_PORT    PORTC
#define AZTEC_AUX_INPUT_PIN     PINC

#define AZTEC_INPUT_Q10		PINA0
#define AZTEC_INPUT_Q11		PINA1
#define AZTEC_INPUT_Q12		PINA2
#define AZTEC_INPUT_Q13		PINA3

#define AZTEC_INPUT_Q20		PINA4
#define AZTEC_INPUT_Q21		PINA5
#define AZTEC_INPUT_Q22		PINA6
#define AZTEC_INPUT_Q23		PINA7

#define ATZEC_INPUT_Q30     PINC0
#define AZTEC_INPUT_Q31     PINC1
#define AZTEC_UNPUT_Q32     PINC2

// SPI / output PORT
#define AZTEC_OUTPUT_DDR    DDRB
#define AZTEC_OUTPUT_PORT	PORTB

#define AZTEC_SPI_MOSI		PINB5
#define AZTEC_SPI_MISO		PINB6
#define AZTEC_SPI_SCK		PINB7
#define AZTEC_SPI_RCK1		PINB0
#define AZTEC_SPI_RCK2		PINB1
#define AZTEC_SPI_RCK3		PINB2
#define AZTEC_SPI_RCK4		PINB3
#define AZTEC_SPI_RCK5		PINB4

// Control port and pin assignments
#define AZTEC_CONTROL_DDR       DDRC
#define AZTEC_CONTROL_PORT		PORTC

#define AZTEC_OUTPUT_ENABLE		PINC7
#define AZTEC_CLOCK_ENABLE		PINC4
#define AZTEC_PARALLEL_LOAD		PINC5
#define AZTEC_SERIAL_CLEAR		PINC6


// Event port and pin assignments
#define AZTEC_EVENT_DDR         DDRD
#define AZTEC_EVENT_PORT        PORTD
#define AZTEC_EVENT_PIN			PIND

#define AZTEC_EVENT_AC_ZERO     PIND2

// Event source mapping
#define AZTEC_EVENT_SRC_GS1     0x01
#define AZTEC_EVENT_SRC_GS2     0x02
#define AZTEC_EVENT_SRC_GS3     0x04
#define AZTEC_EVENT_SRC_BUT1    0x10
#define AZTEC_EVENT_SRC_BUT2    0x20
#define AZTEC_EVENT_SRC_BUT3    0x40

#define AZTEC_EVENT_MASK        0x77


/*
 *  INPUT EVENTS 
 */


#define NO_EVENT                        0xFF

// Input events when GS1 is toggled
#define L_LANE_ROLL_OVER				0x08
#define L_TOP_TARGET					0x09
#define L_TOP_ROLL_OVER					0x0A
#define L_R_TOP_ROLL_OVER_BUTTON		0x0B
#define CENTER_TOP_ROLL_OVER			0x0C
#define CENTER_TOP_ROLL_OVER_BUTTON		0x0D
#define R_TOP_ROLL_OVER					0x0E
#define R_JET_BUMPER					0x0F

#define L_BOT_ROLL_OVER_IN				0x00
#define L_BOT_ROLL_OVER_OUT				0x01
#define E_TARGET						0x02
#define Z_TARGET						0x03
#define SPINNER							0x04
#define	L_JET_BUMPER					0x05
#define CENTER_TARGET					0x06
#define CENTER_JET_BUMPER				0x07

// Input events when GS2 is toggeled
#define OUTHOLE_SWITCH					0x80
#define TILT_SWITCH_FIELD				0x90
#define R_BOT_ROLL_OVER_IN				0xA0
#define R_BOT_ROLL_OVER_OUT				0xB0
#define C_TARGET						0xC0
#define T_TARGET						0xD0
#define SHOOTER_SWITCH					0xE0
#define CREDIT_BUTTON                   0xF0

#define NUMBER_MATCH_S_U      			0x10
// Note: free slot available            0x01
#define HIGHSCORE_EXTRABALL             0x20
#define TILT_SWITCH_CASE				0x30
// NOTE: free slot available
#define SWITCH_5_CT						0x50
#define SWITCH_10_CT					0x60
#define SWITCH_25_CT					0x70

// Auxiliary inputs (Nr. 5) for states, not events
// Highest number has priority... this sucks..
#define CREDIT_UNIT_ZERO_BK             0x00
#define DRUM_UNIT_ZERO_BK_SWS           0x01
#define CREDIT_UNIT_RESET_EOS           0x02

#define AZTEC_AUX_INPUT_MASK            0x03

/*
 *  OUTPUT MAPPING
 */

// 1st shift register
#define NO_1_2_3_RESET                  0
#define RELAY_100_PTS                   1
#define RELAY_1000_PTS                  2
#define RELAY_10000_PTS                 3
#define PLAYER_STEP_UP                  4
#define PLAYER_RESET                    5
#define CREDIT_STEP_UP                  6
#define CREDIT_RESET                    7

// 2nd shift register
#define BALL_IN_PLAY_5                  0
#define BALL_IN_PLAY_4                  1
#define BALL_IN_PLAY_3                  2
#define BALL_IN_PLAY_1_2                3
#define CAN_PLAY_4                      4
#define CAN_PLAY_3                      5
#define CAN_PLAY_1_2                    6
#define GAME_OVER                       7

// 3rd shift register
#define SETUP_RELAY                     0
#define CHANGE_RELAY                    1
#define EXTRA_BALL_RELAY                2
#define A_RELAY                         3
#define Z_RELAY                         4
#define T_RELAY                         5
#define E_RELAY                         6 
#define C_RELAY                         7

// 4th shift register
#define BONUS_GND                       0
#define BONUS_1                         1
#define BONUS_2                         2
#define BONUS_3                         3
#define BONUS_4                         4 
#define BONUS_5                         5
#define DOUBLE_BONUS                    6
#define BALL_RELEASE                    7

// 5th shift register
#define NUMBER_MATCH_GND                0
#define NUMBER_MATCH_1                  1
#define NUMBER_MATCH_2                  2
#define NUMBER_MATCH_3                  3
#define NUMBER_MATCH_4                  4
#define NUMBER_MATCH_5                  5
#define KNOCKER                         6
#define SHOOTER_COIL                    7

#define AZTEC_STATE_IDLE                0x00
#define AZTEC_MOTOR_FLAG                1


#define stateRegisterSet(flag)          aztecStateRegister |= (1 << (flag))
#define stateRegisterClear(flag)        aztecStateRegister &= ~(1 << (flag))

#endif
