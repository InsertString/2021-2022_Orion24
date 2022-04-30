#include "main.h"

void comp_auto() {
    arm_down();
	quadratic_bezi_curve start_curve;
	start_curve.set_points(Vector2D(0,0), Vector2D(-50, 110), Vector2D(-150,210));
	drive_curve(start_curve, PIDConstants(9,0, 30), PIDConstants(9,0,30), 200);
    drive_straight(50, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 1000, 5);
	delay(250);
	drive_straight(-270, PIDConstants(3,0,30), PIDConstants(1,0,0), 80, 5000, 5);
    turn(45, PIDConstants(3,0.0002,30), 5000, 0.75);
    delay(500);
    arm_up();
    idle_conv(-40);
    drive_straight(-50, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    rings_open();
    wall_square(1000);
    drive_straight(50, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    store_goal();
    delay(2000);
    wall_square(1000);
    rings_close();
    delay(500);
    drive_straight(110, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    deploy_stage_1();
    idle_conv(0);
    turn(90, PIDConstants(3,0.0002,30), 5000, 0.75);
    arm_down();
    delay(500);
    drive_straight(50, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 2000, 5);
    arm_up();
    power_drive_directional(-50, 0);
    delay(750);
    power_drive_directional(0,0);
}

void comp_auto_2() {
    arm_down();
    drive_straight(350, PIDConstants(3,0,30), PIDConstants(1,0,0), 127, 2000, 2);
	delay(500);
	drive_straight(-270, PIDConstants(3,0,30), PIDConstants(1,0,0), 80, 5000, 5);
    turn(40, PIDConstants(3,0.0002,30), 5000, 0.75);
    delay(500);
    arm_up();
    idle_conv(-40);
    wall_square(1000);
    rings_open();
    drive_straight(80, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    store_goal();
    wall_square(1000);
    drive_straight(50, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    delay(2000);
    wall_square(1000);
    rings_close();
    delay(500);
    drive_straight(110, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 5000, 5);
    deploy_stage_1();
    idle_conv(0);
    turn(90, PIDConstants(3,0.0002,30), 5000, 0.75);
    arm_down();
    delay(500);
    drive_straight(70, PIDConstants(3,0,30), PIDConstants(1,0,0), 60, 1000, 5);
    arm_up();
    power_drive_directional(-50, 0);
    delay(750);
    power_drive_directional(0,0);
    //*/
}

void skills_auto() {

}
