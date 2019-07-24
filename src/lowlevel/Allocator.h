/////////////////////////////////////////////////////////////////////////////
// Name:	  Allocator
// Purpose:	  allocator main class (Policy Based Design) | MemoryBoundChecking, many Allocator classes and MemoryTracking classes are supported
// Author:	  Dennis Koehler
// Created:	  2015-06-12
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "PointerUtil.h"

template <class AllocationPolicy, class BoundsCheckingPolicy, class MemoryTrackingPolicy>
class Allocator
{
private:
	AllocationPolicy m_allocator;
	BoundsCheckingPolicy m_BoundsChecking;
	MemoryTrackingPolicy m_memoryTracking;

public:
	Allocator(unsigned int size, unsigned int countVariables)
		: m_allocator((size + 
			AllocationPolicy::HEADERSIZE + 
			MemoryTrackingPolicy::TRACKINGSIZE*2 + 
			BoundsCheckingPolicy::FRONT_SIZE + 
			BoundsCheckingPolicy::BACK_SIZE) * countVariables)
	{	
	}

	Allocator(unsigned int size)
		: m_allocator(size)
	{
	}

	void* allocate(uint8_t blocksize, uint8_t line, char* file, uint8_t alignment = 4)
	{
		void *p = m_allocator.allocate(blocksize, alignment, BoundsCheckingPolicy::FRONT_SIZE, BoundsCheckingPolicy::BACK_SIZE, MemoryTrackingPolicy::TRACKINGSIZE, line, file);
		
		/*
		cout << "Blocksize: " << (int)blocksize << endl;
		cout << "Alignment: " << (int)alignment << endl;
		cout << "BoundCheckingFront: " << BoundsCheckingPolicy::FRONT_SIZE << endl;
		cout << "BouncCheckingBack: " << BoundsCheckingPolicy::BACK_SIZE << endl;
		cout << "MemoryTrackingSize:" << MemoryTrackingPolicy::TRACKINGSIZE << endl;
		*/
		
		if (p != nullptr)
		{
			m_BoundsChecking.observeFront(p, AllocationPolicy::HEADERSIZE + MemoryTrackingPolicy::TRACKINGSIZE + BoundsCheckingPolicy::FRONT_SIZE);
			m_memoryTracking.setMemoryTrackingTag(p, line, file, MemoryTrackingPolicy::TRACKINGSIZE);
			m_BoundsChecking.observeBack(PointerUtil::add(p,  blocksize));
		}
		else
		{
			return nullptr;
		}

		return p;
	}

	uint8_t getAllocationCount()
	{
		return m_allocator.getAllocationCount();
	}

	void* getArena()
	{
		return getAllocationCount.getArena();
	}

	uint8_t getUsedMemory()
	{
		return getAllocationCount.getUsedMemory();
	}

	uint8_t getTotalMemory()
	{
		return getAllocationCount.getTotalMemory();
	}

	void deallocate(void* obj, uint8_t line, char* file)
	{
		
		if (AllocationPolicy::ISDEALLOCATABLE)
		{
			m_BoundsChecking.checkFront(obj, MemoryTrackingPolicy::TRACKINGSIZE, AllocationPolicy::HEADERSIZE);
			m_BoundsChecking.checkBack(obj);
			m_allocator.deallocate(obj, MemoryTrackingPolicy::TRACKINGSIZE,  BoundsCheckingPolicy::FRONT_SIZE);
		}
		else
		{
			
		
#if (_MSC_VER >= 1500) // at least Visual Studio 2008
			char buffer[256];
			OutputDebugString("INVISION WARNING: deallocate is not supported by LinearAllocatorPolicy, use instead another Allocator Policy! \n");
			sprintf(buffer, "File: %s\n", file);
			OutputDebugString(buffer);
			sprintf(buffer, "Line: %i\n", line);
			OutputDebugString(buffer);
			return;
#endif
		}
	}

	void clear()
	{
		m_allocator.clear();
	}

	virtual ~Allocator()
	{
		
	}
};

#endif