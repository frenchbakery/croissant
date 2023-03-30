/**
 * @file sequences.cpp
 * @author melektron
 * @brief sequences and procedures to accomplish tasks
 * @version 0.1
 * @date 2023-03-20
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#include <iostream>
#include <kipr/analog/analog.hpp>
#include <kipr/digital/digital.hpp>
#include <kipr/time/time.h>
#include "sequences.hpp"
#include "global_objects.hpp"
#include "defs.hpp"
#include "utils.hpp"


// some sensors
kipr::digital::Digital back_button_left(0);
kipr::digital::Digital back_button_right(1);
kipr::digital::Digital front_button_left(3);
kipr::digital::Digital front_button_right(4);
kipr::analog::Analog line_left(1);
kipr::analog::Analog line_right(0);
kipr::analog::Analog line_center(4);
kipr::analog::Analog front_distance(3);



void sq::alignBack()
{
    // Find zero point in X direction and align robot to game table
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

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

void sq::centerOnLineOuter()
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(-CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    enum state_t
    {
        START,
        LEFT_DONE
    } state = START;

    int time_start = seconds();
    const int timeout = 3;  // s

    for (;;)
    {
        // flag is true when on black (= when value is big)
        bool right = line_right.value() > LIGHT_THRESHOLD;
        bool left = line_left.value() > LIGHT_THRESHOLD;
        
        if (right && state == START)
        {
            // rotate to right
            go::nav->driveLeftSpeed(CALIB_SPEED);
            go::nav->driveRightSpeed(-CALIB_SPEED);
            state = LEFT_DONE;
        }
        else if (left && state == LEFT_DONE)
            break;

        // don't waste to much time on that
        if (seconds() - time_start >= timeout)
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();

    go::nav->rotateBy(D2R(10));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::alignFront()
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    int start = seconds();
    const int timeouts = 5;

    for (;;)
    {
        // a timeout in case the front switches miss
        if (seconds() - start > timeouts) break;

        if (front_button_left.value())
            go::nav->driveLeftSpeed(0);
        else 
            go::nav->driveLeftSpeed(CALIB_SPEED);
        
        if (front_button_right.value())
            go::nav->driveRightSpeed(0);
        else 
            go::nav->driveRightSpeed(CALIB_SPEED);

        if (front_button_left.value() && front_button_right.value())
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
}

void sq::alignOnLineFromRight()
{
    const int DRIVE_SPEED = 500;
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(DRIVE_SPEED);
    go::nav->driveRightSpeed(-DRIVE_SPEED);

    enum dir_t
    {
        RIGHT,
        LEFT
    } dir;

    for (;;)
    {
        // flag is true when on black (= when value is big)
        bool right = line_right.value() > LIGHT_THRESHOLD;
        bool left = line_left.value() > LIGHT_THRESHOLD;
        bool center = line_center.value() > LIGHT_THRESHOLD; 
        
        // if nothing is on
        if (!right && !left && !center)
            dir = RIGHT;
        else if (right && !left && !center)
            dir = RIGHT;
        else if (left && center)
            break;
        else
            dir = RIGHT;
        

        if (dir == RIGHT)
        {
            go::nav->driveLeftSpeed(DRIVE_SPEED);
            go::nav->driveRightSpeed(-DRIVE_SPEED);
        }
        else
        {
            go::nav->driveLeftSpeed(DRIVE_SPEED);
            go::nav->driveRightSpeed(-DRIVE_SPEED);
        }
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);

    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
}

void sq::trackLineUntil(int pos_threshold)
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    //int pos_threshold = 

    while (front_distance.value() < pos_threshold)
    {
        // true = white, false = dark
        bool trl = line_left.value() > LIGHT_THRESHOLD;
        bool trr = line_right.value() > LIGHT_THRESHOLD;
        
        // both white or both black
        if (trl == trr)
        {
            go::nav->driveLeftSpeed(NAV_SPEED);
            go::nav->driveRightSpeed(NAV_SPEED);
        }
        else if (trl == true && trr == false)
        {
            go::nav->driveLeftSpeed(0);
            go::nav->driveRightSpeed(NAV_SPEED);
        }
        else if (trl == false && trr == true)
        {
            go::nav->driveLeftSpeed(NAV_SPEED);
            go::nav->driveRightSpeed(0);
        }
    }


    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
}

void sq::centerOnLine()
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);

    /*while (line_center.value() > LIGHT_THRESHOLD)
    {
        go::nav->driveLeftSpeed(300);
    }
    go::nav->driveLeftSpeed(0);*/

    int start_time = seconds();
    int timeout = 3; // s

    while (line_center.value() > LIGHT_THRESHOLD)
    {
        go::nav->driveLeftSpeed(-300);
        if (seconds() - start_time > timeout)
            break;
    }
    go::nav->driveLeftSpeed(0);

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();

    go::nav->rotateBy(D2R(-12));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

}


void sq::alignFromDropPosition(double distance)
{
    go::nav->driveDistance(-distance);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    sq::alignBack();
}

