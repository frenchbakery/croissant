/**
 * @file sequences.hpp
 * @author melektron
 * @brief sequences and procedures to accomplish tasks
 * @version 0.1
 * @date 2023-03-20
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#pragma once

namespace sq
{
    // == Alignment
    void alignBack();
    void centerOnLineOuter();
    void alignFront();
    void alignOnLineFromRight();
    /**
     * @brief tracks the black line and follows it using the configured navigation 
     * speed until the front distance sensor has reached a threshold
     * 
     * @param pos_threshold threshold of position sensor
     */
    void trackLineUntil(int pos_threshold);
    void centerOnLine();
    void getOfLineToLeft();

    // == Special alignment procedures
    void alignFromDropPosition(double distance);

    // == Setup
    void driveBaseOffset();
    void homeToPomStart();
    void homeToKnock();
    void pomEndToNoodleStart();
    void knockEndToNoodleStart();

    // == Noodles
    void approachRack();
    void pickUpNoodleFromRack();
    void pickUpNoodleFromStand();
    void placeNoodleOnStand();
    void dropNoodle();
    void dropBehind();  // turns 180, drives distance, drops
    void doNoodleTask();

    // == Rocka-Stand
    void knockOverStand();

} // namespace sq
