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

extern Odom odom;

extern Motor LeftConveyor(7);
extern Motor RightConveyor(8, true);
extern Motor DriveR1(1);
extern Motor DriveR2(2);
extern Motor DriveR3(9);
extern Motor DriveR4(10);
extern Motor DriveL1(11, true);
extern Motor DriveL2(12, true);
extern Motor DriveL3(18, true);
extern Motor DriveL4(20, true);

extern ADIDigitalOut Arm_Release(5);
extern ADIDigitalOut HighRings(6);
extern ADIDigitalOut Arm(7);
extern ADIDigitalOut Clamp(8);

#endif  // _PROS_MAIN_H_
