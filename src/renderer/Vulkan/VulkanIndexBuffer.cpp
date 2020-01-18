#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"

#include "VulkanIndexBuffer.h"

namespace Invision
{
	VulkanIndexBuffer::VulkanIndexBuffer()
	{
	}

	VulkanIndexBuffer& VulkanIndexBuffer::CreateIndexBuffer(const SVulkan &vulkanInstance, VulkanCommandPool commandPool, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, const void* source, VkDeviceSize offset)
	{
		mOffset = offset;

		VulkanBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode);


		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, (size_t)size);
		vkUnmapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory());

		mIndexBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sharingMode);

		stagingBuffer.CopyBuffer(vulkanInstance, commandPool, mIndexBuffer, 0, 0, size);

		stagingBuffer.DestroyBuffer(vulkanInstance);

		return *this;
	}


	VkBuffer VulkanIndexBuffer::GetBuffer()
	{
		return mIndexBuffer.GetBuffer();
	}

	VkDeviceSize VulkanIndexBuffer::GetOffset()
	{
		return mOffset;
	}

	void VulkanIndexBuffer::DestroyIndexBuffer(const SVulkan &vulkanInstance)
	{
		mIndexBuffer.DestroyBuffer(vulkanInstance);
	}
}