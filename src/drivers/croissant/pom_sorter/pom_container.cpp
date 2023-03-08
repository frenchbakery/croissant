/**
 * @file pom_container.hpp
 * @author melektron
 * @brief driver class for the pom sorter of the croissant robot
 * @version 0.1
 * @date 2023-02-08
 *
 * @copyright Copyright FrenchBakery (c) 2023
 *
 */

#include <iostream>
#include <cmath>
#include <kipr/time/time.h>

#include "pom_container.hpp"

PomContainer go::pom_container(0);

// servo position defs
#define POSITION_OPEN 1365
#define POSITION_CLOSED 744
#define POSITION_HOLD 819

PomContainer::PomContainer(int sp)
    : selector_servo(sp)
{
}

void PomContainer::moveServoToIn(int target_pos, int time)
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

void PomContainer::initialize()
{
    selector_servo.enable();
    moveServoToIn(POSITION_HOLD, 500);
    // msleep(200);
    // selector_servo.disable();
}

void PomContainer::terminate()
{
    // selector_servo.enable();
    moveServoToIn(POSITION_HOLD, 500);
    selector_servo.disable();
}

void PomContainer::open()
{
    // selector_servo.enable();
    moveServoToIn(POSITION_OPEN, 400);
    // selector_servo.disable();
}

void PomContainer::close()
{

    // selector_servo.enable();
    moveServoToIn(POSITION_CLOSED, 400);
    // selector_servo.disable();
}