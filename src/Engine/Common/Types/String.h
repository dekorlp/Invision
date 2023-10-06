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


#ifndef STRING_H
#define STRING_H
#include "BaseString.h"
#include "common/Network/CRC.h"
#include <map>
#include <Common/BaseTypes.h>

namespace Invision
{
	class String
	{
	private:
		static std::map<size_t, BaseString<const tChar*, tChar>> mBaseTable;
			BaseString<const tChar*, tChar> *rBaseString;
	public:

		INVISION_API String(const tChar* text);
		INVISION_API String(BaseString<const tChar*, tChar> is);
		INVISION_API const tChar* toString();
		INVISION_API bool operator == (const tChar* rValue);
		INVISION_API bool operator == (String rValue);
		INVISION_API size_t Length();
		INVISION_API String substring(size_t pos, size_t len);
		INVISION_API const char& At(size_t index);
		INVISION_API const char& operator[](size_t index);
		INVISION_API String trim();
		INVISION_API String trim(tChar chars[], size_t arrSize);
		INVISION_API std::vector<String> Split(tChar chars[], size_t arrSize);
		INVISION_API bool contains(const tChar* value);
		INVISION_API String replace(const tChar* oldValue, const tChar* newValue);
		INVISION_API String remove(size_t startIndex);
		INVISION_API String remove(size_t startIndex, size_t count);
		INVISION_API size_t indexOf(tChar value);
		INVISION_API size_t indexOf(tChar value, size_t startIndex);
		INVISION_API size_t indexOf(const tChar* value);
		INVISION_API size_t indexOf(const tChar* value, size_t startIndex);
		INVISION_API size_t LastIndexOf(tChar value);
		INVISION_API size_t LastIndexOf(tChar value, size_t startIndex);
		INVISION_API size_t LastIndexOf(const tChar* value);
		INVISION_API size_t LastIndexOf(const tChar* value, size_t startIndex);
		INVISION_API bool endsWith(const tChar* value);
		INVISION_API bool startsWith(const tChar* value);
		INVISION_API String padLeft(size_t totalWidth);
		INVISION_API String padLeft(size_t totalWidth, tChar paddingChar);
		INVISION_API String padRight(size_t totalWidth);
		INVISION_API String padRight(size_t totalWidth, tChar paddingChar);
		INVISION_API String insert(size_t startIndex, const tChar* value);
		INVISION_API String toUpper();
		INVISION_API String toLower();
	};

	std::ostream& operator<<(std::ostream& out, String str)
	{
		out << str.toString(); // for example
		return out;
	}
}
#endif //STRING_H
