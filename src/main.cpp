#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*
 * Odometry Task
 * Tracks the position of the robot
*/
Task odom (odom_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

void initialize() {
	
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
