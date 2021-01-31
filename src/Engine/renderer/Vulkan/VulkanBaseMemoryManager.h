#ifndef VULKAN_BASE_MEMORY_MANAGER_H
#define VULKAN_BASE_MEMORY_MANAGER_H

#include "vulkan\vulkan.h"
#include "lowlevel\allocator\MemoryBlock.h"
#include "lowlevel\allocator\PoolAllocator.h"
namespace Invision
{

	enum MemoryType
	{
		MEMPORY_TYPE_DEDICATED = 0x01,
		MEMORY_TYPE_SHARED = 0x02
	};

	struct VulkanBaseBuffer2
	{
		VkDeviceSize mSize;
		VkDeviceSize  mOffset;
		VkBuffer mBuffer;
		MemoryType mMemType;
	};

	class  VulkanBaseMemoryManager
	{
	public:
		VulkanBaseMemoryManager();
		void Init(const SVulkanBase &vulkanInstance, uint32_t size);
		void Destroy(const SVulkanBase &vulkanInstance);

	private:

		uint32_t findMemoryType(const VkPhysicalDevice& device, VkMemoryPropertyFlags properties);
		void AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, uint32_t size, VkDeviceMemory &memory);

		PoolAllocator mAllocLocalMemory;
		PoolAllocator mAllocSharedMemory;

		VkDeviceMemory mLocalMemory;
		VkDeviceMemory mSharedMemory;
	};
}

#endif // VULKAN_BASE_MEMORY_MANAGER_H
