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

#pragma once

#include <kiprplus/smooth_servo.hpp>

class Knocker
{
protected:
    // Hardware access
    kp::SmoothServo arm_servo;
    kp::SmoothServo lift_servo;

public:

    Knocker(int arm_port, int lift_port);

    void initialize();
    void terminate();

    void place();
    void retract();
    void hold();
    void up();
    void down();
};
