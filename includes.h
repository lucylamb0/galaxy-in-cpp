// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_INCLUDES_H
#define C_VERSION_INCLUDES_H

#ifdef __WINDOWS__
#define WINDOWS_LINUX
#elifdef __LINUX__
#define WINDOWS_LINUX
#else
#define __MAC_OS__
#endif

#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>

#define degrees_to_radians(degrees) ((float)(degrees * PI / 180.0))
#define radians_to_degrees(radians) ((float)(radians * 180.0 / PI))

const long double PI = 3.14159265358979323846;
const long double gravitationalConstant = 6.67408 * pow(10, -11);
const long double gravitationalConstantParsec = 4.30091 * pow(10, -3);
//const long double parsec = 3.08567758 * pow(10, 16);
const long double parsec_to_km = 3.08567758 * pow(10, 13);
const long double parsec_per_year = 9.785 * pow(10, 8);
const long double yearInSeconds = 31536000;
const long double gravitationalConstantFinal = gravitationalConstantParsec * pow(yearInSeconds, 2); // this very important constant is used in the calculations. It has units of parsec solarmasses^-1 (km/year)^2
const long double solarMass = 1.98847 * pow(10, 30);
const long double parsecsPerYear_to_metersPerSecond = parsec_to_km * 1000 / yearInSeconds;

// Time frame the simulation will simulate
static long double timeScale = 365.f/365.f; // In Years
// Amount of simulation frames to have
static long double simulationFrames = 365.f * 2; // 1 year per cycle

// Time to pass per acceleration cycle
static long double time_step = timeScale / simulationFrames;// this is the time step used in the calculations. It has units of years
//const long double time_step = 0.0027397260274 * 0.5;// this is the time step used in the calculations. It has units of years

#endif //C_VERSION_INCLUDES_H
