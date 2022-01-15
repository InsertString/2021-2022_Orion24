#include "main.h"

// controller
Controller master(E_CONTROLLER_MASTER);

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
#define ARM_MIN 0
#define ARM_HOVER 1
#define ARM_STACK 2
#define ARM_MAX 3
#define ARM_MIN_POS 0
#define ARM_HOVER_POS 200
#define ARM_STACK_POS 1000
#define ARM_MAX_POS 1800

// ring elevator
Motor Elevator(17, true);
#define ELEVATOR_STOP 0
#define ELEVATOR_INTAKE 1
#define ELEVATOR_OUTAKE 2

// mogo intake
Motor MogoLeft(18);
Motor MogoRight(13, true);
#define MOGO_MAX_POS 1200
#define MOGO_MID_POS 800
#define MOGO_MIN_POS 0

// sensors
Imu imu(3);

ADIEncoder RightEncoder(5, 6, true);
ADIEncoder BackEncoder(3, 4, false);
ADIDigitalIn ArmLimit(1);
ADIDigitalIn MogoLimit(8);

// pneumatics
ADIDigitalOut Claw(7);
ADIDigitalOut MogoShifter(2);

/*
 * Odometry Task
 * Tracks the position of the robot
*/
void odom_task(void* param) {
	while (true) {
		CalculatePosition();
		odomDebug();
	}
}

Task odom (odom_task, NULL, TASK_PRIORITY_DEFAULT-1, TASK_STACK_DEPTH_DEFAULT, "ODOM");

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	bool claw_state = true;
	bool mogo_shifter_state = false;
	bool mogo_state = false;
	unsigned int elevator_state = 0;
	unsigned int arm_state = 0;

	while (true) {
		lcd::print(0, "Mogo[%f]", MogoRight.get_position());
		lcd::print(1, "ArmL[%f]", ArmLeft.get_position());
		lcd::print(2, "ArmR[%f]", ArmRight.get_position());

		if (MogoLimit.get_value() == 1) {
			MogoRight.tare_position();
			MogoLeft.tare_position();
		}

		if (ArmLimit.get_value() == 1) {
			ArmRight.tare_position();
			ArmLeft.tare_position();
		}

		// drivetrain code
		power_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		// elevator
		if (master.get_digital_new_press(DIGITAL_Y)) {
			if (elevator_state == 1)
				elevator_state = 0;
			else
				elevator_state++;
		}

		switch (elevator_state) {
			case ELEVATOR_STOP :
			Elevator = 0;
			break;
			case ELEVATOR_INTAKE :
			Elevator = 100;
			break;
		}

		// mogo intake
		MogoShifter.set_value(mogo_shifter_state);

		if (master.get_digital_new_press(DIGITAL_L1))
			mogo_state = !mogo_state;

		if (mogo_state == false && MogoLimit.get_value() == 0) {
			if (MogoLeft.get_position() < 100) {
				mogo_shifter_state = false;
			}
			MogoLeft = -70;
			MogoRight = -70;
		}
		else if (mogo_state == true) {
			if (MogoLimit.get_value() == 1) {
				MogoLeft = 70;
				MogoRight = 70;
			}
			else {
				MogoLeft.move_absolute(MOGO_MAX_POS, 100);
				MogoRight.move_absolute(MOGO_MAX_POS, 100);
			}
			mogo_shifter_state = true;
		}
		else {
			MogoLeft = 0;
			MogoRight = 0;
			mogo_shifter_state = false;
		}

		// claw
		if (master.get_digital_new_press(DIGITAL_X))
			claw_state = !claw_state;

		Claw.set_value(claw_state);

		// arm
		if (master.get_digital_new_press(DIGITAL_R1)) {
			if (arm_state == ARM_MIN) arm_state = ARM_HOVER;
			else if (arm_state == ARM_HOVER || arm_state == ARM_STACK) arm_state = ARM_MAX;
			else if (arm_state == ARM_MAX) arm_state = ARM_STACK;
		}
		else if (master.get_digital_new_press(DIGITAL_R2)) {
			arm_state = ARM_MIN;
		}

		switch (arm_state) {
			case ARM_MIN :
			if (ArmLimit.get_value() == 0) {
				ArmLeft.move_absolute(ARM_MIN_POS, 200);
				ArmRight.move_absolute (ARM_MIN_POS, 200);
			}
			else {
				ArmLeft = 0;
				ArmRight = 0;
			}
			break;
			case ARM_HOVER :
			ArmLeft.move_absolute(ARM_HOVER_POS, 100);
			ArmRight.move_absolute(ARM_HOVER_POS, 100);
			break;
			case ARM_STACK :
			ArmLeft.move_absolute(ARM_STACK_POS, 200);
			ArmRight.move_absolute(ARM_STACK_POS, 200);
			break;
			case ARM_MAX :
			ArmLeft.move_absolute(ARM_MAX_POS, 200);
			ArmRight.move_absolute(ARM_MAX_POS, 200);
			break;
		}

		delay(5);
	}
}
