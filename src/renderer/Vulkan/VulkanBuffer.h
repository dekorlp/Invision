#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H


#include "vulkan\vulkan.h"
#include "VulkanCommandPool.h"

namespace Invision
{
	class VulkanBuffer
	{
		public:
			void CreateBuffer(const SVulkan &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset = 0);

			void DestroyBuffer(const SVulkan &vulkanInstance);

			void CopyBuffer(const SVulkan &vulkanInstance, VulkanCommandPool &commandPool, VulkanBuffer &dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

			VkBuffer GetBuffer()
			{
				return mBuffer;
			}

			VkDeviceMemory GetDeviceMemory()
			{
				return mBufferMemory;
			}

		private:
			VkBuffer mBuffer;
			VkDeviceMemory mBufferMemory;
	};
}

#endif // VULKAN_BUFFER_H