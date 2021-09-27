#include "main.h"
#include "test_drive.h"

void power_drive(double x, double y, double turn) {
    driveFL =  y + turn;
    driveBL =  -y + turn;
    driveFR = -y + turn;
    driveBR = y + turn;
    driveSF = -x - (turn * STRAFE_TURN_CONST);
    driveSB = x - (turn * STRAFE_TURN_CONST);
}

Vector2D controller_target;
Vector2D resultant;
Vector2D unit_resultant;
double delta_theta;
double turn_power;

void relativeDriveControl() {
    // calculate the direction and magnitude the controller wants the robot to go in
    if (fabs(master.get_analog(ANALOG_LEFT_X)) > 5)
      controller_target.x = master.get_analog(ANALOG_LEFT_X);
    else 
      controller_target.x = 0;
    
    if (fabs(master.get_analog(ANALOG_LEFT_Y)) > 5)
      controller_target.y = master.get_analog(ANALOG_LEFT_Y);
    else
      controller_target.y = 0; 
    
    delta_theta = controller_target.getAngle() - (imu.get_heading() * 3.14159 / 180);

    resultant.x = controller_target.getLength() * sin(delta_theta);
    resultant.y = controller_target.getLength() * cos(delta_theta);

    std::cout << "\r" << "rx[" << std::setw(5) << std::setprecision(3) << resultant.x;
    std::cout << "] ry[" << std::setw(5) << std::setprecision(3) << resultant.y << "] dT[";
    std::cout << std::setw(10) << std::setprecision(3) << delta_theta << "] imuH[";
    std::cout << std::setw(10) << std::setprecision(3) << imu.get_heading() << "]" << std::flush;

    power_drive(resultant.x, resultant.y, master.get_analog(ANALOG_RIGHT_X));
}
