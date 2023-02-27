#include "precompiled.h"
#include "InStringW.h"
namespace Invision {

	//std::map<size_t, InBaseString<const wChar*, wChar>> InStringW::mBaseTable;

	//InStringW::InStringW()
	//{

	//}

	//InStringW::InStringW(const wChar* text)
	//{
	//	if (text == NULL)
	//	{
	//		throw InvisionArgumentException("");
	//	}


	//	//InBaseStringMultiByte<const wChar*, wChar> baseString(text, wcslen(text), CRC::getCRC32(text));
	//	InBaseString<const wChar*, wChar> baseString(text, wcslen(text), Hash::getHash(text));
	//	size_t id = baseString.GetCRCID();

	//	if (mBaseTable.find(id) == mBaseTable.end())
	//	{
	//		mBaseTable[id] = baseString;
	//	}
	//	rBaseString = &mBaseTable.at(baseString.GetCRCID());
	//}

	////InStringW::InStringW(InBaseStringMultiByte<const wChar*, wChar> bs)
	//InStringW::InStringW(InBaseString<const wChar*, wChar> bs)
	//{
	//	if (bs.GetContent() == NULL)
	//	{
	//		throw InvisionArgumentException("");
	//	}


	//	if (mBaseTable.find(bs.GetCRCID()) == mBaseTable.end())
	//	{
	//		mBaseTable[bs.GetCRCID()] = bs;
	//	}
	//	rBaseString = &mBaseTable.at(bs.GetCRCID());
	//}


	//const wChar* InStringW::toString()
	//{
	//	return rBaseString->GetContent();
	//}

	//bool InStringW::operator == (const wChar* rValue)
	//{
	//	return rBaseString->operator==(Hash::getHash(rValue));
	//}

	//bool InStringW::operator == (InStringW rValue)
	//{
	//	return rBaseString->operator==(rValue.rBaseString->GetCRCID());
	//}

	//size_t InStringW::Length()
	//{
	//	return (size_t)rBaseString->GetLength();
	//}

	//InStringW InStringW::substring(size_t pos, size_t len)
	//{
	//	return InStringW(rBaseString->substring(pos, len));
	//}

	//const wChar& InStringW::At(size_t index)
	//{
	//	return rBaseString->GetContent()[index];
	//}

	//const wChar& InStringW::operator[](size_t index)
	//{
	//	return rBaseString->GetContent()[index];
	//}

	//InStringW InStringW::trim()
	//{
	//	wChar chars[1] = { ' ' };
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->trim(chars, 1);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, 1);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::trim(wChar chars[], size_t arrSize)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->trim(chars, arrSize);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->trim(chars, arrSize);
	//	return InStringW(baseString);
	//}

	//std::vector<InStringW> InStringW::Split(wChar chars[], size_t arrSize)
	//{
	//	//std::vector<InBaseStringMultiByte<const wChar*, wChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);
	//	std::vector<InBaseString<const wChar*, wChar>> splitedInBaseStrings = rBaseString->split(chars, arrSize);

	//	std::vector<InStringW> SplitedStrings;

	//	for (size_t i = 0; i < splitedInBaseStrings.size(); i++)
	//	{
	//		SplitedStrings.push_back(InStringW(splitedInBaseStrings[i]));
	//	}

	//	return SplitedStrings;
	//}

	//bool InStringW::contains(const wChar* value)
	//{
	//	bool containsString = false;
	//	containsString = rBaseString->contains(value);
	//	return containsString;
	//}

	//InStringW InStringW::replace(const wChar* oldValue, const wChar* newValue)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->replace(oldValue, newValue);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->replace(oldValue, newValue);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::remove(size_t startIndex)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, this->Length() - startIndex);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::remove(size_t startIndex, size_t count)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->remove(startIndex, count);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->remove(startIndex, count);
	//	return InStringW(baseString);
	//}

	//size_t InStringW::indexOf(wChar value)
	//{
	//	size_t index = rBaseString->indexOf(value, 0);
	//	return index;
	//}
	//size_t InStringW::indexOf(wChar value, size_t startIndex)
	//{
	//	size_t index = rBaseString->indexOf(value, startIndex);
	//	return index;
	//}

	//size_t InStringW::indexOf(const wChar* value)
	//{
	//	size_t index = rBaseString->indexOf(value, 0);
	//	return index;
	//}

	//size_t InStringW::indexOf(const wChar* value, size_t startIndex)
	//{
	//	size_t index = rBaseString->indexOf(value, startIndex);
	//	return index;
	//}

	//size_t InStringW::LastIndexOf(wChar value)
	//{
	//	size_t index = rBaseString->lastIndexOf(value, this->Length());
	//	return index;
	//}

	//size_t InStringW::LastIndexOf(wChar value, size_t startIndex)
	//{
	//	size_t index = rBaseString->lastIndexOf(value, startIndex);
	//	return index;
	//}

	//size_t InStringW::LastIndexOf(const wChar* value)
	//{
	//	size_t index = rBaseString->lastIndexOf(value, this->Length());
	//	return index;
	//}

	//size_t InStringW::LastIndexOf(const wChar* value, size_t startIndex)
	//{
	//	size_t index = rBaseString->lastIndexOf(value, startIndex);
	//	return (index);
	//}

	//bool InStringW::endsWith(const wChar* value)
	//{
	//	bool bEndsWith = rBaseString->endsWith(value);
	//	return bEndsWith;
	//}

	//bool InStringW::startsWith(const wChar* value)
	//{
	//	bool bEndsWith = rBaseString->startsWith(value);
	//	return bEndsWith;
	//}

	//InStringW InStringW::padLeft(size_t totalWidth)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, ' ');
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, ' ');
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::padLeft(size_t totalWidth, wChar paddingChar)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->padLeft(totalWidth, paddingChar);
	//	return InStringW(baseString);
	//}


	//InStringW InStringW::padRight(size_t totalWidth)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, ' ');
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, ' ');
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::padRight(size_t totalWidth, wChar paddingChar)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->padRight(totalWidth, paddingChar);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::insert(size_t startIndex, const wChar* value)
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->insert(startIndex, value);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->insert(startIndex, value);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::toUpper()
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->toUpper(true);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->toUpper(true);
	//	return InStringW(baseString);
	//}

	//InStringW InStringW::toLower()
	//{
	//	//InBaseStringMultiByte<const wChar*, wChar> baseString = rBaseString->toLower(true);
	//	InBaseString<const wChar*, wChar> baseString = rBaseString->toLower(true);
	//	return InStringW(baseString);
	//}
}