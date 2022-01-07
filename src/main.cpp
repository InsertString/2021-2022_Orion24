#include "main.h"

// controller
Controller master(E_CONTROLLER_MASTER);

// drive motors
Motor driveLFL(19);
Motor driveLFR(20);
Motor driveLBL(9);
Motor driveLBR(10);
Motor driveRFL(11);
Motor driveRFR(12);
Motor driveRBL(1);
Motor driveRBR(2);

Motor ArmLeft(15);
Motor ArmRight(14);

Motor Elevator(17);

Motor MogoLeft(18);
Motor MogoRight(13);

// sensors
Imu imu(3);

ADIEncoder RightEncoder(3, 4, false);
ADIEncoder BackEncoder(5, 6, false);

ADIDigitalOut Claw(7);
ADIDigitalOut MogoShifter(2);

ADIDigitalIn MogoLimit(8);


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	//Claw.set_value(true);
}

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
	bool elevator_state = false;
	bool mogo_state = false;

	while (true) {
		lcd::print(0, "Mogo[%f]", MogoRight.get_position());
		lcd::print(1, "ArmL[%f]", ArmLeft.get_position());
		lcd::print(2, "ArmR[%f]", ArmRight.get_position());

		if (MogoLimit.get_value() == 1) {
			MogoRight.tare_position();
			MogoLeft.tare_position();
		}

		power_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		if (elevator_state) {
			Elevator = -127;
		}
		else {
			Elevator = 0;
		}

		if (master.get_digital_new_press(DIGITAL_X)) {
			elevator_state = !elevator_state;
		}

		if (master.get_digital_new_press(DIGITAL_L1)) {
			mogo_state = true;
		}

		if (master.get_digital(DIGITAL_L2) && MogoLimit.get_value() == 0) {
			MogoLeft = -127;
			MogoRight = 127;
			mogo_state = false;
		}
		else if (mogo_state == true) {
			MogoLeft.move_absolute(1320, 100);
			MogoRight.move_absolute(-1320, 100);
			mogo_shifter_state = true;
		}
		else {
			MogoLeft = 0;
			MogoRight = 0;
			mogo_shifter_state = false;
		}

		if (master.get_digital_new_press(DIGITAL_Y)) {
			claw_state = !claw_state;
		}

		Claw.set_value(claw_state);

		MogoShifter.set_value(mogo_shifter_state);

		if (master.get_digital(DIGITAL_R1)) {
			ArmLeft = -127;
			ArmRight = 127;
		}
		else if (master.get_digital(DIGITAL_R2)) {
			ArmLeft = 127;
			ArmRight = -127;
		}
		else {
			ArmLeft = 0;
			ArmRight = 0;
		}

		delay(5);
	}
}
