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



class INVISION_API CRC
{
	public:
		static uint32 getCRC32(const char* data)
		{
			boost::crc_32_type result;
			result.process_bytes(data, strlen(data));
			return result.checksum();
		}

		static uint32 getCRC32(const wchar_t* data)
		{
			boost::crc_32_type result;
			result.process_bytes(data, wcslen(data));
			return result.checksum();
		}
};




#endif //CRC_H