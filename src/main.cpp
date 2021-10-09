#include "main.h"

// controller
Controller master(E_CONTROLLER_MASTER);

// drive motors
Motor driveFL(11);
Motor driveFR(12);
Motor driveBL(13);
Motor driveBR(14);
Motor driveSF(15);
Motor driveSB(16);

Motor escalator(20);

//mogo intake
Motor mogoIntakeL(1);
Motor mogoIntakeR(2);
Motor arm(3);
Motor clamp(17);

// sensors
Imu imu(10);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	//delay(1000);
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
	double t_L = 0;
	double e_L = 0;
	double t_R = 0;
	double e_R = 0;
	double t_A = 0;
	double e_A = 0;

	while (true) {
		if (imu.is_calibrating() == false) {
			relativeDriveControl();
		}


		if (master.get_digital(DIGITAL_L1)) {
			mogoIntakeL = 100;
			mogoIntakeR = -100;
			t_L = mogoIntakeL.get_position();
			t_R = mogoIntakeR.get_position();
		}
		else if (master.get_digital(DIGITAL_L2)) {
			mogoIntakeL = -30;
			mogoIntakeR = 30;
			t_L = mogoIntakeL.get_position();
			t_R = mogoIntakeR.get_position();
		}
		else {
			e_L = t_L - mogoIntakeL.get_position();
			e_R = t_R - mogoIntakeR.get_position();
			mogoIntakeL = e_L * 0.1;
			mogoIntakeR = e_R * 0.1;
		}


		if (master.get_digital(DIGITAL_R1)) {
			arm = 127;
			t_A = arm.get_position();
		}
		else if (master.get_digital(DIGITAL_R2)) {
			arm = -100;
			t_A = arm.get_position();
		}
		else {
			e_A = t_A - arm.get_position();
			arm = e_A * 0.1;
		}

		if (master.get_digital(DIGITAL_X)) {
			clamp = 60;
		}
		else if (master.get_digital(DIGITAL_B)) {
			clamp = -60;
		}
		else {
			clamp = 0;
		}
		//power_drive(master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));
		//driveBL = master.get_analog(ANALOG_LEFT_Y);
		printf("test\n");
		delay(20);
	}
}
