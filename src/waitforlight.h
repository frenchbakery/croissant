/**
 * @file waitforlight.h
 * @author Nilusink
 * @brief automatically calibrating wait for light code
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#pragma once

#include <cmath>
#include <kipr/time/time.h>
#include <kipr/analog/analog.hpp>


#define LIGHT_CALIB_ACCURACY 100
static int ambient_light = -1;
static int light_range = -1;

static void calibrateLightSensor(kipr::analog::Analog &light_sensor)
{
    int sum = 0;
    int now_val;
    int max_value = 1;
    int min_value = 4086;
    for (int sensor_value_index = 0; sensor_value_index < LIGHT_CALIB_ACCURACY; sensor_value_index++)
    {
        now_val = light_sensor.value();
        sum += now_val;
        max_value = std::max(now_val, max_value);
        min_value = std::min(now_val, min_value);

        msleep(20);
    }
    ambient_light = sum / LIGHT_CALIB_ACCURACY;  // average light value
    light_range = max_value - min_value;
    std::cout << "light_range=" << light_range << std::endl;
}

static void waitForLight(kipr::analog::Analog &light_sensor)
{
    while (light_sensor.value() > ambient_light - (light_range * 2)) msleep(10);
    //while (light_sensor.value() < )
}

