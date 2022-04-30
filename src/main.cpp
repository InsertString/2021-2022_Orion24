#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Controller master(E_CONTROLLER_MASTER);

Motor LeftConveyor(7);
Motor RightConveyor(8, true);

Motor DriveR1(1,  false);
Motor DriveR2(2,  false);
Motor DriveR3(9,  false);
Motor DriveR4(10, false);
Motor DriveL1(11,  true);
Motor DriveL2(12,  true);
Motor DriveL3(18,  true);
Motor DriveL4(20,  true);

ADIDigitalOut ArmRelease(6);
ADIDigitalOut HighRings(5);
ADIDigitalOut Arm(7);
ADIDigitalOut Clamp(8);

Imu imu(6);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void initialize() {
	imu.reset();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	comp_auto();

}


void opcontrol() {
	bool arm_state = false;
	bool clamp_state = false;
	bool arm_release_state = false;
	bool high_rings_state = false;
	double left = 0;
	double right = 0;
	tare_drive_motors();

	while (true) {
		screen::print(TEXT_SMALL, 0, "Pos: %f", drive_position());
		// Tank Drive Controls
		if (master.get_digital(DIGITAL_R2)) {
			left = -30;
			right = -30;
		}
		else {
			left = master.get_analog(ANALOG_LEFT_Y);
			right = master.get_analog(ANALOG_RIGHT_Y);
		}

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

		// high rings release piston toggle
		if (master.get_digital_new_press(DIGITAL_A)) {
			high_rings_state = !high_rings_state;
			HighRings.set_value(high_rings_state);
		}

		// stage 2 release piston toggle
		if (master.get_digital_new_press(DIGITAL_B)) {
			arm_release_state = !arm_release_state;
			ArmRelease.set_value(arm_release_state);
		}

		// conveyor controls
		// when rotating, forwards motion should be less as it seems to have more influence
		// on the motion of the mogo
		if (master.get_digital(DIGITAL_UP)) {
			LeftConveyor =  60;
			RightConveyor = 60;
		}
		else if (master.get_digital(DIGITAL_DOWN)) {
			LeftConveyor =  -127;
			RightConveyor = -127;
		}
		else if (master.get_digital(DIGITAL_LEFT)) {
			LeftConveyor =   60;
			RightConveyor = -90;
		}
		else if (master.get_digital(DIGITAL_RIGHT)) {
			LeftConveyor = -90;
			RightConveyor = 60;
		}
		else {
			LeftConveyor = 0;
			RightConveyor = 0;
		}

		delay(10);
	}
}
