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

#include "global_objects.hpp"
#include "utils.hpp"


// pinout
#define POM_SORTER_SERVO 0
#define ARM_MOTOR_PORT 2
#define ARM_LEFT_SERVO 1
#define ARM_RIGHT_SERVO 2
#define ARM_END_SWITCH 2

// non-critical sensor calibration values
#define CALIB_SPEED 500 // TPS
#define LIGHT_THRESHOLD 1500


// global object pointer definition
namespace go
{
    CRNav *nav = nullptr;
    Arm *arm = nullptr;
    PomContainer *pom = nullptr;
}


kipr::digital::Digital back_button_left(0);
kipr::digital::Digital back_button_right(1);
kipr::analog::Analog line_left(0);
kipr::analog::Analog line_right(1);



void straightLineSorter(double offset = 0)
{
    double first_distance = 15;
    go::nav->driveDistance(first_distance + offset);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom->open();
    go::nav->driveDistance(41 - first_distance);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom->close();
    //go::pom->hold();
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

void sortPoms()
{
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
    go::pom->open();
    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom->close();
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
}

// what the robot should do whitout the cli (basically the normal main function)
void defaultRun()
{
    alignBack();
    msleep(1000);

    // drive to home positoin
    homeBase();

    sortPoms();

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
}

int main()
{
    // Start a bogus motor because this is required for some reason to get the PID controller to work
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    // creating clobal objects
    go::nav = new CRNav;
    go::arm = new Arm(ARM_MOTOR_PORT, ARM_LEFT_SERVO, ARM_RIGHT_SERVO, ARM_END_SWITCH);
    go::pom = new PomContainer(POM_SORTER_SERVO);

    //Press button on Wombat before programm starts
    //wait_for_side_button();


    std::cout << "hello1" << std::endl;

    // initializing required components
    go::pom->initialize();
    std::cout << "hello1" << std::endl;
    go::nav->initialize();
    std::cout << "hello1" << std::endl;
    go::nav->setMotorSpeed(500);
    std::cout << "hello1" << std::endl;
    //go::arm->calibrate();

    std::cout << "hello2" << std::endl;

    // command loop (to be exported to CLI module)
    bool cmdloop = true;
    while (cmdloop)
    {
        int p1;
        char cmd;
        std::cout << ">> ";
        std::cin >> cmd;
        switch (cmd)
        {
        case 'a':
            alignBack();
            break;
        case 'r':
            std::cin >> p1;
            go::nav->rotateBy(D2R(p1));
            go::nav->startSequence();
            break;
        case 'd':
            std::cin >> p1;
            go::nav->driveDistance(p1);
            go::nav->startSequence();
            break;
        case 'y':
            std::cin >> p1;
            go::arm->setYPerc(p1);
            break;
        case 't':
            std::cin >> p1;
            go::arm->tilt(p1);
            break;
        case 'g':
            std::cin >> p1;
            go::arm->grab(p1);
            break;

        case 's':   // "start"
            defaultRun();
            break;

        case 'q':   // "start"
            cmdloop = false;
            break;
        
        default:
            break;
        }
    }

    go::pom->terminate();
    go::nav->terminate();
}