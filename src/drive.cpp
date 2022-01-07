#include "main.h"
#include "drive.h"

void power_drive(double x, double y, double turn) {
    driveLFL = -x - y - turn;
    driveLFR = -x - y - turn;
    driveLBL = x - y - turn;
    driveLBR = x - y - turn;
    driveRFL = -x + y - turn;
    driveRFR = -x + y - turn;
    driveRBL = x + y - turn;
    driveRBR = x + y - turn;
}

Vector2D controller_target;
Vector2D resultant;
Vector2D unit_resultant;
double delta_theta;
double turn_power;

void relativeDriveControl(double target_theta, bool hold_theta, bool debug) {
    // calculate the direction and magnitude the controller wants the robot to go in
    if (fabs(master.get_analog(ANALOG_LEFT_X)) > 5)
      controller_target.x = master.get_analog(ANALOG_LEFT_X);
    else
      controller_target.x = 0;

    if (fabs(master.get_analog(ANALOG_LEFT_Y)) > 5)
      controller_target.y = master.get_analog(ANALOG_LEFT_Y);
    else
      controller_target.y = 0;

    delta_theta = controller_target.getAngle() - (target_theta * 3.14159 / 180);

    resultant.x = controller_target.getLength() * sin(delta_theta);
    resultant.y = controller_target.getLength() * cos(delta_theta);

    if (debug == true) {
      std::cout << "\r" << "rx[" << std::setw(5) << std::setprecision(3) << resultant.x;
      std::cout << "] ry[" << std::setw(5) << std::setprecision(3) << resultant.y << "] dT[";
      std::cout << std::setw(10) << std::setprecision(3) << delta_theta << "] imuH[";
      std::cout << std::setw(10) << std::setprecision(3) << target_theta << "]" << std::flush;
    }

    if (hold_theta == true) {
      turn_power = (target_theta - imu.get_heading()) * 3;
    }
    else {
      turn_power = master.get_analog(ANALOG_RIGHT_X);
    }

    power_drive(resultant.x, resultant.y, turn_power);
}
