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
#include <kipr/motors.hpp>

#include "drivers/croissant/pom_sorter/pom_sorter.hpp"


Motor motorl(0);
Motor motorr(1);


int main()
{
    go::pom_sorter.initialize();

    for (;;)
    {
        go::pom_sorter.setColorSelector(PomSorter::pos_t::green);
        motorl.forward();
        motorr.forward();
        msleep(1000);
        motorl.freeze();
        motorr.freeze();
        go::pom_sorter.setColorSelector(PomSorter::pos_t::red);
        motorl.forward();
        motorr.forward();
        msleep(1000);
        motorl.freeze();
        motorr.freeze();
    }

    go::pom_sorter.terminate();
}