void sq::driveBaseOffset()
{
    go::nav->driveDistance(5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::homeToPomStart()
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
}

void sq::homeToKnock()
{
    go::arm->calibrateY();
    go::nav->driveDistance(7.7);
    go::nav->rotateBy(D2R(92));
    
    go::nav->startSequence();

    go::arm->waitForCalibrate();
    go::nav->awaitSequenceComplete();
}

void sq::pomEndToNoodleStart()
{
    go::nav->driveDistance(113);
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::knockEndToNoodleStart()
{
    go::nav->driveDistance(30);
    go::nav->rotateBy(D2R(-90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::approachRack()
{
    sq::trackLineUntil(2600);
    std::cout << "now center" << std::endl;
    sq::centerOnLine();
    std::cout << "now center done" << std::endl;
}

void sq::pickUpNoodleFromRack()
{
    // in case a grab is still active wait
    go::arm->waitForGrab();
    go::arm->tilt(0);
    //go::nav->set
    go::arm->setYPerc(80);
    go::arm->waitForY();
    go::arm->waitForTilt();
    go::arm->grab(-30);
    msleep(300);
    go::nav->driveDistance(9);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    go::arm->setYPerc(80);
    //go::arm->waitForY();
    msleep(200);

    go::arm->grab(80);
    //go::arm->waitForGrab();
    msleep(800);
    go::nav->driveDistance(-8);
    go::nav->startSequence();
    //go::nav->awaitSequenceComplete();
    msleep(800);
    go::arm->grab(0);
    //go::arm->waitForGrab();
    msleep(400);
    go::arm->setYPerc(90);
    //go::arm->waitForY();
    msleep(400);
    go::nav->driveDistance(8);
    go::nav->startSequence();
    //go::nav->awaitSequenceComplete();
    msleep(1000);
    go::arm->setYPerc(80);
    //go::arm->waitForY();
    msleep(200);
    go::arm->grab(80);
    // wait at least 10 seconds for other robot to pass by
    msleep(2000);
    go::arm->waitForGrab();

    go::nav->driveDistance(-40);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::pickUpNoodleFromStand()
{
    go::arm->grab(20);
    go::arm->setYPerc(0);
    go::arm->waitForGrab();
    sq::alignFront();
    go::arm->waitForY();
    go::arm->grab(70);
    go::arm->waitForGrab();
    go::arm->setYPerc(90);
    go::arm->waitForY();
}

void sq::placeNoodleOnStand()
{
    go::arm->setYPerc(90);
    sq::alignFront();
    go::arm->grab(45);
    go::arm->setYPerc(10);
    go::nav->driveDistance(-1);
    go::nav->driveDistance(1);
    go::arm->waitForGrab();
    go::arm->waitForY();
    go::nav->startSequence();
    while (!go::nav->sequenceComplete())
    {
        go::arm->grab(25);
        go::arm->waitForGrab();
        go::arm->grab(50);
        go::arm->waitForGrab();
    }

    go::arm->grab(0);
    go::arm->waitForGrab();

}

void sq::dropNoodle()
{
    go::arm->tilt(45);
    go::arm->waitForTilt();
    go::arm->grab(0);
    go::arm->waitForGrab();
}

void sq::dropBehind()
{
    go::nav->rotateBy(D2R(180));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    sq::dropNoodle();

}


void sq::doNoodleTask()
{
    // get red noodle and place on rack
    go::arm->grab(0);
    sq::approachRack();
    sq::pickUpNoodleFromRack();
    
    go::arm->tilt(90);
    go::nav->rotateBy(D2R(-135));   // drive less than 180 degrees in order to then realign
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    // now fix the angle to be on the line again in case the robot wasn straight before
    sq::alignOnLineFromRight();

    // wait until the tilt is done
    go::arm->waitForTilt();

    sq::trackLineUntil(2800);

    sq::centerOnLineOuter();

    sq::placeNoodleOnStand();

    go::nav->driveDistance(-15);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    // drive to second to right most noodle
    go::nav->rotateBy(D2R(90));
    go::nav->driveDistance(21);
    go::nav->rotateBy(D2R(-90));
    go::nav->driveDistance(10);
    go::arm->grab(20);
    go::arm->waitForGrab();
    go::arm->tilt(90);
    go::arm->calibrateY();
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::arm->waitForTilt();
    go::arm->waitForCalibrate();

    // pick it up
    sq::pickUpNoodleFromStand();

    // drop behind and realign
    go::nav->driveDistance(-15);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    sq::dropBehind();
    // while aligning, reset the positions to what is needed for the next operation
    go::arm->grab(10);
    go::arm->waitForGrab();
    go::arm->tilt(90);
    go::arm->calibrateY();
    sq::alignFromDropPosition(0);
    go::arm->waitForTilt();
    go::arm->waitForCalibrate();


    // drive to the right most noodle
    go::nav->driveDistance(25);
    go::nav->rotateBy(D2R(-90));
    go::nav->driveDistance(11);
    go::nav->rotateBy(D2R(-90));
    go::arm->grab(20);
    // go::nav->driveDistance(5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    // pick it up
    sq::pickUpNoodleFromStand();

    // drop behind and realign
    go::nav->driveDistance(-15);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    sq::dropBehind();
    go::arm->tilt(90);
    go::arm->waitForTilt();
    // while aligning, reset the positions to what is needed for the next operation
    // go::arm->grab(10);
    // go::arm->waitForGrab();
    // go::arm->tilt(90);
    // go::arm->setYPerc(0);
    // go::nav->driveDistance(-15);
    // go::nav->startSequence();
    // go::nav->awaitSequenceComplete();
    // sq::alignFromDropPosition(0);
    // go::arm->waitForTilt();
    // go::arm->waitForY();
}

void sq::knockOverStand()
{
    go::arm->setYPerc(50);
    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::kno->place();
    msleep(500);
    go::kno->up();
    msleep(300);
    go::kno->hold();

    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    go::arm->setYPerc(10);
    go::arm->waitForY();
    go::nav->driveDistance(-70);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::kno->down();
    go::kno->retract();

}
