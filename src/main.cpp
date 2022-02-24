#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Controller
Controller master(E_CONTROLLER_MASTER);

// Sensors
Imu imu(1);
ADIEncoder YEncoder(1, 2, false);
ADIEncoder XEncoder(3, 4, false);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*
 * Odometry Task
 * Tracks the position of the robot
*/
//Task odom (odom_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

void initialize() {
	imu.reset();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	while (true) {
		if (master.get_digital(DIGITAL_DOWN)) break;
	}
}


void opcontrol() {

	while (true) {
		delay(10);
	}
}
