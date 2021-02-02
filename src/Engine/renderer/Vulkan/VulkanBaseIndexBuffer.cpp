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
		mOffset = offset;

		VulkanBaseBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE);

		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, (size_t)size);
		vkUnmapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory());

		mIndexBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);

		stagingBuffer.CopyBuffer(vulkanInstance, commandPool, mIndexBuffer, 0, 0, size);

		stagingBuffer.DestroyBuffer(vulkanInstance);

		////////////////////
		// Memory Manager Test
		void* mem = memoryManager.BindToSharedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToMemory(vulkanInstance, mem, source);

		memoryManager.Unbind(vulkanInstance, mem);


		return *this;
	}


	VkBuffer VulkanBaseIndexBuffer::GetBuffer()
	{
		return mIndexBuffer.GetBuffer();
	}

	VkDeviceSize VulkanBaseIndexBuffer::GetOffset()
	{
		return mOffset;
	}

	void VulkanBaseIndexBuffer::DestroyIndexBuffer(const SVulkanBase &vulkanInstance)
	{
		mIndexBuffer.DestroyBuffer(vulkanInstance);
	}
}