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
#include "defs.hpp"
#include "sequences.hpp"


// global object pointer definition
namespace go
{
    CRNav *nav = nullptr;
    Arm *arm = nullptr;
    PomContainer *pom = nullptr;
}


// what the robot should do whitout the cli (basically the normal main function)
void defaultRun()
{
    // home coordinate system
    sq::alignBack();

    msleep(1000);

    // drive to home position
    sq::driveBaseOffset();
    // drive to collection starting point
    sq::homeToPomStart();
    // collect poms
    sq::sortPoms();
    // drive to noodle start point
    sq::pomEndToNoodleStart();
    // get middle noodle and place on rack
    sq::doNoodleTask();


    /*This part is the knock down Rock-A-Stack!*/
    /*
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
    sq::alignBack();
    go::nav->driveDistance(30);
    go::nav->rotateBy(-90);
    */
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

    // initializing required components
    go::pom->initialize();
    go::pom->close();
    go::arm->initialize();
    go::arm->setServoSpeed(ARM_SERVO_SPEED);
    go::nav->initialize();
    go::nav->setMotorSpeed(NAV_SPEED);
    
    // initial calibration
    //go::arm->calibrateY();


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
        case 'h':   // home
            std::cin >> cmd;
            if (cmd == 'y')
                go::arm->calibrateY();
            else if (cmd == 'b')
                sq::alignBack();
            else if (cmd == 'f')
                sq::alignFront();
            else if (cmd == 'l')
                sq::alignLine();
            else if (cmd == 'r')
                sq::alignRight();
            else if (cmd == 'c')
                sq::centerOnLine();
            else if (cmd == 't')
            {
                std::cin >> p1;
                sq::trackLineUntil(p1);
            }
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

        case 'q':   // "quit"
            cmdloop = false;
            break;
        
        case 'n':
        {
            std::cin >> cmd;
            switch (cmd)
            {
            case 'u':   // up
                sq::pickUpNoodle();
                break;

            case 'd':   // down
                sq::placeNoodle();
                break;
            
            case 'a':   // all
                sq::doNoodleTask();
                break;
            
            default:
                break;
            }
            break;
        }
        case 'p':
        {
            std::cin >> cmd;
            switch (cmd)
            {
            case 'b':
                sq::driveBaseOffset();
                break;
            case 's':   // up
                sq::sortPoms();
                break;
            case 'a':
                sq::driveBaseOffset();
                sq::sortPoms();
                break;
            
            default:
                break;
            }
            break;
        }
        
        default:
            break;
        }
    }

    go::pom->terminate();
    go::arm->terminate();
    go::nav->terminate();

    return 0;
}