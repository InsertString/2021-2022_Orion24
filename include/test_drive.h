#ifndef _TEST_DRIVE_H_
#define _TEST_DRIVE_H_

// the strafe wheels turn faster then the outer drive ones so they need to be slowed down by this factor
#define STRAFE_TURN_CONST 0.3

// powers the drive motors based on x, y, and turn data
void power_drive(double x, double y, double turn);

// controls the drive relative to the origin based on orientation
void relativeDriveControl();

#endif