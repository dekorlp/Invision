#ifndef VULKAN_BASE_INDEX_BUFFER_H
#define VULKAN_BASE_INDEX_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBaseBuffer.h"

namespace Invision
{
	class VulkanBaseMemoryManager;

	class  VulkanBaseIndexBuffer
	{
	public:
		INVISION_API VulkanBaseIndexBuffer();
		INVISION_API VulkanBaseIndexBuffer& CreateIndexBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, VkDeviceSize size, const void* source, VkDeviceSize offset);
		INVISION_API VkBuffer GetBuffer();
		INVISION_API VkDeviceSize GetOffset();

		INVISION_API void DestroyIndexBuffer(const SVulkanBase &vulkanInstance);


	private:
		VkDeviceSize mOffset;

		VulkanBaseBuffer mIndexBuffer;

	};
}
#endif // VULKAN_BASE_INDEX_BUFFER_H