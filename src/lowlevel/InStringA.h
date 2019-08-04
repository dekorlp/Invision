/////////////////////////////////////////////////////////////////////////////
// Name:	  InStringA
// Purpose:	  offers a string component for ANSI Char
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INSTRINGA_H
#define INSTRINGA_H

#include "InBaseString.h"
#include "CRC.h"

class InStringA
{
private:
	static std::map<size_t, InBaseString<const tChar*, tChar>> mBaseTable;
	InBaseString<const tChar*, tChar> *rBaseString;
	//InBaseString<const char*, char> TestString;
public:

	HAL_API InStringA();
	HAL_API InStringA(const tChar* text);
	HAL_API InStringA(InBaseString<const tChar*, tChar> is);
	HAL_API const tChar* toString();
	HAL_API bool operator == (const tChar* rValue);
	HAL_API bool operator == (InStringA rValue);
	HAL_API size_t Length();
	HAL_API InStringA substring(size_t pos, size_t len);
	HAL_API const char& At(size_t index);
	HAL_API const char& operator[](size_t index);
	HAL_API InStringA trim();
	HAL_API InStringA trim(tChar chars[], size_t arrSize);
	HAL_API std::vector<InStringA> Split(tChar chars[], size_t arrSize);
	HAL_API bool contains(const tChar* value);
	HAL_API InStringA replace(const tChar* oldValue, const tChar* newValue);
	HAL_API InStringA remove(size_t startIndex);
	HAL_API InStringA remove(size_t startIndex, size_t count);
	HAL_API size_t indexOf(tChar value);
	HAL_API size_t indexOf(tChar value, size_t startIndex);
	HAL_API size_t indexOf(const tChar* value);
	HAL_API size_t indexOf(const tChar* value, size_t startIndex);
	HAL_API size_t LastIndexOf(tChar value);
	HAL_API size_t LastIndexOf(tChar value, size_t startIndex);
	HAL_API size_t LastIndexOf(const tChar* value);
	HAL_API size_t LastIndexOf(const tChar* value, size_t startIndex);
	HAL_API bool endsWith(const tChar* value);
	HAL_API bool startsWith(const tChar* value);
	HAL_API InStringA padLeft(size_t totalWidth);
	HAL_API InStringA padLeft(size_t totalWidth, tChar paddingChar);
	HAL_API InStringA padRight(size_t totalWidth);
	HAL_API InStringA padRight(size_t totalWidth, tChar paddingChar);
	HAL_API InStringA insert(size_t startIndex, const tChar* value);
	HAL_API InStringA toUpper();
	HAL_API InStringA toLower();
};

#endif //INSTRINGA_H