//
//  AZTEC_relays.h
//  aztec
//
//  Created by Alexander Warsewa on 31.01.16.
//
//

#ifndef aztec_AZTEC_relays_h
#define aztec_AZTEC_relays_h

#include "relay.h"

#define NUM_RED_RELAYS    21

extern relay0 relay_dummy;
extern relay0 shooterCoil;

extern struct mb1 sw_relayDoubleBonusC;
extern struct mb1 sw_relayDoubleBonusC2;
extern struct mb2 sw_relayDoubleBonusD;
extern relay4 relay_doubleBonus;

extern struct mb1 sw_relayShooterA;
extern struct mb1 sw_relayShooterB;
extern relay relay_shooter;

extern relay2 relay_coinSetUp;

extern struct mb1 sw_relayBallIndexA;
extern struct mb1 sw_relayBallIndexC;
extern struct mb1 sw_relayBallIndexD;
extern struct mb1 sw_relayBallIndexE;
extern struct mb2 sw_relayBallIndexF;
extern relay relay_ballIndex;

extern struct mb1 sw_relayBonus2A;
extern struct mb1 sw_relayBonus2B;
extern relay3 relay_bonus2;

extern struct mb1 sw_relayBonus1B;
extern relay5 relay_bonus;

extern relay0 relay_ballRelease;

extern struct mb1 sw_centerJetBumper;
extern struct mb1 sw_rightJetBumper;
extern struct mb1 sw_leftJetBumper;

extern struct mb2 sw_rightTopRollOver;
extern struct mb2 sw_leftTopRollOver;
extern struct mb2 sw_leftTopTarget;
extern struct mb1 sw_rightBottomRollOverIn;
extern struct mb1 sw_leftBottomRollOverIn;
extern struct mb3 sw_rightBottomRollOverOut;
extern struct mb3 sw_leftBottomRollOverOut;
extern struct mb1 sw_leftRightTopRollOverButton;
extern struct mb1 sw_centerTopRollOverButton;
extern struct mb1 sw_leftLaneRollOver;
extern struct mb1 sw_leftLaneRollOver2;
extern struct mb1 sw_spinner;

extern struct mb1 sw_shooter;
extern struct mb1 sw_relayOutholeA; 
extern struct mb1 sw_outhole;
extern struct mb2 sw_bonusUnitZeroPos;

extern struct mb1 sw_creditButton;

extern struct mb1 sw_noMatchAlt;

extern struct mb1 sw_5ct;
extern struct mb1 sw_10ct;
extern struct mb2 sw_25ct;

extern struct mb1 sw_drumUnitZeroBk;

extern struct mb2 sw_targetC;
extern struct mb2 sw_relayCC;
extern relay4 relay_C;

extern struct mb2 sw_targetE;
extern struct mb2 sw_relayEC;
extern relay4 relay_E;

extern struct mb2 sw_targetT;
extern struct mb2 sw_relayTC;
extern relay4 relay_T;

extern struct mb2 sw_targetZ;
extern struct mb3 sw_relayZB;
extern struct mb2 sw_relayZC;
extern struct mb2 sw_relayZF;
extern relay relay_Z;

extern struct mb2 sw_centerTopRollOver;
extern struct mb1 sw_relayAB;
extern struct mb2 sw_relayAC;
extern struct mb1 sw_relayAD;
extern struct mb2 sw_relayAF;
extern relay5 relay_A;

extern relay4 relay_outhole;

extern struct mb3 sw_relayResetA;
extern struct mb1 sw_relayResetB;
extern struct mb5 sw_relayResetF;
extern relay relay_reset;
extern struct mb1 sw_relayGameOverB;
extern relay5 relay_gameOver;
extern relay1 relay_gameOverTrip;
extern relay1 relay_gameOverLatch;

extern struct mb1 sw_relayCoinA;
extern struct mb1 sw_relayCoinE;
extern relay5 relay_coin;

extern struct mb1 sw_relayPlayerResetA;
extern struct mb2 sw_relayPlayerResetC;
extern relay3 relay_playerReset;

extern struct mb1 sw_centerTarget;
extern struct mb1 sw_relayCenterTarget2A;
extern struct mb1 sw_relayCenterTarget2B;
extern struct mb1 sw_relayCenterTargetA;
extern struct mb1 sw_relayCenterTargetB;
extern relay5 relay_centerTarget;

extern struct mb1 sw_relayChangeA;
extern struct mb1 sw_relayChangeA2;
extern struct mb2 sw_relayChangeB;
extern struct mb2 sw_relayChangeC;
extern struct mb2 sw_relayChangeD;
extern struct mb2 sw_relayChangeE;
extern relay relay_change;

extern struct mb1 sw_relaySetUpA;
extern struct mb1 sw_relaySetUpB;
extern struct mb1 sw_relaySetUpD;
extern struct mb1 sw_relaySetUpD2;
extern relay4 relay_setUp;

extern struct mb1 sw_relayAdvanceA;
extern relay2 relay_advance;

extern struct mb1 sw_relay5000A;
extern struct mb1 sw_relay5000B;
extern relay3 relay_5000;

extern relay0 relay_10000Pt;

extern struct mb1 sw_relay1000PtB;
extern relay2 relay_1000Pt;

extern struct mb1 sw_relay100PtB;
extern struct mb1 sw_relay100Pt2B;
extern relay2 relay_100Pt;

extern struct mb1 sw_relay10PtB;
extern struct mb1 sw_relay10Pt2C;
extern relay2 relay_10Pt;

extern relay0 relay_No123Reset;

extern struct mb2 sw_relayExtraBallA;
extern struct mb1 sw_relayExtraBallC;
extern struct mb1 sw_relayExtraBallD;
extern relay3 relay_extraBall;

extern relay0 relay_knocker;

extern relay4 relay_10ct;

extern struct mb1 sw_relay25ct1D;
extern relay5 relay_25ct;

extern relay* red_relays[NUM_RED_RELAYS];

#endif
