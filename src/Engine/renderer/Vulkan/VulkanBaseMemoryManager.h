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

	struct VulkanBaseMemory
	{
		VkDeviceMemory mMemory;
		PoolAllocator mMappedMemory;
		void* mStartPosition;
	};

	struct VulkanBaseBuffer2
	{
		VkDeviceSize mSize;
		VkDeviceSize  mOffset;
		VkBuffer mBuffer;
		MemoryType mMemType;
		unsigned int mAllocatedPages;
		bool inUse = false;
	};

	class  VulkanBaseMemoryManager
	{
	public:
		VulkanBaseMemoryManager();
		void Init(const SVulkanBase &vulkanInstance, uint32_t size);
		void Destroy(const SVulkanBase &vulkanInstance);
		void* BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void Unbind(const SVulkanBase &vulkanInstance, void* memory);
		void CopyDataToMemory(const SVulkanBase &vulkanInstance, void* memory, const void* data);

	private:

		uint32_t findMemoryType(const VkPhysicalDevice& device, VkMemoryPropertyFlags properties);
		void AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, uint32_t size, VkDeviceMemory &memory);
		void CreateBuffer(const SVulkanBase &vulkanInstance, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset);


		//PoolAllocator mAllocLocalMemory;
		//PoolAllocator mAllocSharedMemory;

		//VkDeviceMemory mLocalMemory;
		//VkDeviceMemory mSharedMemory;

		VulkanBaseMemory mLocalMemory;
		VulkanBaseMemory mSharedMemory;
	};
}

#endif // VULKAN_BASE_MEMORY_MANAGER_H
