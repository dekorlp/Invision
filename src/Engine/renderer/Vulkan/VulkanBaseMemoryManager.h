#ifndef VULKAN_BASE_MEMORY_MANAGER_H
#define VULKAN_BASE_MEMORY_MANAGER_H

#include "vulkan\vulkan.h"
#include "lowlevel\allocator\MemoryBlock.h"
#include "lowlevel\allocator\PoolAllocator.h"
#include "VulkanBaseCommandPool.h"

namespace Invision
{

	enum MemoryType
	{
		MEMORY_TYPE_DEDICATED = 0x01,
		MEMORY_TYPE_SHARED = 0x02
	};

	struct VulkanBaseMemory
	{
		VkDeviceMemory mMemory;
		PoolAllocator mMappedMemory;
		void* mStartPosition;
	};

	struct VulkanBaseBuffer2
	{
		VkDeviceSize mSize;
		VkDeviceSize mOffset;
		VkDeviceSize mBufferOffset;
		VkBuffer mBuffer;
		MemoryType mMemType;
		VkDeviceSize mAllocatedPages;
		bool inUse = false;
	};

	class  VulkanBaseMemoryManager
	{
	public:
		VulkanBaseMemoryManager();
		void Init(const SVulkanBase &vulkanInstance, size_t size);
		void Destroy(const SVulkanBase &vulkanInstance);
		void* BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void* BindToDedicatedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void Unbind(const SVulkanBase &vulkanInstance, void* memory);
		void CopyDataToBuffer(const SVulkanBase &vulkanInstance, void* memory, const void* data);
		void CopyBufferToBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, void* dest);

		VkBuffer GetBuffer(void* handle)
		{
			return ((VulkanBaseBuffer2*)(handle))->mBuffer;
		}

		VkDeviceSize GetOffset(void* handle)
		{
			return ((VulkanBaseBuffer2*)(handle))->mBufferOffset;
		}

	private:

		uint32_t findMemoryType(const VkPhysicalDevice& device, VkMemoryPropertyFlags properties);
		void AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, size_t size, VkDeviceMemory &memory);
		void CreateBuffer(const SVulkanBase &vulkanInstance, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset);

		static VkCommandBuffer beginSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool);
		static void endSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer);

		void* BindBufferToMemory(const SVulkanBase &vulkanInstance, VulkanBaseMemory &memory, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, MemoryType memType);

		VulkanBaseMemory mLocalMemory;
		VulkanBaseMemory mSharedMemory;
	};
}

#endif // VULKAN_BASE_MEMORY_MANAGER_H