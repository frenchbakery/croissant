/**
 * @file pomtest.cpp
 * @author test program for pom sorting
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#include <kipr/util.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

CRNav __crnav;
namespace go
{
    Navigation &nav = __crnav;
};

int main()
{
    go::pom_container.initialize();
    go::nav.initialize();
    go::nav.setMotorSpeed(500);
    
    /*go::pom_container.setColorSelector(PomContainer::pos_t::red);

    go::nav.driveDistance(35);
    go::nav.awaitTargetReached();
    go::nav.rotateBy(-M_PI_2);
    go::nav.awaitTargetReached();*/
    
    for (int i = 0; i < 5; i++)
    {
        go::pom_container.open();
        go::nav.driveDistance(20);
        go::nav.awaitTargetReached();
        go::pom_container.close();
        go::nav.driveDistance(20);
        go::nav.awaitTargetReached();
    }

    go::pom_container.terminate();
    go::nav.terminate();
}