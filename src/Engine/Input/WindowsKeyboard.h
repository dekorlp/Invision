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

		INVISION_API bool GetStateOfKey(Invision::Key key, Invision::KeyState keystate) override;

		INVISION_API ~WindowsKeyboard();
	private:
		INVISION_API SHORT TranslateKey(Invision::Key k);
		INVISION_API unsigned int TranslateKeyState(Invision::KeyState ks);
	};
#endif
}
#endif // WINDOWS_KEYBOARD_H