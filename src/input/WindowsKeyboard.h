#ifndef WINDOWS_KEYBOARD_H
#define WINDOWS_KEYBOARD_H

#include "IKeyboard.h"

namespace Invision
{

#ifdef WIN_32
	class WindowsKeyboard : public IKeyboard
	{
	public:
		INVISION_API WindowsKeyboard();

		INVISION_API short int GetStateOfKey(int nVirtKey) override;

		INVISION_API ~WindowsKeyboard();
	private:

	};
#endif
}
#endif // WINDOWS_KEYBOARD_H