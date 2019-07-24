/////////////////////////////////////////////////////////////////////////////
// Name:	  DefaultMemoryTracking
// Purpose:	  allocator subComponent placeholder for allocator class
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef DEFAULTMEMORYTRACKING_H
#define DEFAULTMEMORYTRACKING_H

#include "TrackingStruct.h"

class HAL_API DefaultMemoryTracking
{
public:
	static const size_t TRACKINGSIZE = 0;

	inline void setMemoryTrackingTag(void* memory, int fileNumber, char *fileName, int offset);
};

#endif //DEFAULTMEMORYTRACKING_H