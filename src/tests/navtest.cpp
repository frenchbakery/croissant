/**
 * @file navtest.cpp
 * @author melektron
 * @brief test for the navigation system
 * @version 0.1
 * @date 2023-02-15
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#include <iostream>
#include <el/vec.hpp>
#include <kipr/motors.hpp>
#include <kipr/util.hpp>

#include "drivers/navigation/croissant/ramped_motor.hpp"

/*
BackEMF counter(0);
Motor motor(0);

int main()
{
    el::vec2_t myvector(el::cart_t(3, 3));
    std::cout << myvector.get_x() << "," << myvector.get_y() << std::endl;
    std::cout << myvector.get_r() << "v" << myvector.get_phi() << std::endl; 
    
    int goal = 1900;
    int currgoal = goal;
    int speed = 1500;
    bool state = true;
    motor.clearPositionCounter();
    motor.moveToPosition(speed, currgoal);

    const int allowedDelta = 1;
    
    for (;;)
    {
        int val = counter.value();
        //std::cout << "Motor Position: " << val << std::endl;

        // check if done
        int delta = std::abs(val - currgoal);
        if (delta < allowedDelta)
        {
            std::cout << "Finished!" << std::endl;
            motor.freeze();
            msleep(1000);
            state = !state;
            if (state) currgoal = goal;
            else currgoal = 0;
            //motor.freeze();
            motor.moveToPosition(speed, currgoal);
        }
        else
        {
            int scaledSpeed = speed;
            if (delta < 170)
                scaledSpeed = speed * delta / 200 + 30;
            motor.moveToPosition(scaledSpeed, currgoal);
        }


        //msleep(50);
        
    }
    return 0;
}*/

RampedMotor motor(0);

int main()
{
    //motor.setAccuracy(2);
    int goal = 1900;
    bool state = true;
    for (;;)
    {
        std::cout << "Start" << std::endl;
        motor.moveToPosition(500, (state ? goal : 0));
        motor.blockMotorDone();
        std::cout << "Finished" << std::endl;
        state = !state;
        msleep(1000);
    }

    return 0;
}