#include "main.h"

// Object Declarations //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// Controller
Controller master(E_CONTROLLER_MASTER);
Controller partner(E_CONTROLLER_PARTNER);

// Motors
Motor MogoLeft(10, true);
Motor MogoRight(9, false);
Motor Intake(20, true);
Motor ArmLeft(14, false);
Motor ArmRight(15, true);
Motor Wrist(17);

Motor DriveLF(19);
Motor DriveLM(18);
Motor DriveLB(8);
Motor DriveRF(12, true);
Motor DriveRM(13, true);
Motor DriveRB(2, true);

// Sensors
Imu imu(5);
ADIEncoder YEncoder(3, 4, false);
ADIEncoder XEncoder(1, 2, false);
ADIDigitalIn MogoEndstop(5);
ADIDigitalIn ArmEndstop(6);

// Pistons
ADIDigitalOut Claw(7);
ADIDigitalOut Needle(8);

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
	// comp:   
	// skills: 
	odom.configure_starting(Vector2D(0,0), 0);
	odom.configure(8.25, 20, 3.22, 20, 20);
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
	comp_auto();
}


void opcontrol() {
	bool mogo_up = false;
	bool wrist_loaded = false;
	bool claw_state = false;
	bool needle_state = false;
	bool pulse_intake = false;
	Timer wrist_timer;
	Timer ring_timer;

	while (true) {
		debug_base_systems();

		power_drive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_LEFT_X), partner.get_analog(ANALOG_LEFT_Y), partner.get_analog(ANALOG_RIGHT_Y));

		if (MogoEndstop.get_value() == MOGO_ENDSTOP_TRIGGERED) {
			reset_mogo_position();
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


		if (partner.get_digital_new_press(DIGITAL_B)) {
			wrist_loaded = !wrist_loaded;
			wrist_timer.reset();
		}

		if (wrist_loaded) {
			if (wrist_timer.delta_time() < 500) {
				power_wrist(90);
			}
			else {
				power_wrist(3);
			}
		}
		else {
			if (wrist_timer.delta_time() < 500) {
				power_wrist(-90);
			}
			else {
				power_wrist(-3);
			}
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

		if (ArmEndstop.get_value() == ARM_ENDSTOP_TRIGGERED) {
			reset_arm_position();
		}

		if (partner.get_digital(DIGITAL_L2)) {
			if (ArmEndstop.get_value() == false) {
				power_arm(-80);
			}
			else {
				power_arm(0);
			}
		}
		else if (partner.get_digital(DIGITAL_RIGHT)) {
			if (ArmEndstop.get_value() == true) power_arm(80);
			else move_arm_to_position(90, 100);
		}
		else if (partner.get_digital(DIGITAL_L1)) {
			power_arm(127);
		}
		else {
			power_arm(master.get_analog(ANALOG_RIGHT_Y));
		}

		if (partner.get_digital_new_press(DIGITAL_R1)) {
			claw_state = !claw_state;
		}

		if (partner.get_digital_new_press(DIGITAL_R2)) {
			needle_state = !needle_state;
		}

		Claw.set_value(claw_state);
		Needle.set_value(needle_state);

		delay(10);
	}
}
