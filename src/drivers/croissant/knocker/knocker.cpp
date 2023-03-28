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

// servo position defs
#define POSITION_OPEN 228//400
#define POSITION_CLOSED 983//1119
#define POSITION_HOLD 822//933

Knocker::Knocker(int sp)
    : selector_servo(sp)
{
}

void Knocker::moveServoToIn(int target_pos, int time)
{
    int pos = selector_servo.position();
    if (pos == target_pos) return;
    const int steps_size = 10;
    const int delay_per_step = (time * steps_size) / std::abs(target_pos - pos);
    const int direction = pos > target_pos ? -1 : 1; // decrement if target is smaller
    while (std::abs(target_pos - pos) >= steps_size)
    {
        selector_servo.setPosition(pos += direction * steps_size);
        msleep(delay_per_step);
    }
    // the final step can be smaller than the step size
    selector_servo.setPosition(target_pos);
}

void Knocker::initialize()
{
    selector_servo.enable();
    moveServoToIn(POSITION_HOLD, 500);
    // msleep(200);
    // selector_servo.disable();
}

void Knocker::terminate()
{
    // selector_servo.enable();
    moveServoToIn(POSITION_HOLD, 500);
    selector_servo.disable();
}

void Knocker::open()
{
    // selector_servo.enable();
    moveServoToIn(POSITION_OPEN, 400);
    // selector_servo.disable();
}

void Knocker::close()
{

    // selector_servo.enable();
    moveServoToIn(POSITION_CLOSED, 400);
    // selector_servo.disable();
}

void Knocker::hold()
{

    // selector_servo.enable();
    moveServoToIn(POSITION_HOLD, 400);
    // selector_servo.disable();
}