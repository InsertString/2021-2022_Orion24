#ifndef _DRIVE_H_
#define _DRIVE_H_

// powers the drive motors based on x, y, and turn data
void power_drive(double x, double y, double turn);

// controls the drive relative to the origin based on orientation
void relativeDriveControl(double target_theta, bool hold_theta);

#endif
