#include "misc.hpp"

#include "../../config/config_system.hpp"
#include "../../../sdk files/sdk.hpp"

#include "../../../other files/imgui/imgui.h"

namespace misc {
	void thirdperson( ) noexcept {

		static bool in_third_person{ false };

		static auto* third_person_convar{ sdk::console->get_convar( "thirdperson" ) };
		static auto* first_person_convar{ sdk::console->get_convar( "firstperson" ) };
		
		if ( config::thirdperson && ImGui::IsMouseClicked( 2 ) ) // A mouse button identifier (0=left, 1=right, 2=middle)
			in_third_person = !in_third_person;

		if ( in_third_person ) third_person_convar->change_callback( );
		if ( !in_third_person || !config::thirdperson ) first_person_convar->change_callback( );
	}
}
