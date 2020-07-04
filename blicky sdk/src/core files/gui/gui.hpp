#pragma once
#include "../../other files/imgui/imgui.h"

#include <memory>

class c_gui {
public:
    c_gui( ) noexcept;
    void render( ) const noexcept;

    bool is_open = true;
private:
    ImFont* verdana = nullptr;
};

inline std::unique_ptr<c_gui> gui;