#ifndef IKEYBOARD_H
#define IKEYBOARD_H

namespace Invision
{
	namespace InputLib
	{
		enum Type
		{
			INPUTLIB_WINDOWS,
			INPUTLIB_LINUX,
			INPUTLIB_UNKNOWN
		};
	}

	class IKeyboard
	{
	public:
		INVISION_API IKeyboard(InputLib::Type inputType);

		INVISION_API virtual short int GetStateOfKey(int nVirtKey) = 0;


		INVISION_API ~IKeyboard();
	protected:
		InputLib::Type Type_ = InputLib::INPUTLIB_UNKNOWN;

	};
}
#endif