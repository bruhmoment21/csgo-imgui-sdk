#include <ShlObj.h>
#include <filesystem>

#include "../../sdk files/sdk.hpp"

c_gui::c_gui( ) noexcept {
	
    ImGui::StyleColorsDark( );

    ImGuiStyle& style = ImGui::GetStyle( );
    style.ScrollbarSize = 13.0f;
    style.WindowTitleAlign = { 0.5f, 0.5f };
    style.Colors[ ImGuiCol_WindowBg ].w = 0.8f;
    /* uncomment this if you wish to disable anti aliasing (you must uncomment the one from hooks.cpp too)
    * style.AntiAliasedLines = false;
    * style.AntiAliasedFill = false;
    */
	
    ImGuiIO& io = ImGui::GetIO( );
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;

    if ( PWSTR path_to_fonts; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &path_to_fonts ) ) ) {
        const std::filesystem::path path{ path_to_fonts };
        CoTaskMemFree( path_to_fonts );

        static constexpr ImWchar ranges[ ]{ 0x0020, 0xFFFF, 0 };
        ImFontConfig cfg;
        cfg.OversampleV = 3;

        verdana = io.Fonts->AddFontFromFileTTF( ( path / "verdana.ttf" ).string( ).c_str( ), 15.0f, &cfg, ranges );
    }
}

void c_gui::render( ) const noexcept { // you may make this static in the future
	
    static bool checkbox, gucci_bool;
    static float pfloat{ 0.f };
	
    ImGui::Begin( "blicky sdk");

    ImGui::Text( "Test text." );
    ImGui::Checkbox( "Test checkbox", &checkbox );
    if ( ImGui::IsItemHovered( ) )
        ImGui::SetTooltip( "Very nice" );
	
    ImGui::SliderFloat( "Test float", &pfloat, 0.0f, 1.0f );
    if ( ImGui::Button( "Test Button" ) )
        gucci_bool = !gucci_bool;
    if( gucci_bool )
        ImGui::Text( "Nice." );
	
    ImGui::SameLine( );
    ImGui::Text( "You are a boss." );
	
    ImGui::End( );
}
