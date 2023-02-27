/////////////////////////////////////////////////////////////////////////////
// Name:	  InStringW
// Purpose:	  offers a string component for UNICODE Char
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef WSTRING_H
#define WSTRING_H
#include "BaseString.h"
#include "common/Network/CRC.h"
#include <map>
#include <Common/BaseTypes.h>

namespace Invision
{
	class WString
	{
	private:
		static std::map<size_t, BaseString<const wChar*, wChar>> mBaseTable;
		BaseString<const wChar*, wChar>* rBaseString;
	public:

		INVISION_API WString(const wChar* text);
		INVISION_API WString(BaseString<const wChar*, wChar> is);
		INVISION_API const wChar* toString();
		INVISION_API bool operator == (const wChar* rValue);
		INVISION_API bool operator == (WString rValue);
		INVISION_API size_t Length();
		INVISION_API WString substring(size_t pos, size_t len);
		INVISION_API const wChar& At(size_t index);
		INVISION_API const wChar& operator[](size_t index);
		INVISION_API WString trim();
		INVISION_API WString trim(wChar chars[], size_t arrSize);
		INVISION_API std::vector<WString> Split(wChar chars[], size_t arrSize);
		INVISION_API bool contains(const wChar* value);
		INVISION_API WString replace(const wChar* oldValue, const wChar* newValue);
		INVISION_API WString remove(size_t startIndex);
		INVISION_API WString remove(size_t startIndex, size_t count);
		INVISION_API size_t indexOf(wChar value);
		INVISION_API size_t indexOf(wChar value, size_t startIndex);
		INVISION_API size_t indexOf(const wChar* value);
		INVISION_API size_t indexOf(const wChar* value, size_t startIndex);
		INVISION_API size_t LastIndexOf(wChar value);
		INVISION_API size_t LastIndexOf(wChar value, size_t startIndex);
		INVISION_API size_t LastIndexOf(const wChar* value);
		INVISION_API size_t LastIndexOf(const wChar* value, size_t startIndex);
		INVISION_API bool endsWith(const wChar* value);
		INVISION_API bool startsWith(const wChar* value);
		INVISION_API WString padLeft(size_t totalWidth);
		INVISION_API WString padLeft(size_t totalWidth, wChar paddingChar);
		INVISION_API WString padRight(size_t totalWidth);
		INVISION_API WString padRight(size_t totalWidth, wChar paddingChar);
		INVISION_API WString insert(size_t startIndex, const wChar* value);
		INVISION_API WString toUpper();
		INVISION_API WString toLower();
	};

	std::wostream& operator<<(std::wostream& out, WString str)
	{
		out << str.toString(); // for example
		return out;
	}
}
#endif //WSTRING_H
