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
namespace Invision
{
	class InStringA
	{
	private:
		/*static std::map<size_t, InBaseString<const tChar*, tChar>> mBaseTable;
		InBaseString<const tChar*, tChar> *rBaseString;*/
	public:

		/*INVISION_API InStringA();
		INVISION_API InStringA(const tChar* text);
		INVISION_API InStringA(InBaseString<const tChar*, tChar> is);
		INVISION_API const tChar* toString();
		INVISION_API bool operator == (const tChar* rValue);
		INVISION_API bool operator == (InStringA rValue);
		INVISION_API size_t Length();
		INVISION_API InStringA substring(size_t pos, size_t len);
		INVISION_API const char& At(size_t index);
		INVISION_API const char& operator[](size_t index);
		INVISION_API InStringA trim();
		INVISION_API InStringA trim(tChar chars[], size_t arrSize);
		INVISION_API std::vector<InStringA> Split(tChar chars[], size_t arrSize);
		INVISION_API bool contains(const tChar* value);
		INVISION_API InStringA replace(const tChar* oldValue, const tChar* newValue);
		INVISION_API InStringA remove(size_t startIndex);
		INVISION_API InStringA remove(size_t startIndex, size_t count);
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
		INVISION_API InStringA padLeft(size_t totalWidth);
		INVISION_API InStringA padLeft(size_t totalWidth, tChar paddingChar);
		INVISION_API InStringA padRight(size_t totalWidth);
		INVISION_API InStringA padRight(size_t totalWidth, tChar paddingChar);*/
		//INVISION_API InStringA insert(size_t startIndex, const tChar* value);
		//INVISION_API InStringA toUpper();
		//INVISION_API InStringA toLower();
	};
}
#endif //INSTRINGA_H