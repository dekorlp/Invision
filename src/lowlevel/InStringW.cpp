#include "precompiled.h"
#include "InStringW.h"

//std::map<uint32, InBaseStringMultiByte<const wChar*, wChar>> InStringW::mBaseTable;
std::map<uint32, InBaseString<const wChar*, wChar>> InStringW::mBaseTable;

InStringW::InStringW()
{

}

InStringW::InStringW(const wChar* text)
{
	if (text == NULL)
	{
		throw invisionCoreArgumentException("");
	}


	//InBaseStringMultiByte<const wChar*, wChar> baseString(text, wcslen(text), CRC::getCRC32(text));
	InBaseString<const wChar*, wChar> baseString(text, wcslen(text), CRC::getCRC32(text));
	uint32 id = baseString.GetCRCID();

	if (mBaseTable.find(id) == mBaseTable.end())
	{
		mBaseTable[id] = baseString;
	}
	rBaseString = &mBaseTable.at(baseString.GetCRCID());
}

//InStringW::InStringW(InBaseStringMultiByte<const wChar*, wChar> bs)
InStringW::InStringW(InBaseString<const wChar*, wChar> bs)
{
	if (bs.GetContent() == NULL)
	{
		throw invisionCoreArgumentException("");
	}


	if (mBaseTable.find(bs.GetCRCID()) == mBaseTable.end())
	{
		mBaseTable[bs.GetCRCID()] = bs;
	}
	rBaseString = &mBaseTable.at(bs.GetCRCID());
}


const wChar* InStringW::toString()
{
	return rBaseString->GetContent();
}

bool InStringW::operator == (const wChar* rValue)
{
	return rBaseString->operator==(CRC::getCRC32(rValue));
}

bool InStringW::operator == (InStringW rValue)
{
	return rBaseString->operator==(rValue.rBaseString->GetCRCID());
}

uint32 InStringW::Length()
{
	return rBaseString->GetLength();
}

InStringW InStringW::substring(uint32 pos, uint32 len)
{
	return InStringW(rBaseString->substring(pos, len));
}

const wChar& InStringW::At(uint32 index)
{
	return rBaseString->GetContent()[index];
}

const wChar& InStringW::operator[](uint32 index)
{
	return rBaseString->GetContent()[index];
}

InStringW InStringW::trim()
{
	wChar chars[1] = { ' ' };
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->trim(chars, 1);
	InBaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, 1);
	return InStringW(baseString);
}

InStringW InStringW::trim(wChar chars[], uint32 arrSize)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->trim(chars, arrSize);
	InBaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, arrSize);
	return InStringW(baseString);
}

std::vector<InStringW> InStringW::Split(wChar chars[], uint32 arrSize)
{
	//std::vector<InBaseStringMultiByte<const wChar*, wChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);
	std::vector<InBaseString<const wChar*, wChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);

	std::vector<InStringW> SplitedStrings;

	for (uint32 i = 0; i < splitedInBaseStrings.size(); i++)
	{
		SplitedStrings.push_back(InStringW(splitedInBaseStrings[i]));
	}

	return SplitedStrings;
}

bool InStringW::contains(const wChar* value)
{
	bool containsString = false;
	containsString = rBaseString->contains(value);
	return containsString;
}

InStringW InStringW::replace(const wChar* oldValue, const wChar* newValue)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->replace(oldValue, newValue);
	InBaseString<const wChar*, wChar> baseString = rBaseString->replace(oldValue, newValue);
	return InStringW(baseString);
}

InStringW InStringW::remove(uint32 startIndex)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
	InBaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
	return InStringW(baseString);
}

InStringW InStringW::remove(uint32 startIndex, uint32 count)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->remove(startIndex, count);
	InBaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, count);
	return InStringW(baseString);
}

int32 InStringW::indexOf(wChar value)
{
	uint32 index = rBaseString->indexOf(value, 0);
	return index;
}
int32 InStringW::indexOf(wChar value, uint32 startIndex)
{
	uint32 index = rBaseString->indexOf(value, startIndex);
	return index;
}

int32 InStringW::indexOf(const wChar* value)
{
	uint32 index = rBaseString->indexOf(value, 0);
	return index;
}

int32 InStringW::indexOf(const wChar* value, uint32 startIndex)
{
	uint32 index = rBaseString->indexOf(value, startIndex);
	return index;
}

int32 InStringW::LastIndexOf(wChar value)
{
	uint32 index = rBaseString->lastIndexOf(value, this->Length());
	return index;
}

int32 InStringW::LastIndexOf(wChar value, uint32 startIndex)
{
	uint32 index = rBaseString->lastIndexOf(value, startIndex);
	return index;
}

int32 InStringW::LastIndexOf(const wChar* value)
{
	uint32 index = rBaseString->lastIndexOf(value, this->Length());
	return index;
}

int32 InStringW::LastIndexOf(const wChar* value, uint32 startIndex)
{
	uint32 index = rBaseString->lastIndexOf(value, startIndex);
	return (index);
}

bool InStringW::endsWith(const wChar* value)
{
	bool bEndsWith = rBaseString->endsWith(value);
	return bEndsWith;
}

bool InStringW::startsWith(const wChar* value)
{
	bool bEndsWith = rBaseString->startsWith(value);
	return bEndsWith;
}

InStringW InStringW::padLeft(uint32 totalWidth)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, ' ');
	InBaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, ' ');
	return InStringW(baseString);
}

InStringW InStringW::padLeft(uint32 totalWidth, wChar paddingChar)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
	InBaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
	return InStringW(baseString);
}


InStringW InStringW::padRight(uint32 totalWidth)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, ' ');
	InBaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, ' ');
	return InStringW(baseString);
}

InStringW InStringW::padRight(uint32 totalWidth, wChar paddingChar)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
	InBaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
	return InStringW(baseString);
}

InStringW InStringW::insert(uint32 startIndex, const wChar* value)
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->insert(startIndex, value);
	InBaseString<const wChar*, wChar> baseString = rBaseString->insert(startIndex, value);
	return InStringW(baseString);
}

InStringW InStringW::toUpper()
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->toUpper(true);
	InBaseString<const wChar*, wChar> baseString = rBaseString->toUpper(true);
	return InStringW(baseString);
}

InStringW InStringW::toLower()
{
	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->toLower(true);
	InBaseString<const wChar*, wChar> baseString = rBaseString->toLower(true);
	return InStringW(baseString);
}