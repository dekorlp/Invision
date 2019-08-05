/////////////////////////////////////////////////////////////////////////////
// Name:	  WinLocale
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2015-06-05
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#ifndef WINLOCALE_H
#define WINLOCALE_H

class HAL_API WinLocale
{
public:
	uint32 IdentifyCID();
	uint32 IdentifyCID(WCHAR * langISO3166);
	const char* IdentifySystemLanguage();
	const char* IdentifyLanguage(uint32 langId);
	const char* GetCountry();
	const char* GetCountry(WCHAR * langISO3166);



};

#endif // WINLOCALE_H
#endif