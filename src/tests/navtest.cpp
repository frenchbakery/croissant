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
#include <kipr/util.hpp>

#include "drivers/navigation/croissant/crnav.hpp"

CRNav __crnav;
namespace go
{
    Navigation &nav = __crnav;
};

int main()
{
    go::nav.initialize();

    for (int i = 0; i < 4; i++)
    {
        go::nav.driveDistance(20);
        go::nav.awaitTargetReached();
        go::nav.rotateBy(M_PI_2);
        go::nav.awaitTargetReached();
    }


    go::nav.terminate();
    return 0;
}