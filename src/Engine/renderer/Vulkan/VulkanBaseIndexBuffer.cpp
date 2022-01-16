#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"

#include "VulkanBaseIndexBuffer.h"

namespace Invision
{
	VulkanBaseIndexBuffer::VulkanBaseIndexBuffer()
	{
	}

	VulkanBaseIndexBuffer& VulkanBaseIndexBuffer::CreateIndexBuffer(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, VkDeviceSize size,  const void* source)
	{
		mMemoryManager = &memoryManager;

		void* shared = memoryManager.BindToSharedMemory(vulkanContext, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanContext, shared, source);
		mDedicatedIndexBuffer = memoryManager.BindToDedicatedMemory(vulkanContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyBufferToBuffer(vulkanContext, commandPool, shared, mDedicatedIndexBuffer);
		memoryManager.Unbind(vulkanContext, shared);

		return *this;
	}


	VkBuffer VulkanBaseIndexBuffer::GetBuffer()
	{
		return mMemoryManager->GetBuffer(mDedicatedIndexBuffer);
	}

	VkDeviceSize VulkanBaseIndexBuffer::GetOffset()
	{
		return mMemoryManager->GetOffset(mDedicatedIndexBuffer);
	}

	void VulkanBaseIndexBuffer::DestroyIndexBuffer(const SVulkanContext &vulkanContext)
	{
		mMemoryManager->Unbind(vulkanContext, mDedicatedIndexBuffer);
	}
}