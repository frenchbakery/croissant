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

#include <kipr/servo/servo.hpp>

class PomContainer
{
protected:
    // Hardware access
    kipr::servo::Servo selector_servo;

    /**
     * @brief moves the servo to the specified position in the
     * specified time duration
     * 
     * @param target_pos target position
     * @param time time in ms it should take
     */
    void moveServoToIn(int target_pos, int time);

public:

    PomContainer(int sp);

    void initialize();
    void terminate();

    void open();
    void close();
};

namespace go
{
    extern ::PomContainer pom_container;
};