#include "precompiled.h"
#include "String.h"
namespace Invision {

	std::map<size_t, BaseString<const tChar*, tChar>> String::mBaseTable;

	String::String(const tChar* text)
	{
		if (text == NULL)
		{
			throw InvisionArgumentException("");
		}

		BaseString<const tChar*, tChar> baseString(text, strlen(text), Hash::getHash(text));
		size_t id = baseString.GetCRCID();

		if (mBaseTable.find(id) == mBaseTable.end())
		{
			mBaseTable[id] = baseString;
		}
		rBaseString = &mBaseTable.at(baseString.GetCRCID());
	}

	String::String(BaseString<const tChar*, tChar> bs)
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


	const tChar* String::toString()
	{
		return rBaseString->GetContent();
	}

	bool String::operator == (const tChar* rValue)
	{
		return rBaseString->operator==(Hash::getHash(rValue));
	}

	bool String::operator == (String rValue)
	{
		return rBaseString->operator==(rValue.rBaseString->GetCRCID());
	}

	size_t String::Length()
	{
		return (size_t)rBaseString->GetLength();
	}

	String String::substring(size_t pos, size_t len)
	{
		return String(rBaseString->substring(pos, len));
	}

	const tChar& String::At(size_t index)
	{
		return rBaseString->GetContent()[index];
	}

	const tChar& String::operator[](size_t index)
	{
		return rBaseString->GetContent()[index];
	}

	String String::trim()
	{
		tChar chars[1] = { ' ' };
		BaseString<const tChar*, tChar> baseString = rBaseString->trim(chars, 1);
		return String(baseString);
	}

	String String::trim(tChar chars[], size_t arrSize)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->trim(chars, arrSize);
		return String(baseString);
	}

	std::vector<String> String::Split(tChar chars[], size_t arrSize)
	{
		std::vector<BaseString<const tChar*, tChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);

		std::vector<String> SplitedStrings;

		for (size_t i = 0; i < splitedInBaseStrings.size(); i++)
		{
			SplitedStrings.push_back(String(splitedInBaseStrings[i]));
		}

		return SplitedStrings;
	}

	bool String::contains(const tChar* value)
	{
		bool containsString = false;
		containsString = rBaseString->contains(value);
		return containsString;
	}

	String String::replace(const tChar* oldValue, const tChar* newValue)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->replace(oldValue, newValue);
		return String(baseString);
	}

	String String::remove(size_t startIndex)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
		return String(baseString);
	}

	String String::remove(size_t startIndex, size_t count)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->remove(startIndex, count);
		return String(baseString);
	}

	size_t String::indexOf(tChar value)
	{
		size_t index = rBaseString->indexOf(value, 0);
		return index;
	}
	size_t String::indexOf(tChar value, size_t startIndex)
	{
		size_t index = rBaseString->indexOf(value, startIndex);
		return index;
	}

	size_t String::indexOf(const tChar* value)
	{
		size_t index = rBaseString->indexOf(value, 0);
		return index;
	}

	size_t String::indexOf(const tChar* value, size_t startIndex)
	{
		size_t index = rBaseString->indexOf(value, startIndex);
		return index;
	}

	size_t String::LastIndexOf(tChar value)
	{
		size_t index = rBaseString->lastIndexOf(value, this->Length());
		return index;
	}

	size_t String::LastIndexOf(tChar value, size_t startIndex)
	{
		size_t index = rBaseString->lastIndexOf(value, startIndex);
		return index;
	}

	size_t String::LastIndexOf(const tChar* value)
	{
		size_t index = rBaseString->lastIndexOf(value, this->Length());
		return index;
	}

	size_t String::LastIndexOf(const tChar* value, size_t startIndex)
	{
		size_t index = rBaseString->lastIndexOf(value, startIndex);
		return (index);
	}

	bool String::endsWith(const tChar* value)
	{
		bool bEndsWith = rBaseString->endsWith(value);
		return bEndsWith;
	}

	bool String::startsWith(const tChar* value)
	{
		bool bEndsWith = rBaseString->startsWith(value);
		return bEndsWith;
	}

	String String::padLeft(size_t totalWidth)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->padLeft(totalWidth, ' ');
		return String(baseString);
	}

	String String::padLeft(size_t totalWidth, tChar paddingChar)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
		return String(baseString);
	}


	String String::padRight(size_t totalWidth)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->padRight(totalWidth, ' ');
		return String(baseString);
	}

	String String::padRight(size_t totalWidth, tChar paddingChar)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
		return String(baseString);
	}

	String String::insert(size_t startIndex, const tChar* value)
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->insert(startIndex, value);
		return String(baseString);
	}

	String String::toUpper()
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->toUpper(true);
		return String(baseString);
	}

	String String::toLower()
	{
		BaseString<const tChar*, tChar> baseString = rBaseString->toLower(true);
		return String(baseString);
	}
}