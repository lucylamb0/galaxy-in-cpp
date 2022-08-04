//
// Created by User on 04/08/2022.
//

#ifndef C_VERSION_CONFIG_H
#define C_VERSION_CONFIG_H

struct config {
    struct c_controls {
        float movement_speed = 0.1f;
        float mouse_sensitivity_x = 0.1f;
        float mouse_sensitivity_y = 0.1f;

        float camera_yaw_min = -180.0f;
        float camera_yaw_max = 180.0f;

        float camera_pitch_min = -90.0f;
        float camera_pitch_max = 90.0f;

        float camera_fov = 45.0f;


    }*controls;
}*g_config;


#endif //C_VERSION_CONFIG_H
