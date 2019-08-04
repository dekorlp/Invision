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

class InStringW
{
private:
	//static std::map<usize_t, InBaseStringMultiByte<const wChar*, wChar>> mBaseTable;
	static std::map<size_t, InBaseString<const wChar*, wChar>> mBaseTable;
	//InBaseStringMultiByte<const wChar*, wChar> *rBaseString;
	InBaseString<const wChar*, wChar> *rBaseString;
	//InBaseString<const char*, char> TestString;
public:

	HAL_API InStringW();
	HAL_API InStringW(const wChar* text);
	//HAL_API InStringW(InBaseStringMultiByte<const wChar*, wChar> is);
	HAL_API InStringW(InBaseString<const wChar*, wChar> is);
	HAL_API const wChar* toString();
	HAL_API bool operator == (const wChar* rValue);
	HAL_API bool operator == (InStringW rValue);
	HAL_API size_t Length();
	HAL_API InStringW substring(size_t pos, size_t len);
	HAL_API const wChar& At(size_t index);
	HAL_API const wChar& operator[](size_t index);
	HAL_API InStringW trim();
	HAL_API InStringW trim(wChar chars[], size_t arrSize);
	HAL_API std::vector<InStringW> Split(wChar chars[], size_t arrSize);
	HAL_API bool contains(const wChar* value);
	HAL_API InStringW replace(const wChar* oldValue, const wChar* newValue);
	HAL_API InStringW remove(size_t startIndex);
	HAL_API InStringW remove(size_t startIndex, size_t count);
	HAL_API size_t indexOf(wChar value);
	HAL_API size_t indexOf(wChar value, size_t startIndex);
	HAL_API size_t indexOf(const wChar* value);
	HAL_API size_t indexOf(const wChar* value, size_t startIndex);
	HAL_API size_t LastIndexOf(wChar value);
	HAL_API size_t LastIndexOf(wChar value, size_t startIndex);
	HAL_API size_t LastIndexOf(const wChar* value);
	HAL_API size_t LastIndexOf(const wChar* value, size_t startIndex);
	HAL_API bool endsWith(const wChar* value);
	HAL_API bool startsWith(const wChar* value);
	HAL_API InStringW padLeft(size_t totalWidth);
	HAL_API InStringW padLeft(size_t totalWidth, wChar paddingChar);
	HAL_API InStringW padRight(size_t totalWidth);
	HAL_API InStringW padRight(size_t totalWidth, wChar paddingChar);
	HAL_API InStringW insert(size_t startIndex, const wChar* value);
	HAL_API InStringW toUpper();
	HAL_API InStringW toLower();
};

#endif //INSTRINGW_H
