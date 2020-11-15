#ifndef VULKAN_BASE_BUFFER_H
#define VULKAN_BASE_BUFFER_H


#include "vulkan\vulkan.h"
#include "VulkanBaseCommandPool.h"

namespace Invision
{
	class VulkanBaseBuffer
	{
		public:
			void CreateBuffer(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset = 0);

			void DestroyBuffer(const SVulkanBase &vulkanInstance);

			void CopyBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VulkanBaseBuffer &dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

			static VkCommandBuffer beginSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool);

			static void endSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer);

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

#endif // VULKAN_BASE_BUFFER_H