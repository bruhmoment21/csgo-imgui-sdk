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
        /* uncomment this if you wish to disable anti aliasing (you must uncomment the one from hooks.cpp too)
        * style.AntiAliasedLines = false;
        * style.AntiAliasedFill = false;
        */

        auto& io{ ImGui::GetIO( ) };
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        initialized = true;

        if ( PWSTR path_to_fonts; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &path_to_fonts ) ) ) {
            
            const std::filesystem::path path{ path_to_fonts };
            CoTaskMemFree( path_to_fonts );

            static constexpr ImWchar ranges[ ]{ 0x0020, 0xFFFF, 0 };
            ImFontConfig cfg;
            cfg.OversampleV = 3;

            verdana = io.Fonts->AddFontFromFileTTF( ( path / "verdana.ttf" ).string( ).c_str( ), 16.0f, &cfg, ranges );
        }
    }

    void render( ) noexcept {

        ImGui::Begin( "blicky sdk", nullptr, ImGuiWindowFlags_AlwaysAutoResize );

        ImGui::Checkbox( "Thirdperson", &config::thirdperson );
        if ( ImGui::IsItemHovered( ) )
            ImGui::SetTooltip( "Automatically bound on Middle Mouse Button." );

        ImGui::End( );
    }
}