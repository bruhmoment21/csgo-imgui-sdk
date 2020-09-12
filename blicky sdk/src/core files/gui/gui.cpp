#include "gui.hpp"

#include "../config/config_system.hpp"

#include "../../other files/imgui/imgui.h"

#include <ShlObj.h>
#include <filesystem>

namespace gui {
    void init( ) noexcept {

        ImGui::StyleColorsDark( );

        auto& style{ ImGui::GetStyle( ) };
        style.ScrollbarSize = 13.0f;
        style.WindowTitleAlign = { 0.5f, 0.5f };
        style.Colors[ ImGuiCol_WindowBg ].w = 0.8f;
        /* Uncomment this if you wish to disable anti aliasing (you must uncomment the one from hooks.cpp too).
        * style.AntiAliasedLines = false;
        * style.AntiAliasedFill = false;
        */

        auto& io{ ImGui::GetIO( ) };
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        if ( PWSTR path_to_fonts; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &path_to_fonts ) ) ) {
            
            const std::filesystem::path path{ path_to_fonts };
            CoTaskMemFree( path_to_fonts );

            static constexpr ImWchar ranges[ 3 ]{ 0x0020, 0xFFFF, 0 };
            ImFontConfig cfg;
            cfg.OversampleV = 3;

            verdana = io.Fonts->AddFontFromFileTTF( ( path / "verdana.ttf" ).string( ).c_str( ), 16.0f, &cfg, ranges );
            initialized = true;
        }
    }

    void render( ) noexcept {

        ImGui::Begin( "blicky sdk", nullptr, ImGuiWindowFlags_AlwaysAutoResize );

        ImGui::Checkbox( "Thirdperson", &config::thirdperson );
        if ( ImGui::IsItemHovered( ) )
            ImGui::SetTooltip( "Automatically bound on Middle Mouse Button." );

        ImGui::Checkbox( "Auto Accept", &config::auto_accept );
        if ( ImGui::IsItemHovered( ) )
            ImGui::SetTooltip( "hinnie's method of accepting match (It clicks the button)." );
        
        ImGui::End( );
    }
}