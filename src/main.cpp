#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Controller
Controller master(E_CONTROLLER_MASTER);

// Sensors
Imu imu(11);
ADIEncoder YEncoder(1, 2, false);
ADIEncoder XEncoder(3, 4, false);

// Objects
Odom odom;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


/*
 * Odometry Task
 * Tracks the position of the robot
*/

void odom_task(void* param) {
	delay(100);
	printf("Initializing Odometry...\n");
	odom.configure_starting(Vector2D(0,0), 0);
	odom.configure(10, 10, 20, 20, 20);
	printf("waiting for imu to initialize...\n");
	while (imu.is_calibrating() == true) {}
	printf("Initialization complete\n");

	while (true) {
		odom.calculate_position(ODOM_DEBUG_GLOBAL_POSITION);
	}
}

Task task_odom (odom_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

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
