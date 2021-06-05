#pragma once

#include "math.hpp"

class c_input
{
    std::byte pad0[0xC];
public:
    bool ir_track_available;
    bool mouse_initialized;
    bool mouse_active;
private:
    std::byte pad1[0x9E];
public:
    bool camera_is_in_thirdperson;
    bool camera_moving_with_mouse;
    vec3_t camera_offset;
};