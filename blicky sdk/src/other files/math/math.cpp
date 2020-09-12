#include "math.hpp"

#include "../../other files/utilities/utilities.hpp"
#include "../../sdk files/sdk.hpp"
#include "helpers/matrices.hpp"

#include "../imgui/imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui_internal.h"

namespace math {
    bool world_to_screen( const vec3_t& origin, ImVec2& screen ) noexcept {

        static auto& matrix{ *reinterpret_cast< matrix_4x4* >( sdk::view_matrix ) };

        const auto w{ matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ] };
        if ( w < 0.001f )
            return false;

        screen = ImGui::GetIO( ).DisplaySize / 2.0f;

        screen.x *= 1.0f + ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) / w;
        screen.y *= 1.0f - ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) / w;

        return true;
    }
}