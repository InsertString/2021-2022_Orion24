#include "main.h"

int chassis_step = 0;
int intake_state = 0;
bool claw_state = false;
bool needle_state = false;
int mogo_state = 0;
int arm_state = 0;
int OBar_state = 0;

int auto_step = 0;
int auto_part = 0;

Timer auto_timer_1;


void change_auto_step() {
    auto_step++;
    chassis_step = 0;
}

void change_auto_part() {
    auto_step = 0;
    chassis_step = 0;
}

void set_up_auto() {
    auto_part = 0;
    auto_step = 0;
    chassis_step = 0;
    arm_state = 0;
    intake_state = 0;
    mogo_state = 4;
    claw_state = false;
    auto_timer_1.reset();
}

bool SkillsPt1() {
    switch (auto_step) {
        case 0 :
        
        break;
    }
}

void SkillsAuto() {
    switch (auto_part) {
        case 0 :
        if (SkillsPt1() == true) auto_part++;
        break;
    }
}
