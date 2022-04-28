#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Sensor
Imu imu(5);
ADIEncoder YEncoder(3, 4, false);
ADIEncoder XEncoder(1, 2, false);

// Odom
Odom odom(&imu, &XEncoder, &YEncoder);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


/*
 * Odometry Task
 * Tracks the position of the robot
*/

void odom_task(void* param) {
	//*
	delay(50);
	printf("Initializing Odometry...\n");
	//* comp:
	odom.configure_starting(Vector2D(0,0), 0);
	odom.configure(8.25, 20, 3.22, 20, 20);
	//*/
	/* skills: 
	odom.configure_starting(Vector2D(0,0), 0);
	odom.configure(8.25, 20, 3.22, 20, 20);
	//*/
	printf("waiting for imu to initialize...\n");
	while (imu.is_calibrating() == true) {}
	printf("Initialization complete\n");

	while (true) {
		odom.calculate_position(ODOM_DEBUG_ENCODER_RAW);
	}
	//*/
}

Task task_odom (odom_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

void initialize() {
	imu.reset();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	
}


void opcontrol() {

	while (true) {
		
	}
}
