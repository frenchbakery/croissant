/**
 * @file pomtest.cpp
 * @author test program for pom sorting
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <kipr/util.hpp>

#include "drivers/croissant/pom_sorter/pom_sorter.hpp"



int main()
{
    go::pom_sorter.initialize();

    for (;;)
    {
        go::pom_sorter.setColorSelector(PomSorter::pos_t::green);
        msleep(1000);
        go::pom_sorter.setColorSelector(PomSorter::pos_t::red);
        msleep(1000);
    }

    go::pom_sorter.terminate();
}