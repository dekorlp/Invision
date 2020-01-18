#ifndef VULKAN_INDEX_BUFFER_H
#define VULKAN_INDEX_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBuffer.h"

namespace Invision
{
	class  VulkanIndexBuffer
	{
	public:
		INVISION_API VulkanIndexBuffer();
		INVISION_API VulkanIndexBuffer& CreateIndexBuffer(const SVulkan &vulkanInstance, VulkanCommandPool commandPool, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, const void* source, VkDeviceSize offset);
		INVISION_API VkBuffer GetBuffer();
		INVISION_API VkDeviceSize GetOffset();

		INVISION_API void DestroyIndexBuffer(const SVulkan &vulkanInstance);


	private:
		VkDeviceSize mOffset;

		VulkanBuffer mIndexBuffer;

	};
}
#endif // VULKAN_INDEX_BUFFER_H