/////////////////////////////////////////////////////////////////////////////
// Name:	  CRC
// Purpose:	  offers cryptographic operations
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef CRC_H
#define CRC_H

#include <functional>

namespace Invision
{
	class INVISION_API Hash
	{
	public:
		static uint32 getHash(const char* data)
		{
			//boost::crc_32_type result;
			//result.process_bytes(data, strlen(data));
			//return result.checksum();

			std::hash<const char*> hash;
			uint32 str_hash = hash(data);
			return str_hash;
		}

		static uint32 getHash(const wchar_t* data)
		{
			//boost::crc_32_type result;
			//result.process_bytes(data, wcslen(data));
			//return result.checksum();

			std::hash<const wchar_t*> hash;
			uint32 str_hash = hash(data);
			return str_hash;
		}
	};
}



#endif //CRC_H