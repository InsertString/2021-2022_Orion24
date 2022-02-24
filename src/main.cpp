#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// controller
Controller master(E_CONTROLLER_MASTER);
Controller partner(E_CONTROLLER_PARTNER);

// drive
Motor driveLFL(19);
Motor driveLFR(20);
Motor driveLBL(9);
Motor driveLBR(10);
Motor driveRFL(11);
Motor driveRFR(12);
Motor driveRBL(1);
Motor driveRBR(2);

// arm
Motor ArmLeft(15, true);
Motor ArmRight(14);

// ring elevator
Motor Elevator(17, true);

// mogo intake
Motor MogoLeft(18);
Motor MogoRight(13, true);

// onebar
Motor OArm(6);
Motor OWrist(7);

// sensors
Imu imu(3);

ADIEncoder RightEncoder(5, 6, true);
ADIEncoder BackEncoder(3, 4, false);
ADIDigitalIn ArmLimit(1);
ADIDigitalIn MogoLimit(8);
ADIPotentiometer OBarPot({{5, 8}});

// pneumatics
ADIDigitalOut Claw(7);
ADIDigitalOut MogoShifter(2);
ADIDigitalOut Needle({{5, 7}});

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

/*
 * Odometry Task
 * Tracks the position of the robot
*/
Task odom (odom_task, NULL, TASK_PRIORITY_DEFAULT - 1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

/*
 * Dynamic Current Allocation Task
 * Dynamically allocates current to the motors based on which systems are being used
*/

Task dca (dynamic_current_task, NULL, TASK_PRIORITY_DEFAULT - 2, TASK_STACK_DEPTH_DEFAULT, "DCAT");


void initialize() {
	imu.reset();
	OArm.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	OWrist.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	OWrist.tare_position();
	Claw.set_value(false);
	MogoShifter.set_value(false);
	Needle.set_value(false);
	MotorPriority[0] = 8;
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	while (true) {
		if (master.get_digital(DIGITAL_DOWN)) break;
	}
}


void opcontrol() {

	bool claw_state = false;
	bool mogo_shifter_state = false;
	bool needle_state = false;
	int mogo_state = 0;
	int OBar_state = 0;
	int elevator_state = 0;
	int arm_state = 0;

	while (true) {
		/*
		power_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));


		if (master.get_digital_new_press(DIGITAL_UP)) autonomous();
		//*/
		///*

		// Reset Motor Encoder Positions //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		if (MogoLimit.get_value() == 1) {
			MogoRight.tare_position();
			MogoLeft.tare_position();
		}

		if (ArmLimit.get_value() == 1) {
			ArmRight.tare_position();
			ArmLeft.tare_position();
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


		// Drivetrain Code //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

		// normal arcade drive
		power_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		// check if the driver is trying to move the robot and set the motor prio
		if (fabs(master.get_analog(ANALOG_LEFT_X)) > 5 || fabs(master.get_analog(ANALOG_LEFT_Y)) > 5 || fabs(master.get_analog(ANALOG_RIGHT_X)) > 5) {
			MotorPriority[DRIVE] = 8;
		}
		else {
			MotorPriority[DRIVE] = 2;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


		// Elevator //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// Elevator Master
		if (master.get_digital(DIGITAL_L1)) {
			Elevator = 127;
			MotorPriority[INTAKE] = 2;
		}
		else if (master.get_digital(DIGITAL_L2)) {
			Elevator = -127;
			MotorPriority[INTAKE] = 2;
		}
		else {
			Elevator = 0;
			MotorPriority[INTAKE] = 0;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


		// Mogo Intake and OBar//
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

		MogoShifter.set_value(mogo_shifter_state);
		Needle.set_value(needle_state);

		// mogo pneumatics toggle
		if (partner.get_digital_new_press(DIGITAL_L1)) {
			OBar_state = 0;
			if (mogo_state == 0) {
				mogo_state = 1;
			} 
			else if (OBarAngle() > -60) {
				mogo_state = 0;
			}
			else {
				mogo_state = 5;
			}
		}

		// button for OBar loading
		// only actuate if the arm is at the minimum position
		if (partner.get_digital_new_press(DIGITAL_L2)) {
			if (arm_state == ARM_MIN) {
				mogo_state = 2;
			}
			else {
				arm_state = ARM_MIN;
			}
		}

		// manually trigger the needle intake
		if (partner.get_digital_new_press(DIGITAL_B)) {
			needle_state = !needle_state;
		}

		// toggle between the two unloading state
		if (partner.get_digital_new_press(DIGITAL_UP)) {
			OBar_state = 2;
			arm_state = ARM_DOCK_POS;
			claw_state = true;
		}
		
		if (partner.get_digital_new_press(DIGITAL_DOWN)) 
			OBar_state = 3;

		// state machine for the OBar system
		switch (OBar_state) {
			case 0 :
			OBarMoveToPosition(0);
			break;
			case 1 :
			OBarMoveToPosition(-120);
			break;
			case 2 :
			OBarMoveToPosition(-4);
			OWrist.move_absolute(-10, 50);
			break;
			case 3 :
			OBarMoveToPosition(0);
			break;
		}

		// state machine for mogo intake system
		switch (mogo_state) {
			case 0 :
				// power the mogo intake until the mogo_limit is triggered
				if (MogoLimit.get_value() == 0) {
					// only close the cap if the mogo is out of the way
					if (MogoLeft.get_position() < 100) {
						mogo_shifter_state = false;
					}
					MogoLeft = -70;
					MogoRight = -70;
					MotorPriority[MOGO] = 2;
				}
				// stop the intake if it is triggered
				else {
					MogoLeft = 0;
					MogoRight = 0;
					mogo_shifter_state = false;
					MotorPriority[MOGO] = 0;
				}
			MotorPriority[ONE] = 1;
			break;
			case 1 :
			// move the mogo intake to the max out position
			// move with voltage if the mogo_limit is triggered
			if (MogoLimit.get_value() == 1) {
				MogoLeft = 70;
				MogoRight = 70;
			}
			// move with move_absolute if the mogo_limit is triggered
			else {
				MogoLeft.move_absolute(MOGO_MAX_POS, 100);
				MogoRight.move_absolute(MOGO_MAX_POS, 100);
			}
			// flatten the mogo intake so that it is easier to grab a mogo
			mogo_shifter_state = true;
			MotorPriority[MOGO] = 2;
			MotorPriority[ONE] = 1;			
			break;
			case 2 :
			OWrist.move_absolute(100, 50);
			if (MogoLeft.get_position() < (MOGO_MID_POS - 20)) {
				mogo_shifter_state = true;
				if (MogoLimit.get_value() == 1) {
					MogoLeft = 70;
					MogoRight = 70;
				}
				else {
					MogoLeft.move_absolute(MOGO_MID_POS, 100);
					MogoRight.move_absolute(MOGO_MID_POS, 100);
				}
			}
			else {
				mogo_state++;
				OBar_state = 1;
			}
			MotorPriority[MOGO] = 2;
			MotorPriority[ONE] = 1;
			break;
			case 3 :
			OWrist.move_absolute(100, 50);
			MogoLeft.move_absolute(MOGO_MID_POS, 100);
			MogoRight.move_absolute(MOGO_MID_POS, 100);
			
			if (OBarAngle() < -45) {
				mogo_shifter_state = true;
			}

			if (OBarAngle() < -110) {
				mogo_state++;
			}
			MotorPriority[MOGO] = 1;
			MotorPriority[ONE] = 1;
			break;
			case 4 :
			OWrist.move_absolute(-50, 200);
			MogoLeft.move_absolute(MOGO_MID_POS, 100);
			MogoRight.move_absolute(MOGO_MID_POS, 100);
			needle_state = true;
			mogo_shifter_state = false;
			MotorPriority[MOGO] = 1;
			MotorPriority[ONE] = 1;
			break;
			case 5 :
			needle_state = false;
			OWrist.move_absolute(200, 50);
			OBar_state = 0;
			mogo_shifter_state = true;

			if (OBarAngle() > -60)
				mogo_state = 0;
			break;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


		// Claw //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		if (partner.get_digital_new_press(DIGITAL_X))
			claw_state = !claw_state;

		Claw.set_value(claw_state);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


		// Arm //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		if (partner.get_digital_new_press(DIGITAL_R1)) {
			if (arm_state == ARM_MIN) arm_state = ARM_HOVER;
			else if (arm_state == ARM_HOVER || arm_state == ARM_STACK) arm_state = ARM_MAX;
			else if (arm_state == ARM_MAX) arm_state = ARM_STACK;
		}
		else if (partner.get_digital_new_press(DIGITAL_R2)) {
			arm_state = ARM_MIN;
		}

		switch (arm_state) {
			case ARM_MIN :
			if (ArmLimit.get_value() == 0) {
				ArmLeft.move_absolute(ARM_MIN_POS, 200);
				ArmRight.move_absolute (ARM_MIN_POS, 200);
				MotorPriority[ARM] = 2;
			}
			else {
				ArmLeft = 0;
				ArmRight = 0;
				MotorPriority[ARM] = 0;
			}
			break;
			case ARM_HOVER :
			if (ArmLimit.get_value() == 1) {
				ArmLeft = 70;
				ArmRight = 70;
			}
			else {
				ArmLeft.move_absolute(ARM_HOVER_POS, 100);
				ArmRight.move_absolute(ARM_HOVER_POS, 100);
			}
			MotorPriority[ARM] = 1;
			break;
			case ARM_STACK :
			ArmLeft.move_absolute(ARM_STACK_POS, 100);
			ArmRight.move_absolute(ARM_STACK_POS, 100);
			MotorPriority[ARM] = 2;
			break;
			case ARM_MAX :
			ArmLeft.move_absolute(ARM_MAX_POS, 200);
			ArmRight.move_absolute(ARM_MAX_POS, 200);
			MotorPriority[ARM] = 2;
			break;
			case ARM_DOCK_POS :
			ArmLeft.move_absolute(ARM_DOCK_POS, 200);
			ArmRight.move_absolute(ARM_DOCK_POS, 200);
			MotorPriority[ARM] = 2;
			break;
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//*/
		delay(10);
	}
}
