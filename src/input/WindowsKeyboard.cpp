#include "precompiled.h"

#include "WindowsKeyboard.h"

#ifdef WIN_32
	#include <WinUser.h>
#endif 

namespace Invision
{
#ifdef WIN_32
	
	WindowsKeyboard::WindowsKeyboard() :
		IKeyboard(InputLib::INPUTLIB_WINDOWS)
	{
		

		
	}




	bool WindowsKeyboard::GetStateOfKey(Invision::Key key, Invision::KeyState keystate)
	{
		bool isPressed = false;

		if ((GetKeyState(TranslateKey(key)) & TranslateKeyState(keystate)))
		{
			isPressed = true;
		}
		return isPressed;
	}

	SHORT WindowsKeyboard::TranslateKey(Invision::Key k)
	{
		SHORT winKey;
		
		switch (k)
		{
		case INVISION_KEY_LBUTTON:
			winKey = VK_LBUTTON;
			break;
		case INVISION_KEY_RBUTTON:
			winKey = VK_RBUTTON;
			break;
		case INVISION_KEY_MBUTTON:
			winKey = VK_MBUTTON;
			break;
		case INVISION_KEY_XBUTTON1:
			winKey = VK_XBUTTON1;
			break;
		case INVISION_KEY_XBUTTON2:
			winKey = VK_XBUTTON2;
			break;
		case INVISION_KEY_BACK:
			winKey = VK_BACK;
			break;
		case INVISION_KEY_TAB:
			winKey = VK_TAB;
			break;
		case INVISION_KEY_CLEAR:
			winKey = VK_CLEAR;
			break;
		case INVISION_KEY_RETURN:
			winKey = VK_RETURN;
			break;
		case INVISION_KEY_SHIFT:
			winKey = VK_SHIFT;
			break;
		case INVISION_KEY_CTRL:
			winKey = VK_CONTROL;
			break;
		case INVISION_KEY_ALT:
			winKey = VK_MENU;
			break;
		case INVISION_KEY_PAUSE:
			winKey = VK_PAUSE;
			break;
		case INVISION_KEY_CAPITAL:
			winKey = VK_CAPITAL;
			break;
		case INVISION_KEY_ESCAPE:
			winKey = VK_ESCAPE;
			break;
		case INVISION_KEY_SPACE:
			winKey = VK_SPACE;
			break;
		case INVISION_KEY_PAGEUP:
			winKey = VK_UP;
			break;
		case INVISION_KEY_PAGEDOWN:
			winKey = VK_DOWN;
			break;
		case INVISION_KEY_END:
			winKey = VK_END;
			break;
		case INVISION_KEY_HOME:
			winKey = VK_HOME;
			break;
		case INVISION_KEY_LEFT_ARROW:
			winKey = VK_LEFT;
			break;
		case INVISION_KEY_RIGHT_ARROW:
			winKey = VK_RIGHT;
			break;
		case INVISION_KEY_UP_ARROW:
			winKey = VK_UP;
			break;
		case INVISION_KEY_DOWN_ARROW:
			winKey = VK_DOWN;
			break;
		case INVISION_KEY_EXECUTE:
			winKey = VK_EXECUTE;
			break;
		case INVISION_KEY_PRINT:
			winKey = VK_PRINT;
			break;
		case INVISION_KEY_INSERT:
			winKey = VK_INSERT;
			break;
		case INVISION_KEY_DELETE:
			winKey = VK_DELETE;
			break;
		case INVISION_KEY_NUM_0:
			winKey = 0x30;
			break;
		case INVISION_KEY_NUM_1:
			winKey = 0x31;
			break;
		case INVISION_KEY_NUM_2:
			winKey = 0x32;
			break;
		case INVISION_KEY_NUM_3:
			winKey = 0x33;
			break;
		case INVISION_KEY_NUM_4:
			winKey = 0x34;
			break;
		case INVISION_KEY_NUM_5:
			winKey = 0x35;
			break;
		case INVISION_KEY_NUM_6:
			winKey = 0x36;
			break;
		case INVISION_KEY_NUM_7:
			winKey = 0x37;
			break;
		case INVISION_KEY_NUM_8:
			winKey = 0x38;
			break;
		case INVISION_KEY_NUM_9:
			winKey = 0x39;
			break;
		case INVISION_KEY_A:
			winKey = 0x41;
			break;
		case INVISION_KEY_B:
			winKey = 0x42;
			break;
		case INVISION_KEY_C:
			winKey = 0x43;
			break;
		case INVISION_KEY_D:
			winKey = 0x44;
			break;
		case INVISION_KEY_E:
			winKey = 0x45;
			break;
		case INVISION_KEY_F:
			winKey = 0x46;
			break;
		case INVISION_KEY_G:
			winKey = 0x47;
			break;
		case INVISION_KEY_H:
			winKey = 0x48;
			break;
		case INVISION_KEY_I:
			winKey = 0x49;
			break;
		case INVISION_KEY_J:
			winKey = 0x4A;
			break;
		case INVISION_KEY_K:
			winKey = 0x4B;
			break;
		case INVISION_KEY_L:
			winKey = 0x4C;
			break;
		case INVISION_KEY_M:
			winKey = 0x4D;
			break;
		case INVISION_KEY_N:
			winKey = 0x4E;
			break;
		case INVISION_KEY_O:
			winKey = 0x4F;
			break;
		case INVISION_KEY_P:
			winKey = 0x50;
			break;
		case INVISION_KEY_Q:
			winKey = 0x51;
			break;
		case INVISION_KEY_R:
			winKey = 0x52;
			break;
		case INVISION_KEY_S:
			winKey = 0x53;
			break;
		case INVISION_KEY_T:
			winKey = 0x54;
			break;
		case INVISION_KEY_U:
			winKey = 0x55;
			break;
		case INVISION_KEY_V:
			winKey = 0x56;
			break;
		case INVISION_KEY_W:
			winKey = 0x57;
			break;
		case INVISION_KEY_X:
			winKey = 0x58;
			break;
		case INVISION_KEY_Y:
			winKey = 0x59;
			break;
		case INVISION_KEY_Z:
			winKey = 0x5A;
			break;
		case INVISION_KEY_NUMPAD0:
			winKey = VK_NUMPAD0;
			break;
		case INVISION_KEY_NUMPAD1:
			winKey = VK_NUMPAD1;
			break;
		case INVISION_KEY_NUMPAD2:
			winKey = VK_NUMPAD2;
			break;
		case INVISION_KEY_NUMPAD3:
			winKey = VK_NUMPAD3;
			break;
		case INVISION_KEY_NUMPAD4:
			winKey = VK_NUMPAD4;
			break;
		case INVISION_KEY_NUMPAD5:
			winKey = VK_NUMPAD5;
			break;
		case INVISION_KEY_NUMPAD6:
			winKey = VK_NUMPAD6;
			break;
		case INVISION_KEY_NUMPAD7:
			winKey = VK_NUMPAD7;
			break;
		case INVISION_KEY_NUMPAD8:
			winKey = VK_NUMPAD8;
			break;
		case INVISION_KEY_NUMPAD9:
			winKey = VK_NUMPAD9;
			break;
		case INVISION_KEY_MULTIPLY:
			winKey = VK_MULTIPLY;
			break;
		case INVISION_KEY_ADD:
			winKey = VK_ADD;
			break;
		case INVISION_KEY_SEPARATOR:
			winKey = VK_SEPARATOR;
			break;
		case INVISION_KEY_SUBTRACT:
			winKey = VK_SUBTRACT;
			break;
		case INVISION_KEY_DECIMAL:
			winKey = VK_DECIMAL;
			break;
		case INVISION_KEY_DIVIDE:
			winKey = VK_DIVIDE;
			break;
		case INVISION_KEY_F1:
			winKey = VK_F1;
			break;
		case INVISION_KEY_F2:
			winKey = VK_F2;
			break;
		case INVISION_KEY_F3:
			winKey = VK_F3;
			break;
		case INVISION_KEY_F4:
			winKey = VK_F4;
			break;
		case INVISION_KEY_F5:
			winKey = VK_F5;
			break;
		case INVISION_KEY_F6:
			winKey = VK_F6;
			break;
		case INVISION_KEY_F7:
			winKey = VK_F7;
			break;
		case INVISION_KEY_F8:
			winKey = VK_F8;
			break;
		case INVISION_KEY_F9:
			winKey = VK_F9;
			break;
		case INVISION_KEY_F10:
			winKey = VK_F10;
			break;
		case INVISION_KEY_F11:
			winKey = VK_F11;
			break;
		case INVISION_KEY_F12:
			winKey = VK_F12;
			break;
		case INVISION_KEY_NUMLOCK:
			winKey = VK_NUMLOCK;
			break;
		case INVISION_KEY_SCROLL:
			winKey = VK_SCROLL;
			break;
		default:
			winKey = 0;
			// Key is not implemented
		}

		return winKey;
	}

	unsigned int WindowsKeyboard::TranslateKeyState(Invision::KeyState ks)
	{
		unsigned int state;
		if (ks == INVISION_KEY_PRESSED)
		{
			state = 0x8000; // Button Pressed
		}
		else // INVISION_KEY_RELEASED
		{
			state = 0x0001; // Button Released
		}

		return state;
	}

	WindowsKeyboard::~WindowsKeyboard()
	{
	
	}

#endif

}