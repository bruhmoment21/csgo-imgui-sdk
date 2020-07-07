#pragma once
#include <memory>

struct ImFont;

class c_gui {
public:
    c_gui( ) noexcept;
    static void render( ) noexcept;

    bool is_open = true;
private:
    ImFont* verdana = nullptr;
};

inline std::unique_ptr<c_gui> gui;
