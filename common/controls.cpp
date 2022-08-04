// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include "globals.h"
#include <iostream>
//#include "config.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;

// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.055f;

void computeMatricesFromInputs(){
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();


    // Compute time difference between current and last frame
	double currentTime = glfwGetTime();
    globals->frame_time = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	if(globals->mouse_captured)
        glfwSetCursorPos(window,  globals->window_size.width/2, globals->window_size.height/2);

	// Compute new orientation
    globals->camera_angles.yaw += mouseSpeed * float(globals->window_size.width/2 - xpos );
    globals->camera_angles.pitch   += mouseSpeed * float( globals->window_size.height/2 - ypos );
    std::cout << "horizontalAngle: " << globals->camera_angles.yaw << " verticalAngle: " << globals->camera_angles.pitch << std::endl;

    // Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(globals->camera_angles.pitch) * sin(globals->camera_angles.yaw),
		sin(globals->camera_angles.pitch),
		cos(globals->camera_angles.pitch) * cos(globals->camera_angles.yaw)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(globals->camera_angles.yaw - 3.14f/2.0f),
		0,
		cos(globals->camera_angles.yaw - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * globals->frame_time * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * globals->frame_time * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * globals->frame_time * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * globals->frame_time * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(
            glm::radians(FoV),
            4.0f / 3.0f,
            0.1f,
            100.0f);
//	ProjectionMatrix = glm::perspective(glm::radians(FoV), 19.0f / 9.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}