#include "math.hpp"

#include "../../other files/utilities/utilities.hpp"
#include "helpers/matrices.hpp"

#include "../imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui_internal.h"

#include <Windows.h>

bool math::world_to_screen( const vec3_t& origin, ImVec2& screen ) {

    static std::uintptr_t view_matrix; // engine get_view_matrix will cause flickering if you have multithreading enabled and draw using dx9
    if ( !view_matrix )
        view_matrix = *reinterpret_cast< std::uintptr_t* >( utilities::pattern_scan( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) + 3 ) + 176;

    const auto& matrix = *reinterpret_cast< matrix_4x4* >( view_matrix ); // matrix_3x4 works well too since all matrix.m[3][0, 1, 2] values are all 0 and [3] is 1

    const auto w = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];
    if ( w < 0.001f )
        return false;
	
    screen = ImGui::GetIO( ).DisplaySize / 2.0f;

    screen.x *= 1.0f + ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) / w;
    screen.y *= 1.0f - ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) / w;

    return true;
}
