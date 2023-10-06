#include "precompiled.h"
#include "WString.h"
namespace Invision {

	std::map<size_t, BaseString<const wChar*, wChar>> WString::mBaseTable;

	WString::WString(const wChar* text)
	{
		if (text == NULL)
		{
			throw InvisionArgumentException("");
		}

		BaseString<const wChar*, wChar> baseString(text, wcslen(text), Hash::getHash(text));
		size_t id = baseString.GetCRCID();

		if (mBaseTable.find(id) == mBaseTable.end())
		{
			mBaseTable[id] = baseString;
		}
		rBaseString = &mBaseTable.at(baseString.GetCRCID());
	}

	WString::WString(BaseString<const wChar*, wChar> bs)
	{
		if (bs.GetContent() == NULL)
		{
			throw InvisionArgumentException("");
		}


		if (mBaseTable.find(bs.GetCRCID()) == mBaseTable.end())
		{
			mBaseTable[bs.GetCRCID()] = bs;
		}
		rBaseString = &mBaseTable.at(bs.GetCRCID());
	}


	const wChar* WString::toString()
	{
		return rBaseString->GetContent();
	}

	bool WString::operator == (const wChar* rValue)
	{
		return rBaseString->operator==(Hash::getHash(rValue));
	}

	bool WString::operator == (WString rValue)
	{
		return rBaseString->operator==(rValue.rBaseString->GetCRCID());
	}

	size_t WString::Length()
	{
		return (size_t)rBaseString->GetLength();
	}

	WString WString::substring(size_t pos, size_t len)
	{
		return WString(rBaseString->substring(pos, len));
	}

	const wChar& WString::At(size_t index)
	{
		return rBaseString->GetContent()[index];
	}

	const wChar& WString::operator[](size_t index)
	{
		return rBaseString->GetContent()[index];
	}

	WString WString::trim()
	{
		wChar chars[1] = { ' ' };
		BaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, 1);
		return WString(baseString);
	}

	WString WString::trim(wChar chars[], size_t arrSize)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, arrSize);
		return WString(baseString);
	}

	std::vector<WString> WString::Split(wChar chars[], size_t arrSize)
	{
		std::vector<BaseString<const wChar*, wChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);

		std::vector<WString> SplitedStrings;

		for (size_t i = 0; i < splitedInBaseStrings.size(); i++)
		{
			SplitedStrings.push_back(WString(splitedInBaseStrings[i]));
		}

		return SplitedStrings;
	}

	bool WString::contains(const wChar* value)
	{
		bool containsString = false;
		containsString = rBaseString->contains(value);
		return containsString;
	}

	WString WString::replace(const wChar* oldValue, const wChar* newValue)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->replace(oldValue, newValue);
		return WString(baseString);
	}

	WString WString::remove(size_t startIndex)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
		return WString(baseString);
	}

	WString WString::remove(size_t startIndex, size_t count)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, count);
		return WString(baseString);
	}

	size_t WString::indexOf(wChar value)
	{
		size_t index = rBaseString->indexOf(value, 0);
		return index;
	}
	size_t WString::indexOf(wChar value, size_t startIndex)
	{
		size_t index = rBaseString->indexOf(value, startIndex);
		return index;
	}

	size_t WString::indexOf(const wChar* value)
	{
		size_t index = rBaseString->indexOf(value, 0);
		return index;
	}

	size_t WString::indexOf(const wChar* value, size_t startIndex)
	{
		size_t index = rBaseString->indexOf(value, startIndex);
		return index;
	}

	size_t WString::LastIndexOf(wChar value)
	{
		size_t index = rBaseString->lastIndexOf(value, this->Length());
		return index;
	}

	size_t WString::LastIndexOf(wChar value, size_t startIndex)
	{
		size_t index = rBaseString->lastIndexOf(value, startIndex);
		return index;
	}

	size_t WString::LastIndexOf(const wChar* value)
	{
		size_t index = rBaseString->lastIndexOf(value, this->Length());
		return index;
	}

	size_t WString::LastIndexOf(const wChar* value, size_t startIndex)
	{
		size_t index = rBaseString->lastIndexOf(value, startIndex);
		return (index);
	}

	bool WString::endsWith(const wChar* value)
	{
		bool bEndsWith = rBaseString->endsWith(value);
		return bEndsWith;
	}

	bool WString::startsWith(const wChar* value)
	{
		bool bEndsWith = rBaseString->startsWith(value);
		return bEndsWith;
	}

	WString WString::padLeft(size_t totalWidth)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, ' ');
		return WString(baseString);
	}

	WString WString::padLeft(size_t totalWidth, wChar paddingChar)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
		return WString(baseString);
	}


	WString WString::padRight(size_t totalWidth)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, ' ');
		return WString(baseString);
	}

	WString WString::padRight(size_t totalWidth, wChar paddingChar)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
		return WString(baseString);
	}

	WString WString::insert(size_t startIndex, const wChar* value)
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->insert(startIndex, value);
		return WString(baseString);
	}

	WString WString::toUpper()
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->toUpper(true);
		return WString(baseString);
	}

	WString WString::toLower()
	{
		BaseString<const wChar*, wChar> baseString = rBaseString->toLower(true);
		return WString(baseString);
	}
}