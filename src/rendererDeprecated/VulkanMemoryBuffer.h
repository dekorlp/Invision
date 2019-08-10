/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanMemoryBuffer
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANMEMORYBUFFER
#define INCLUDED_VULKANMEMORYBUFFER

#include <vector>
#include "vulkan/vulkan.h"
#include "VulkanDataVariables.h"
#include "VulkanException.h"


class VulkanMemoryBuffer
{
private:

	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
	void createVertexBuffer(VkPhysicalDevice physicalDevice, SVulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline);
	void cleanUpBuffers(SVulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline);
};

#endif // INCLUDED_VULKANMEMORYBUFFER