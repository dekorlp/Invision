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
	//static std::map<uint32, InBaseStringMultiByte<const wChar*, wChar>> mBaseTable;
	static std::map<uint32, InBaseString<const wChar*, wChar>> mBaseTable;
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
	HAL_API uint32 Length();
	HAL_API InStringW substring(uint32 pos, uint32 len);
	HAL_API const wChar& At(uint32 index);
	HAL_API const wChar& operator[](uint32 index);
	HAL_API InStringW trim();
	HAL_API InStringW trim(wChar chars[], uint32 arrSize);
	HAL_API std::vector<InStringW> Split(wChar chars[], uint32 arrSize);
	HAL_API bool contains(const wChar* value);
	HAL_API InStringW replace(const wChar* oldValue, const wChar* newValue);
	HAL_API InStringW remove(uint32 startIndex);
	HAL_API InStringW remove(uint32 startIndex, uint32 count);
	HAL_API int32 indexOf(wChar value);
	HAL_API int32 indexOf(wChar value, uint32 startIndex);
	HAL_API int32 indexOf(const wChar* value);
	HAL_API int32 indexOf(const wChar* value, uint32 startIndex);
	HAL_API int32 LastIndexOf(wChar value);
	HAL_API int32 LastIndexOf(wChar value, uint32 startIndex);
	HAL_API int32 LastIndexOf(const wChar* value);
	HAL_API int32 LastIndexOf(const wChar* value, uint32 startIndex);
	HAL_API bool endsWith(const wChar* value);
	HAL_API bool startsWith(const wChar* value);
	HAL_API InStringW padLeft(uint32 totalWidth);
	HAL_API InStringW padLeft(uint32 totalWidth, wChar paddingChar);
	HAL_API InStringW padRight(uint32 totalWidth);
	HAL_API InStringW padRight(uint32 totalWidth, wChar paddingChar);
	HAL_API InStringW insert(uint32 startIndex, const wChar* value);
	HAL_API InStringW toUpper();
	HAL_API InStringW toLower();
};

#endif //INSTRINGW_H
