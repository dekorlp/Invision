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


	short int WindowsKeyboard::GetStateOfKey(int nVirtKey)
	{
		// 0x0001 key is up
		// 0x8000 key is down

		if ((GetKeyState(0x53) & 0x8000))
		{
			std::cout << "Taste s wurde gedrückt" << std::endl;
		}
		return 0;
	}

	WindowsKeyboard::~WindowsKeyboard()
	{
	
	}

#endif

}