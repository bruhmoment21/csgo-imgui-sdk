#pragma once

struct ImFont;

namespace gui {
    void init( ) noexcept;
    void render( ) noexcept;

    inline bool initialized{ false };
    inline bool is_open{ false };
    inline ImFont* verdana{ nullptr };
}