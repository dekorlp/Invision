/////////////////////////////////////////////////////////////////////////////
// Name:	  AdvancedMemoryTracking
// Purpose:	  allocator subComponent who tracks  the line and the file of specific allocation
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////



#ifndef ADVANCEDMEMORYTRACKING_H
#define ADVANCEDMEMORYTRACKING_H

class HAL_API AdvancedMemoryTracking
{
public:
	static const size_t TRACKINGSIZE = sizeof(trackingStruct);

	inline void setMemoryTrackingTag(void* memory, int fileNumber, char *fileName, int offset);
};

#endif //ADVANCEDMEMORYTRACKING_H