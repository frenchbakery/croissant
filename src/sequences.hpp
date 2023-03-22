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
    void alignLine();
    void alignFront();
    void alignRight();
    /**
     * @brief tracks the black line and follows it using the configured navigation 
     * speed until the front distance sensor has reached a threshold
     * 
     * @param pos_threshold threshold of position sensor
     */
    void trackLineUntil(int pos_threshold);
    void centerOnLine();

    // == Setup
    void driveBaseOffset();
    void homeToPomStart();
    void homeToKnock();
    void pomEndToNoodleStart();
    void knockEndToNoodleStart();

    // == Pom sorting
    void sortOnePom(double offset1 = 0, double offset2 = 0);
    void sortPoms();

    // == Noodles
    void approachRack();
    void pickUpNoodle();
    void placeNoodle();
    void doNoodleTask();

    // == Rocka-Stand
    void knockOverStand();

} // namespace sq
