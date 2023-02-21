/**
 * @file pom_sorter.hpp
 * @author melektron
 * @brief driver class for the pom sorter of the croissant robot
 * @version 0.1
 * @date 2023-02-08
 *
 * @copyright Copyright FrenchBakery (c) 2023
 *
 */

#pragma once

#include <kipr/servo.hpp>

class PomSorter
{
protected:
    // Hardware access
    Servo selector_servo;

    /**
     * @brief moves the servo to the specified position in the
     * specified time duration
     * 
     * @param target_pos target position
     * @param time time in ms it should take
     */
    void moveServoToIn(int target_pos, int time);

public:

    PomSorter(int sp);

    void initialize();
    void terminate();
    enum pos_t
    {
        green,
        red
    };

    void setColorSelector(pos_t col);
};

namespace go
{
    extern ::PomSorter pom_sorter;
};