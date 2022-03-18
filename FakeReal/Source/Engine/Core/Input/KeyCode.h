#pragma once
#include <stdint.h>
namespace FakeReal {
	using KeyCode = uint16_t;
		enum Key: KeyCode	//Board Key
		{
			//¼üÅÌ¼üÉ¨ÃèÂë
			KC_ESC,

			KC_F1,
			KC_F2,
			KC_F3,
			KC_F4,
			KC_F5,
			KC_F6,
			KC_F7,
			KC_F8,
			KC_F9,
			KC_F10,
			KC_F11,
			KC_F12,

			KC_GRAVE, // '~ '
			KC_1,
			KC_2,
			KC_3,
			KC_4,
			KC_5,
			KC_6,
			KC_7,
			KC_8,
			KC_9,
			KC_0,
			KC_MINUS, // '-'
			KC_EQUALS, // '='
			KC_BACKSPACE,

			KC_TAB,
			KC_Q,
			KC_W,
			KC_E,
			KC_R,
			KC_T,
			KC_Y,
			KC_U,
			KC_I,
			KC_O,
			KC_P,
			KC_LBRACKET, // '[ '
			KC_RBRACKET, //'] '
			KC_ENTER, // '\ '

			KC_CAPSLOCK,
			KC_A,
			KC_S,
			KC_D,
			KC_F,
			KC_G,
			KC_H,
			KC_J,
			KC_K,
			KC_L,
			KC_SEMICOLON, //'; '
			KC_APOSTROPHE, // ' ' '
			KC_KC_BACKSLASH,

			KC_LSHIFT,
			KC_Z,
			KC_X,
			KC_C,
			KC_V,
			KC_B,
			KC_N,
			KC_M,
			KC_COMMA, // ', '
			KC_PERIOD, // '. '
			KC_SLASH, // '/ '
			KC_RSHIFT,

			KC_LCTRL,
			KC_LWIN,
			KC_LALT,
			KC_SPACE,
			KC_RALT,
			KC_RWIN,
			KC_RCTRL,

			KC_PRINT_SCREEN,
			KC_SCROLL_LOCK,
			//KC_PB,

			KC_INSERT,
			KC_DELETE,
			KC_HOME,
			KC_END,
			KC_PAGE_UP,
			KC_PAGE_DOWN,

			KC_UP,
			KC_DOWN,
			KC_LEFT,
			KC_RIGHT,

			KC_NUM_LOCK,
			KC_NP_DIVIDE,
			KC_NP_MULTIPLY,
			KC_NP_SUBTRACT,
			KC_NP_ADD,
			KC_NP_ENTER,
			KC_NP_DECIMAL, //'. '
			KC_NP_0,
			KC_NP_1,
			KC_NP_2,
			KC_NP_3,
			KC_NP_4,
			KC_NP_5,
			KC_NP_6,
			KC_NP_7,
			KC_NP_8,
			KC_NP_9,

			KC_MAX

		};
}