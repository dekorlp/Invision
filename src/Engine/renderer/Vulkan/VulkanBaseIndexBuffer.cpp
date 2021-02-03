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

	VulkanBaseIndexBuffer& VulkanBaseIndexBuffer::CreateIndexBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, VkDeviceSize size,  const void* source, VkDeviceSize offset)
	{
		mMemoryManager = &memoryManager;

		////////////////////
		// Memory Manager Test
		void* shared = memoryManager.BindToSharedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanInstance, shared, source);
		mDedicatedIndexBuffer = memoryManager.BindToDedicatedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyBufferToBuffer(vulkanInstance, commandPool, shared, mDedicatedIndexBuffer);
		memoryManager.Unbind(vulkanInstance, shared);

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

	void VulkanBaseIndexBuffer::DestroyIndexBuffer(const SVulkanBase &vulkanInstance)
	{
		mMemoryManager->Unbind(vulkanInstance, mDedicatedIndexBuffer);
	}
}