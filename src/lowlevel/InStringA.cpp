#include "precompiled.h"
#include "InStringA.h"


std::map<uint32, InBaseString<const tChar*, tChar>> InStringA::mBaseTable;

InStringA::InStringA()
{

}

InStringA::InStringA(const tChar* text)
{
	if (text == NULL)
	{
		throw invisionCoreArgumentException("");
	}


	InBaseString<const tChar*, tChar> baseString(text, strlen(text), CRC::getCRC32(text));
	uint32 id = baseString.GetCRCID();

	if (mBaseTable.find(id) == mBaseTable.end())
	{
		mBaseTable[id] = baseString;
	}
	rBaseString = &mBaseTable.at(baseString.GetCRCID());
}

InStringA::InStringA(InBaseString<const tChar*, tChar> bs)
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

const tChar* InStringA::toString()
{
	return rBaseString->GetContent();
}

bool InStringA::operator == (const tChar* rValue)
{
	return rBaseString->operator==(CRC::getCRC32(rValue));
}

bool InStringA::operator == (InStringA rValue)
{
	return rBaseString->operator==(rValue.rBaseString->GetCRCID());
}

uint32 InStringA::Length()
{
	return rBaseString->GetLength();
}

InStringA InStringA::substring(uint32 pos, uint32 len)
{
	return InStringA(rBaseString->substring(pos, len)); 
}

const char& InStringA::At(uint32 index)
{
	return rBaseString->GetContent()[index];
}

const char& InStringA::operator[](uint32 index)
{
	return rBaseString->GetContent()[index];
}

InStringA InStringA::trim()
{
	char chars[1] = { ' ' };
	InBaseString<const tChar*, tChar> baseString = rBaseString->trim(chars, 1);
	return InStringA(baseString);
}

InStringA InStringA::trim(tChar chars[], uint32 arrSize)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->trim(chars, arrSize);
	return InStringA(baseString);
}

std::vector<InStringA> InStringA::Split(tChar chars[], uint32 arrSize)
{
	std::vector<InBaseString<const tChar*, tChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);

	std::vector<InStringA> SplitedStrings;
	
	for (uint32 i = 0; i < splitedInBaseStrings.size(); i++)
	{
		SplitedStrings.push_back(InStringA(splitedInBaseStrings[i]));
	}

	return SplitedStrings;
}

bool InStringA::contains(const tChar* value)
{
	bool containsString = false;
	containsString = rBaseString->contains(value);
	return containsString;
}

InStringA InStringA::replace(const tChar* oldValue, const tChar* newValue)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->replace(oldValue, newValue);
	return InStringA(baseString);
}

InStringA InStringA::remove(uint32 startIndex)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
	return InStringA(baseString);
}

InStringA InStringA::remove(uint32 startIndex, uint32 count)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->remove(startIndex, count);
	return InStringA(baseString);
}

int32 InStringA::indexOf(tChar value)
{
	uint32 index = rBaseString->indexOf(value, 0);
	return index;
}
int32 InStringA::indexOf(tChar value, uint32 startIndex)
{
	uint32 index = rBaseString->indexOf(value, startIndex);
	return index;
}

int32 InStringA::indexOf(const tChar* value)
{
	uint32 index = rBaseString->indexOf(value, 0);
	return index;
}

int32 InStringA::indexOf(const tChar* value, uint32 startIndex)
{
	uint32 index = rBaseString->indexOf(value, startIndex);
	return index;
}

int32 InStringA::LastIndexOf(tChar value)
{
	uint32 index = rBaseString->lastIndexOf(value, this->Length());
	return index;
}

int32 InStringA::LastIndexOf(tChar value, uint32 startIndex)
{
	uint32 index = rBaseString->lastIndexOf(value, startIndex);
	return index;
}

int32 InStringA::LastIndexOf(const tChar* value)
{
	uint32 index = rBaseString->lastIndexOf(value, this->Length());
	return index;
}

int32 InStringA::LastIndexOf(const tChar* value, uint32 startIndex)
{
	uint32 index = rBaseString->lastIndexOf(value, startIndex);
	return (index);
}

bool InStringA::endsWith(const tChar* value)
{
	bool bEndsWith = rBaseString->endsWith(value);
	return bEndsWith;
}

bool InStringA::startsWith(const tChar* value)
{
	bool bEndsWith = rBaseString->startsWith(value);
	return bEndsWith;
}

InStringA InStringA::padLeft(uint32 totalWidth)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->padLeft(totalWidth, ' ');
	return InStringA(baseString);
}

InStringA InStringA::padLeft(uint32 totalWidth, tChar paddingChar)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
	return InStringA(baseString);
}


InStringA InStringA::padRight(uint32 totalWidth)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->padRight(totalWidth, ' ');
	return InStringA(baseString);
}

InStringA InStringA::padRight(uint32 totalWidth, tChar paddingChar)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
	return InStringA(baseString);
}

InStringA InStringA::insert(uint32 startIndex, const tChar* value)
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->insert(startIndex, value);
	return InStringA(baseString);
}

InStringA InStringA::toUpper()
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->toUpper(false);
	return InStringA(baseString);
}

InStringA InStringA::toLower()
{
	InBaseString<const tChar*, tChar> baseString = rBaseString->toLower(false);
	return InStringA(baseString);
}