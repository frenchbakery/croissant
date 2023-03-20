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

void sq::alignLine()
{
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

void sq::alignFront()
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    for (;;)
    {
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

void sq::trackLineUntil(int pos_threshold)
{
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

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

    while (line_left.value() < LIGHT_THRESHOLD)
    {
        go::nav->driveLeftSpeed(200);
    }
    go::nav->driveLeftSpeed(0);

    while (line_right.value() < LIGHT_THRESHOLD)
    {
        go::nav->driveLeftSpeed(-200);
    }
    go::nav->driveLeftSpeed(0);

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();

    go::nav->rotateBy(D2R(-8));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}


void sq::sortOnePom(double offset)
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
}

void sq::driveBaseOffset()
{
    go::nav->driveDistance(5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    msleep(1000);
}

void sq::sortPoms()
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
    sq::alignBack();
    go::pom->open();
    go::nav->driveDistance(35);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom->close();
    
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
            sq::sortOnePom(-15);
        else
            sq::sortOnePom();
    }
    
    //Turns 180 to drive in other direction
    go::nav->driveDistance(-10);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(180));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    sq::alignBack();
    go::nav->driveDistance(20);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}


void sq::approachRack()
{
    sq::trackLineUntil(2400);
    std::cout << "now center" << std::endl;
    sq::centerOnLine();
    std::cout << "now center done" << std::endl;
}

void sq::pickUpNoodle()
{
    go::arm->tilt(0);
    //go::nav->set
    go::arm->setYPerc(80);
    go::arm->waitForY();
    go::arm->waitForTilt();
    
    go::nav->driveDistance(10);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    go::arm->setYPerc(70);
    go::arm->waitForY();

    go::arm->grab(70);
    go::arm->waitForGrab();
    go::nav->driveDistance(-8);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::arm->grab(10);
    go::arm->waitForGrab();
    go::arm->setYPerc(90);
    go::arm->waitForY();
    go::nav->driveDistance(8);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::arm->setYPerc(70);
    go::arm->waitForY();
    go::arm->grab(70);
    go::arm->waitForGrab();

    go::nav->driveDistance(-30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::placeNoodle()
{
    go::arm->setYPerc(90);
    sq::alignFront();
    go::arm->grab(30);
    go::arm->setYPerc(10);
    go::arm->waitForGrab();
    go::arm->waitForY();

    go::arm->grab(0);
    go::arm->waitForGrab();
    go::arm->grab(40);
    go::arm->waitForGrab();
    go::arm->grab(0);
    go::arm->waitForGrab();

    msleep(1000);

    go::nav->driveDistance(-10);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}

void sq::doNoodleTask()
{
    go::nav->rotateBy(D2R(190));
    go::nav->startSequence();
    std::thread clthread(&Arm::calibrateY, go::arm);
    go::nav->awaitSequenceComplete();
    if (clthread.joinable()) clthread.join();

    sq::approachRack();
    sq::pickUpNoodle();

    go::arm->tilt(90);
    go::nav->rotateBy(D2R(-190));
    go::nav->startSequence();
    go::arm->waitForTilt();
    go::nav->awaitSequenceComplete();

    sq::trackLineUntil(2800);

    sq::placeNoodle();

    go::nav->driveDistance(-15);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
}