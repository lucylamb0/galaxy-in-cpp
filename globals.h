//
// Created by User on 04/08/2022.
//

#ifndef C_VERSION_GLOBALS_H
#define C_VERSION_GLOBALS_H

#include <optional>

struct Globals {
    bool mouse_captured = true;
    float frame_time = -1;

    GLFWwindow* window;
    struct window_size {
        int width = 1824;
        int height = 768;
    } window_size;

    struct camera_angles {
        float yaw;
        float pitch;
    } camera_angles;

    struct c_movement {
        float movement_speed = 1.f; // 1 unit per second
    } movement;
};;

inline Globals* globals = new Globals();
//inline std::optional<Globals> globals;

#endif //C_VERSION_GLOBALS_H
