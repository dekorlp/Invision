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


#ifndef INSTRINGW_H
#define INSTRINGW_H
#include "InBaseString.h"
#include "CRC.h"
namespace Invision
{
	class InStringW
	{
	private:
		//static std::map<usize_t, InBaseStringMultiByte<const wChar*, wChar>> mBaseTable;
		static std::map<size_t, InBaseString<const wChar*, wChar>> mBaseTable;
		//InBaseStringMultiByte<const wChar*, wChar> *rBaseString;
		InBaseString<const wChar*, wChar> *rBaseString;
		//InBaseString<const char*, char> TestString;
	public:

		INVISION_API InStringW();
		INVISION_API InStringW(const wChar* text);
		//HAL_API InStringW(InBaseStringMultiByte<const wChar*, wChar> is);
		INVISION_API InStringW(InBaseString<const wChar*, wChar> is);
		INVISION_API const wChar* toString();
		INVISION_API bool operator == (const wChar* rValue);
		INVISION_API bool operator == (InStringW rValue);
		INVISION_API size_t Length();
		INVISION_API InStringW substring(size_t pos, size_t len);
		INVISION_API const wChar& At(size_t index);
		INVISION_API const wChar& operator[](size_t index);
		INVISION_API InStringW trim();
		INVISION_API InStringW trim(wChar chars[], size_t arrSize);
		INVISION_API std::vector<InStringW> Split(wChar chars[], size_t arrSize);
		INVISION_API bool contains(const wChar* value);
		INVISION_API InStringW replace(const wChar* oldValue, const wChar* newValue);
		INVISION_API InStringW remove(size_t startIndex);
		INVISION_API InStringW remove(size_t startIndex, size_t count);
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
		INVISION_API InStringW padLeft(size_t totalWidth);
		INVISION_API InStringW padLeft(size_t totalWidth, wChar paddingChar);
		INVISION_API InStringW padRight(size_t totalWidth);
		INVISION_API InStringW padRight(size_t totalWidth, wChar paddingChar);
		INVISION_API InStringW insert(size_t startIndex, const wChar* value);
		INVISION_API InStringW toUpper();
		INVISION_API InStringW toLower();
	};
}
#endif //INSTRINGW_H
