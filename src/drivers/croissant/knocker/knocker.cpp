/**
 * @file pom_container.hpp
 * @author melektron
 * @brief driver class for the arm extending the independent structure for knockin over rocka-stand
 * @version 0.1
 * @date 2023-02-08
 *
 * @copyright Copyright FrenchBakery (c) 2023
 *
 */

#include <iostream>
#include <cmath>
#include <kipr/time/time.h>

#include "knocker.hpp"

// arm servo position defs
#define POSITION_HOLD 0
#define POSITION_PLACE 700
#define POSITION_RETRACT 2047
// lift servo position defs
#define POSITION_UP 1951
#define POSITION_DOWN 1511

#define SERVO_SPEED 2000


Knocker::Knocker(int arm_port, int lift_port)
    : arm_servo(arm_port),
      lift_servo(lift_port)
{
}

void Knocker::initialize()
{
    arm_servo.enable();
    lift_servo.enable();
    arm_servo.setSpeed(SERVO_SPEED);
    lift_servo.setSpeed(SERVO_SPEED);
}

void Knocker::terminate()
{
    arm_servo.setPosition(POSITION_RETRACT);
    arm_servo.disable();
}

void Knocker::place()
{
    arm_servo.setPosition(POSITION_PLACE);
}

void Knocker::retract()
{
    arm_servo.setPosition(POSITION_RETRACT);
}

void Knocker::hold()
{
    arm_servo.setPosition(POSITION_HOLD);
}

void Knocker::up()
{
    lift_servo.setPosition(POSITION_UP);
}

void Knocker::down()
{
    lift_servo.setPosition(POSITION_DOWN);
}