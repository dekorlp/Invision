#include "precompiled.h"
#include "VulkanBaseException.h"

#include "VulkanBase.h"
#include "VulkanBaseBuffer.h"


namespace Invision
{
	void VulkanBaseBuffer::CreateBuffer(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset)
	{
		// create Vertex Buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;

		if (vkCreateBuffer(vulkanInstance.logicalDevice, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		//allocate Vertex Buffer Memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanInstance.logicalDevice, mBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Invision::findMemoryType(vulkanInstance.physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(vulkanInstance.logicalDevice, mBuffer, mBufferMemory, memoryOffset);
	}

	void VulkanBaseBuffer::CopyBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VulkanBaseBuffer &dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool.GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vulkanInstance.logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = srcOffset; // Optional
		copyRegion.dstOffset = dstOffset; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, mBuffer, dstBuffer.GetBuffer(), 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkanInstance.graphicsQueue);
		vkFreeCommandBuffers(vulkanInstance.logicalDevice, commandPool.GetCommandPool(), 1, &commandBuffer);
	}

	void VulkanBaseBuffer::DestroyBuffer(const SVulkanBase &vulkanInstance)
	{
		vkDestroyBuffer(vulkanInstance.logicalDevice, mBuffer, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mBufferMemory, nullptr);
	}
}

