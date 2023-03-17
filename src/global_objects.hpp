/**
 * @file global_objects.hpp
 * @author melektron
 * @brief global object instances
 * @version 0.1
 * @date 2023-03-17
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#pragma once

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/arm/arm.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

// global object pointer declaration
namespace go
{
    extern CRNav *nav;
    extern Arm *arm;
    extern PomContainer *pom;
} // namespace go
