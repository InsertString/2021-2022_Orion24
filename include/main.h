/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"
#include "math.h"
#include "iomanip"
#include "PID.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"
#include "drive.h"
#include "odometry.hpp"
#include "auto_functions.h"
#include "dca.hpp"
#include "OBarInverseKin.hpp"
#include "auto.hpp"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#define CONTROLLER_JOYSTICK_THRESHOLD 5

// extern controllers
extern Controller master;

// extern drive mtoors
extern Motor driveLFL;
extern Motor driveLFR;
extern Motor driveLBL;
extern Motor driveLBR;
extern Motor driveRFR;
extern Motor driveRFL;
extern Motor driveRBL;
extern Motor driveRBR;
extern Motor ArmLeft;
extern Motor ArmRight;
extern Motor Elevator;
extern Motor MogoLeft;
extern Motor MogoRight;
extern Motor OArm;
extern Motor OWrist;

// extern for sensors
extern Imu imu;
extern ADIEncoder RightEncoder;
extern ADIEncoder BackEncoder;
extern ADIDigitalIn ArmLimit;
extern ADIDigitalIn MogoLimit;
extern ADIPotentiometer OBarPot;

// extern for pnuematics
extern ADIDigitalOut Claw;
extern ADIDigitalOut MogoShifter;
extern ADIDigitalOut Needle;


#define ARM_MIN 0
#define ARM_HOVER 1
#define ARM_STACK 2
#define ARM_MAX 3
#define ARM_MIN_POS 0
#define ARM_HOVER_POS 200
#define ARM_STACK_POS 1100
#define ARM_DOCK_POS 1300
#define ARM_MAX_POS 1700


#define ELEVATOR_STOP 0
#define ELEVATOR_INTAKE 1
#define ELEVATOR_OUTAKE 2

#define MOGO_MAX_POS 1270
#define MOGO_MID_POS 800
#define MOGO_MIN_POS 0

#define OBAR_STATE_IDLE 0
#define OBAR_STATE_LOADING 1
#define OBAR_STATE_FRONT_UNLOADING 2
#define OBAR_STATE_BACK_UNLOADING 3

#endif  // _PROS_MAIN_H_
