/////////////////////////////////////////////////////////////////////////////
// Name:	  StandardBoundsChecking
// Purpose:	  allocator subComponent who places a magic code on the begin and end of each memory block
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef STANDARDBOUNDSCHECKING_H
#define STANDARDBOUNDSCHECKING_H

class HAL_API StandardBoundsChecking
{
public:
	static const unsigned int FRONT_SIZE = 4;
	static const unsigned int BACK_SIZE = 4;

	inline void observeFront(void* memory, unsigned int headerSize);
	inline void observeBack(void* memory);

	inline void checkFront(void* memory, unsigned int offset, unsigned int headerSize);
	inline void checkBack(void* memory);

};

#endif //STANDARDBOUNDSCHECKING_H