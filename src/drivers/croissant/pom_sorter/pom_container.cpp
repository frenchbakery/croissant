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
#include <kipr/servo.hpp>
#include <kipr/util.hpp>

#include "pom_container.hpp"

PomContainer go::pom_container(0);
using c_t = PomContainer::pos_t;


// servo position defs
#define POSITION_CENTER 1120
#define POSITION_RED    687
#define POSITION_GREEN  1467




PomContainer::PomContainer(int sp)
    : selector_servo(sp)
{
}

void PomContainer::moveServoToIn(int target_pos, int time)
{
    int pos = selector_servo.position();
    const int steps_size = 10;
    const int delay_per_step = (time * steps_size) / std::abs(target_pos - pos);
    const int direction = pos > target_pos ? -1 : 1;    // decrement if target is smaller
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
    selector_servo.setPosition(POSITION_CENTER);
    //msleep(200);
    //selector_servo.disable();
}

void PomContainer::terminate()
{
    //selector_servo.enable();
    selector_servo.setPosition(POSITION_CENTER);
    msleep(200);
    selector_servo.disable();
}

void PomContainer::setColorSelector(PomContainer::pos_t col)
{
    if (col == c_t::green)
    {
        std::cout << "selecting green" << std::endl;
        //selector_servo.enable();
        moveServoToIn(POSITION_GREEN, 1000);
        //selector_servo.disable();
    }
    else if (col == c_t::red)
    {
        std::cout << "selecting red" << std::endl;
        //selector_servo.enable();
        moveServoToIn(POSITION_RED, 1000);
        //selector_servo.disable();
    }
}