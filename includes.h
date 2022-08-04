//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_INCLUDES_H
#define C_VERSION_INCLUDES_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Include ImGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Utils.h"
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include "common/objloader.hpp"
#include "Model.h"

#include "globals.h"

const long double PI = 3.14159265358979323846;
const long double gravitationalConstant = 6.67408 * pow(10, -11);
const long double starMass = 1.98855 * pow(10, 30);
const long double starRadius = 6.957 * pow(10, 8);
const long double starDistance = 1.496 * pow(10, 11);
const long double starTemperature = 5778;
const long double starLuminosity = 3.828 * pow(10, 26);
const long double starDiameter = 6.957 * pow(10, 8);
const long double starSurfaceGravity = 0.006674;
const long double starSurfacePressure = 0.000002;
const long double starSurfaceTemperature = 5778;
const long double starSurfaceLuminosity = 3.828 * pow(10, 26);
const long double starSurfaceRadius = 6.957 * pow(10, 8);
const long double starSurfaceDiameter = 6.957 * pow(10, 8);
const long double starSurfaceVolume = 4.18879 * pow(10, 18);
const long double starSurfaceArea = 5.670 * pow(10, -8);
const long double starSurfaceMass = 1.98855 * pow(10, 30);
const long double parsec = 3.08567758 * pow(10, 16);
const long double parsec_per_year = 9.785 * pow(10, 8);

const long double yearInSeconds = 31536000;
const long double time_step = yearInSeconds * 10;

#endif //C_VERSION_INCLUDES_H
