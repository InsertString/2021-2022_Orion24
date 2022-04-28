#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Controller master(E_CONTROLLER_MASTER);

Motor LeftConveyor(7);
Motor RightConveyor(8, true);

Motor DriveR1(1);
Motor DriveR2(2);
Motor DriveR3(9);
Motor DriveR4(10);
Motor DriveL1(11, true);
Motor DriveL2(12, true);
Motor DriveL3(18, true);
Motor DriveL4(20, true);

ADIDigitalOut Arm_Release(5);
ADIDigitalOut HighRings(6);
ADIDigitalOut Arm(7);
ADIDigitalOut Clamp(8);

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
	bool arm_state = false;
	bool clamp_state = false;
	double left = 0;
	double right = 0;

	while (true) {
		// Tank Drive Controls
		left = master.get_analog(ANALOG_LEFT_Y);
		right = master.get_analog(ANALOG_RIGHT_Y);

		DriveL1 = left;
		DriveL2 = left;
		DriveL3 = left;
		DriveL4 = left;
		DriveR1 = right;
		DriveR2 = right;
		DriveR3 = right;
		DriveR4 = right;

		// arm piston toggle
		if (master.get_digital_new_press(DIGITAL_L1)) {
			arm_state = !arm_state;
			Arm.set_value(arm_state);
		}

		// clamp piston toggle
		if (master.get_digital_new_press(DIGITAL_R1)) {
			clamp_state = !clamp_state;
			Clamp.set_value(clamp_state);
		}

		// conveyor controls
		if (master.get_digital(DIGITAL_UP)) {
			LeftConveyor = 127;
			RightConveyor = 127;
		}
		else if (master.get_digital(DIGITAL_DOWN)) {
			LeftConveyor = -127;
			RightConveyor = -127;
		}
		else if (master.get_digital(DIGITAL_LEFT)) {
			LeftConveyor = 127;
			RightConveyor = -127;
		}
		else if (master.get_digital(DIGITAL_RIGHT)) {
			LeftConveyor = -127;
			RightConveyor = 127;
		}
		else {
			LeftConveyor = 0;
			RightConveyor = 0;
		}

		delay(10);
	}
}
