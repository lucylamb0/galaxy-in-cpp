// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_INCLUDES_H
#define C_VERSION_INCLUDES_H

#include <cmath>
#include <vector>
#include <iostream>

//const long double PI = 3.14159265358979323846;
const long double gravitationalConstant = 6.67408 * pow(10, -11);
const long double gravitationalConstantParsec = 4.30091 * pow(10, -3);
//const long double starMass = 1.98855 * pow(10, 30);
//const long double starRadius = 6.957 * pow(10, 8);
//const long double starDistance = 1.496 * pow(10, 11);
//const long double starTemperature = 5778;
//const long double starLuminosity = 3.828 * pow(10, 26);
//const long double starDiameter = 6.957 * pow(10, 8);
//const long double starSurfaceGravity = 0.006674;
//const long double starSurfacePressure = 0.000002;
//const long double starSurfaceTemperature = 5778;
//const long double starSurfaceLuminosity = 3.828 * pow(10, 26);
//const long double starSurfaceRadius = 6.957 * pow(10, 8);
//const long double starSurfaceDiameter = 6.957 * pow(10, 8);
//const long double starSurfaceVolume = 4.18879 * pow(10, 18);
//const long double starSurfaceArea = 5.670 * pow(10, -8);
//const long double starSurfaceMass = 1.98855 * pow(10, 30);
//const long double parsec = 3.08567758 * pow(10, 16);
const long double parsec_to_km = 3.08567758 * pow(10, 13);
//const long double parsec_per_year = 9.785 * pow(10, 8);
const long double yearInSeconds = 31536000;
const long double gravitationalConstantFinal = gravitationalConstantParsec * pow(yearInSeconds, 2); // this very important constant is used in the calculations. It has units of parsec solarmasses^-1 (km/year)^2
const long double time_step = 0.0027397260274 * 0.5;// this is the time step used in the calculations. It has units of years
const int accelCycles = 365 * 4; // this is the number of acceleration cycles that are run before the velocity and position are updated
//const long double chandrasekharLimit = 1.4 * starMass;

#endif //C_VERSION_INCLUDES_H
