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
#include <kipr/wombat.h>
#include <kipr/botball/botball.h>

#include "global_objects.hpp"
#include "utils.hpp"
#include "defs.hpp"
#include "sequences.hpp"
#include "waitforlight.h"


// global object pointer definition
namespace go
{
    CRNav *nav = nullptr;
    Arm *arm = nullptr;
    Knocker *kno = nullptr;
}

// wait for light sensor
kipr::analog::Analog wait_for_light_sensor(2);


// what the robot should do whitout the cli (basically the normal main function)
void defaultRun()
{
    // home coordinate system
    sq::alignBack();
    msleep(500);
    // drive to home position
    sq::driveBaseOffset();

    // wait for light (right now just wait for input)
    //calibrateLightSensor(wait_for_light_sensor);
    waitForLightOrInput(wait_for_light_sensor);
    
    //system("/home/access/projects/stopper/run/stopper &");
    shut_down_in(117);


    /*
    // drive to collection starting point
    sq::homeToPomStart();
    // collect poms
    sq::sortPoms();
    // drive to noodle start point
    sq::pomEndToNoodleStart();
    // get middle noodle and place on rack
    sq::doNoodleTask();
    */

    sq::homeToKnock();
    sq::knockOverStand();

    sq::knockEndToNoodleStart();

    sq::doNoodleTask();
}

int main()
{
    // Start a bogus motor because this is required for some reason to get the PID controller to work
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    // creating clobal objects
    go::nav = new CRNav;
    go::arm = new Arm(ARM_MOTOR_PORT, ARM_LEFT_SERVO, ARM_RIGHT_SERVO, ARM_END_SWITCH);
    go::kno = new Knocker(KNOCKER_ARM_SERVO, KNOCKER_LIFT_SERVO);

    //Press button on Wombat before programm starts
    //wait_for_side_button();

    // initializing required components
    go::kno->initialize();
    go::kno->hold();
    go::kno->down();
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
            else if (cmd == 'a')
                sq::approachRack();
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
        
        // S-Button on BotballUI
        case 's':   // "start"
            defaultRun();
            break;
        
        // B-Button on BotballUI
        case 'b':
            go::arm->calibrateY();
            break;

        case 'q':   // "quit"
            cmdloop = false;
            break;
        
        case 'n':
        {
            std::cin >> cmd;
            switch (cmd)
            {
            case 'r':   // rack
                sq::pickUpNoodleFromRack();
                break;

            case 's':   // stand
                sq::placeNoodleOnStand();
                break;

            case 'p':   // pickup
                sq::pickUpNoodleFromStand();
                break;

            case 'd':   // drop
                sq::dropNoodle();
                break;
                
            case 'a':   // all
                sq::doNoodleTask();
                break;
            
            default:
                break;
            }
            break;
        }
        case 'k':
        {
            std::cin >> cmd;
            switch (cmd)
            {
            case 'u':   // up
                go::kno->up();
                break;
            case 'd':
                go::kno->down();
                break;
            case 'h':
                go::kno->hold();
                break;
            case 'p':
                go::kno->place();
                break;
            case 'r':
                go::kno->retract();
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

    go::kno->terminate();
    go::arm->terminate();
    go::nav->terminate();

    alloff();
    //system("PID=$(pidof run/stopper) && echo \"Stopper: killing ${PID} now\" && kill -INT \"${PID}\"");

    return 0;
}