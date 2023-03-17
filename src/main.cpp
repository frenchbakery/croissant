/**
 * @file main.cpp
 * @author LiloZz
 * @brief croissant robot main file
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright FrenchBakery(c) 2023
 * 
 */

#include <iostream>

#include <kipr/time/time.h>
#include <kipr/wait_for/wait_for.h>
#include <kipr/digital/digital.hpp>
#include <kipr/analog/analog.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"
#include "drivers/croissant/arm/arm.hpp"


#define D2R(deg) ((deg)*M_PI/180)

#define CALIB_SPEED 500 // TPS
#define LIGHT_THRESHOLD 1500

#define MOTOR_PORT 2
#define LEFT_SERVO 1
#define RIGHT_SERVO 2
#define END_SWITCH 2



kipr::digital::Digital back_button_left(0);
kipr::digital::Digital back_button_right(1);
kipr::analog::Analog line_left(0);
kipr::analog::Analog line_right(1);



namespace go
{
    CRNav *nav = nullptr;
    Arm *arm = nullptr;

}

void straightLineSorter(double offset = 0)
{
    double first_distance = 15;
    go::nav->driveDistance(first_distance + offset);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.open();
    go::nav->driveDistance(41 - first_distance);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();
    //go::pom_container.hold();
    //correctAlignment();
}
    void homeBase()
    {
        go::nav->driveDistance(5);
        go::nav->startSequence();
        go::nav->awaitSequenceComplete();
        msleep(1000);
    }

void alignBack()
{
    // Find zero point in X direction and align robot to game table
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(-100);
    go::nav->driveRightSpeed(-100);

    for (;;)
    {
        if (back_button_left.value())
            go::nav->driveLeftSpeed(0);
        else 
            go::nav->driveLeftSpeed(-CALIB_SPEED);
        
        if (back_button_right.value())
            go::nav->driveRightSpeed(0);
        else 
            go::nav->driveRightSpeed(-CALIB_SPEED);

        if (back_button_left.value() && back_button_right.value())
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
}

void alignFront()
{
    // Find zero point in X direction and align robot to game table
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    for (;;)
    {
        if (line_left.value() > LIGHT_THRESHOLD)
            go::nav->driveLeftSpeed(0);
        else 
            go::nav->driveLeftSpeed(CALIB_SPEED);
        
        if (line_right.value() > LIGHT_THRESHOLD)
            go::nav->driveRightSpeed(0);
        else 
            go::nav->driveRightSpeed(CALIB_SPEED);

        if (line_left.value() > LIGHT_THRESHOLD && line_right.value() > LIGHT_THRESHOLD)
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
}

int main()
{
    /*This part is the red pom pom sorter!*/

    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    go::nav = new CRNav;
    go::arm = new Arm(MOTOR_PORT, LEFT_SERVO, RIGHT_SERVO, END_SWITCH);
    //Press button on Wombat before programm starts
    //wait_for_side_button();

    go::pom_container.initialize();
    go::nav->initialize();
    go::nav->setMotorSpeed(1200);

    alignBack();
    msleep(1000);

    // drive to home positoin
    homeBase();

    //get out of starting box
    go::nav->rotateBy(D2R(-90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(36);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    //collect fist poms
    alignBack();
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    homeBase();
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.open();
    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    
    //get into aligned position parallel to black tape
    go::nav->driveDistance(-6);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(-265));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    //Takes red poms from the line until end is reached
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
            straightLineSorter(-15);
        else
            straightLineSorter();
    }
    
    //Turns 180 to drive in other direction
    go::nav->driveDistance(-10);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(180));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    alignBack();
    go::nav->driveDistance(20);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    /*This part is the knock down Rock-A-Stack!*/
    go::nav->driveDistance(30);
    go::arm-> setYPerc(100);
    go::arm->waitForY();
    go::arm->setYPerc(50);
    go::arm->waitForY();
    go::arm->grab(0);
    go::arm->waitForGrab();
    go::arm->setYPerc(0);
    go::arm->waitForY();
    go::arm->grab(50);
    go::arm->waitForGrab();
    go::arm->setYPerc(100);
    go::arm->waitForY(); 
    go::nav->driveDistance(-30);
    go::arm->grab(0);
    go::arm->waitForGrab();
    go::nav->driveDistance(-10);
    go::nav->rotateBy(90);
    alignBack();
    go::nav->driveDistance(30);
    go::nav->rotateBy(-90);

    msleep(2000);
    go::pom_container.terminate();
    go::nav->terminate();
}