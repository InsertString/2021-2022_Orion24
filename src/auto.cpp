#include "main.h"

void skills_auto() {
    
}


void comp_auto() {
    Claw.set_value(false);
    home_mogo();
    power_drive(60,0,0,0);
    delay(1000);
    power_drive(10,0,0,0);
    delay(300);
    Claw.set_value(true);
    power_drive(-30,0,0,0);
    delay(1000);
    power_drive(0,0,0,0);
}
