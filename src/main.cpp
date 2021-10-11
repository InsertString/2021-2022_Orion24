#include "main.h"

// controller
Controller master(E_CONTROLLER_MASTER);

// drive motors
Motor driveLFL(11);
Motor driveLFR(12);
Motor driveLBL(13);
Motor driveLBR(14);
Motor driveRFL(15);
Motor driveRFR(16);
Motor driveRBL(17);
Motor driveRBR(18);

Motor SideRollers(9);
Motor Elevator(2);

Motor MogoLeft(20);
Motor MogoRight(1);

// sensors
Imu imu(19);

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

	double rollerState = 0;
	bool pulse_elevator = false;
	Timer pulse_timer;
	double targetAngle = 0;
	bool holdAngle = false;

	while (true) {
		if (imu.is_calibrating() == false) {
			if (master.get_digital_new_press(DIGITAL_X)) {
				targetAngle = imu.get_heading();
				holdAngle = (holdAngle == true) ? false : true;
				relativeDriveControl(targetAngle, holdAngle);
			}
			else {
				relativeDriveControl(imu.get_heading(), holdAngle);
			}
		}

		if (master.get_digital_new_press(DIGITAL_L1)) {
			pulse_elevator = (pulse_elevator == true) ? false : true;
			rollerState = (rollerState == -127) ? 0 : -127;
			pulse_timer.reset();
		}

		if (pulse_elevator == true) {
			Elevator = 80;
		}
		else {
			Elevator = 0;
		}

		if (master.get_digital(DIGITAL_R1)) {
			MogoLeft = 127;
			MogoRight = -127;
		}
		else if (master.get_digital(DIGITAL_R2)) {
			MogoLeft = -127;
			MogoRight = 127;
		}
		else {
			MogoLeft = 0;
			MogoRight = 0;
		}

		SideRollers = rollerState;
		delay(20);
	}
}
