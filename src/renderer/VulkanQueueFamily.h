/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanQueueFamily
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANQUEUEFAMILY
#define INCLUDED_VULKANQUEUEFAMILY

#define NOMINMAX

#include <vector>
#include <set>
#include "VulkanException.h"
#include "VulkanDataVariables.h"

class VulkanQueueFamily
{
private:

public:
	VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface) const;
	std::vector<VkDeviceQueueCreateInfo> CreateQueueCreateInfos(
		const std::set<int>& uniqueQueueFamilies) const noexcept;

};

#endif // INCLUDED_VULKANQUEUEFAMILY