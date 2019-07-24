/////////////////////////////////////////////////////////////////////////////
// Name:	  PointerUtil
// Purpose:	  offers Pointer operation components
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef POINTERUTIL_H
#define POINTERUTIL_H

namespace PointerUtil
{
	inline void* alignForward(void* address, uint8 alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address)+static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline const void* alignForward(const void* address, uint8_t alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address)+static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline void* alignBackward(void* address, uint8_t alignment)
	{
		return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline const void* alignBackward(const void* address, uint8_t alignment)
	{
		return (void*)(reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline uint8_t alignForwardAdjustment(const void* address, uint8_t alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

		if (adjustment == alignment)
			return 0; //already aligned

		return adjustment;
	}

	inline uint8_t alignBackwardAdjustment(const void* address, uint8_t alignment)
	{
		uint8_t adjustment = reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1);

		if (adjustment == alignment)
			return 0; //already aligned

		return adjustment;
	}

	inline void* add(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<uintptr_t>(p)+x);
	}

	inline const void* add(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<uintptr_t>(p)+x);
	}

	inline void* subtract(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<uintptr_t>(p)-x);
	}

	inline const void* subtract(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<uintptr_t>(p)-x);
	}

	inline uint8_t alignForwardAdjustmentWithHeader(const void* address, uint8_t alignment, uint8_t headerSize)
	{
		uint8_t adjustment = alignForwardAdjustment(address, alignment);

		uint8_t neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}

	inline uint8_t alignBackwardAdjustmentWithHeader(const void* address, uint8_t alignment, uint8_t headerSize)
	{
		uint8_t adjustment = alignBackwardAdjustment(address, alignment);

		uint8_t neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}
}
#endif