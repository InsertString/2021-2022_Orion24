/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * Copyright (c) 2017-2022, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#include "api.h"
#include "ORION/ORION_includes.hpp"
#include "auto.hpp"
#include "auto_functions.hpp"
#include "base_functions.hpp"

using namespace pros;
using namespace std;

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

extern Motor LeftConveyor;
extern Motor RightConveyor;
extern Motor DriveR1;
extern Motor DriveR2;
extern Motor DriveR3;
extern Motor DriveR4;
extern Motor DriveL1;
extern Motor DriveL2;
extern Motor DriveL3;
extern Motor DriveL4;

extern ADIDigitalOut ArmRelease;
extern ADIDigitalOut HighRings;
extern ADIDigitalOut Arm;
extern ADIDigitalOut Clamp;

extern Imu imu;

#endif  // _PROS_MAIN_H_
