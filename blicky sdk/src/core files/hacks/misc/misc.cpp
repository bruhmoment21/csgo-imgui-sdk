#include "misc.hpp"

#include "../../config/config_system.hpp"
#include "../../../sdk files/sdk.hpp"

namespace misc {

	void thirdperson( ) noexcept {	// I don't know if this thirdperson is detected. I'll delete this comment if it isn't.
									// This could be the best thirdperson method.
		static bool in_third_person{ false };
		static float last_time{ 0.f };

		auto* const third_person_convar{ interfaces::console->get_convar( "thirdperson" ) };
		auto* const first_person_convar{ interfaces::console->get_convar( "firstperson" ) };

		if ( config::thirdperson && GetAsyncKeyState( VK_MBUTTON ) && interfaces::globals->real_time - last_time > 0.25f ) {
			
			in_third_person = !in_third_person;
			last_time = interfaces::globals->real_time;
		}

		if ( in_third_person ) third_person_convar->change_callback( );
		if ( !in_third_person || !config::thirdperson ) first_person_convar->change_callback( );
	}
}