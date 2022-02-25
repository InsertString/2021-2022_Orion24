#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Controller
Controller master(E_CONTROLLER_MASTER);

// Motors
Motor MogoLeft(10, true);
Motor MogoRight(9, false);
Motor Intake(8);

// Sensors
Imu imu(11);
ADIEncoder YEncoder(1, 2, false);
ADIEncoder XEncoder(3, 4, false);
ADIDigitalIn MogoEndstop(5);

// Objects
Odom odom;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


/*
 * Odometry Task
 * Tracks the position of the robot
*/

void odom_task(void* param) {
	delay(50);
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
	MogoLeft.tare_position();
	MogoRight.tare_position();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	while (true) {
		if (master.get_digital(DIGITAL_DOWN)) break;
	}
}


void opcontrol() {
	bool mogo_up = false;

	while (true) {
		debug_base_systems();

		if (MogoEndstop.get_value() == MOGO_ENDSTOP_TRIGGERED) {
			MogoLeft.tare_position();
			MogoRight.tare_position();
		}

		if (mogo_up) {
			if (MogoEndstop.get_value() == MOGO_ENDSTOP_TRIGGERED)
				power_mogo(60);
			else 
				move_mogo_to_position(MOGO_LIFT_POSITION, 100);
		}
		else {
			if (MogoEndstop.get_value() == MOGO_ENDSTOP_TRIGGERED)
				power_mogo(0);
			else 
				power_mogo(-30);
		}

		if (master.get_digital_new_press(DIGITAL_A)) {
			mogo_up = !mogo_up;
		}

		if (master.get_digital(DIGITAL_L1)) {
			power_intake(127);
		}
		else if (master.get_digital(DIGITAL_L2)) {
			power_intake(-127);
		}
		else {
			power_intake(0);
		}

		delay(10);
	}
}
