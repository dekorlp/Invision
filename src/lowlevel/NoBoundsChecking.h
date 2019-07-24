/////////////////////////////////////////////////////////////////////////////
// Name:	  NoBoundsChecking
// Purpose:	  allocator subComponent who is only a placeholder
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef NOBOUNDSCHECKING_H
#define NOBOUNDSCHECKING_H

class HAL_API NoBoundsChecking
{
public:
	static const size_t FRONT_SIZE = 0;
	static const size_t BACK_SIZE = 0;

	inline void observeFront(void* memory, size_t headerSize);
	inline void observeBack(void* memory);

	inline void checkFront(void* memory, size_t offset, size_t headerSize);
	inline void checkBack(void* memory);

};

#endif //NOBOUNDSCHECKING_H