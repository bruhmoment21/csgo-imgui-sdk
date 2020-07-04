#pragma once

enum class analog_code_t {
	analog_code_invalid = -1,
	mouse_x = 0,
	mouse_y,
	mouse_xy, // invoked when either x or y changes
	mouse_wheel,
	analog_code_last = 10,
};

enum class button_code_t {
	button_code_invalid = -1,
	button_code_none = 0,

	key_first = 0,

	key_none = key_first,
	key_0,
	key_1,
	key_2,
	key_3,
	key_4,
	key_5,
	key_6,
	key_7,
	key_8,
	key_9,
	key_a,
	key_b,
	key_c,
	key_d,
	key_e,
	key_f,
	key_g,
	key_h,
	key_i,
	key_j,
	key_k,
	key_l,
	key_m,
	key_n,
	key_o,
	key_p,
	key_q,
	key_r,
	key_s,
	key_t,
	key_u,
	key_v,
	key_w,
	key_x,
	key_y,
	key_z,
	key_pad_0,
	key_pad_1,
	key_pad_2,
	key_pad_3,
	key_pad_4,
	key_pad_5,
	key_pad_6,
	key_pad_7,
	key_pad_8,
	key_pad_9,
	key_pad_divide,
	key_pad_multiply,
	key_pad_minus,
	key_pad_plus,
	key_pad_enter,
	key_pad_decimal,
	key_lbracket,
	key_rbracket,
	key_semicolon,
	key_apostrophe,
	key_backquote,
	key_comma,
	key_period,
	key_slash,
	key_backslash,
	key_minus,
	key_equal,
	key_enter,
	key_space,
	key_backspace,
	key_tab,
	key_capslock,
	key_numlock,
	key_escape,
	key_scrolllock,
	key_insert,
	key_delete,
	key_home,
	key_end,
	key_pageup,
	key_pagedown,
	key_break,
	key_lshift,
	key_rshift,
	key_lalt,
	key_ralt,
	key_lcontrol,
	key_rcontrol,
	key_lwin,
	key_rwin,
	key_app,
	key_up,
	key_left,
	key_down,
	key_right,
	key_f1,
	key_f2,
	key_f3,
	key_f4,
	key_f5,
	key_f6,
	key_f7,
	key_f8,
	key_f9,
	key_f10,
	key_f11,
	key_f12,
	key_capslocktoggle,
	key_numlocktoggle,
	key_scrolllocktoggle,

	key_last = key_scrolllocktoggle,
	key_count = key_last - key_first + 1,

	// mouse
	mouse_first = key_last + 1,

	mouse_left = mouse_first,
	mouse_right,
	mouse_middle,
	mouse_4,
	mouse_5,
	mouse_wheel_up,				// a fake button which is 'pressed' and 'released' when the wheel is moved up
	mouse_wheel_down,			// a fake button which is 'pressed' and 'released' when the wheel is moved down

	mouse_last = mouse_wheel_down,
	mouse_count = mouse_last - mouse_first + 1,
};

class i_input_system {
public:
	void enable_input( const bool enable ) {
		using original_fn = void( __thiscall* )( void*, bool );
		return ( *reinterpret_cast< original_fn** >( this ) )[ 11 ]( this, enable );
	}

	bool is_button_down( const button_code_t code ) {
		using original_fn = bool( __thiscall* )( void*, button_code_t );
		return ( *reinterpret_cast< original_fn** >( this ) )[ 15 ]( this, code );
	}

	void reset_input_state( ) {
		using original_fn = void( __thiscall* )( void* );
		return ( *reinterpret_cast<original_fn**>(this) )[ 39 ]( this );
	}
};