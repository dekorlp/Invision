#ifndef VULKAN_BASE_MEMORY_MANAGER_H
#define VULKAN_BASE_MEMORY_MANAGER_H

#include "vulkan\vulkan.h"
#include "lowlevel\allocator\MemoryBlock.h"
#include "lowlevel\allocator\PoolAllocator.h"
#include "common/DoubleLinkedList.h"
#include "VulkanBaseCommandPool.h"

namespace Invision
{

	enum MemoryType
	{
		MEMORY_TYPE_DEDICATED = 0x01,
		MEMORY_TYPE_SHARED = 0x02
	};

	struct VulkanAllocation
	{
		unsigned int pageIndex;
		VkDeviceSize size;
		VkBuffer mBuffer;
		VkDeviceSize mBufferOffset;
		MemoryType mMemType;
	};

	struct VulkanPage
	{
		bool mInUse = false;
	};

	struct VulkanChunk
	{
		MemoryType mMemType;
		std::vector<VulkanPage> mPages;
		DoubleLinkedList<VulkanAllocation> mAllocations;
		VkDeviceMemory mMemory;
		uint32_t pageSize;
	};

#define PAGESIZE 10

	class  VulkanBaseMemoryManager
	{
	public:
		VulkanBaseMemoryManager();
		void Init(const SVulkanBase &vulkanInstance, size_t size);
		void Destroy(const SVulkanBase &vulkanInstance);
		void* BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void* BindToDedicatedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void* BindImageToDedicatedMemory(const SVulkanBase &vulkanInstance, VkImage &image, VkDeviceSize size);
		void Unbind(const SVulkanBase &vulkanInstance, void* memory);
		void CopyDataToBuffer(const SVulkanBase &vulkanInstance, void* memory, const void* data);
		void CopyBufferToBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, void* dest);
		void CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, VkImage& image, uint32_t baseArrayLayer, uint32_t width, uint32_t height);

		static VkCommandBuffer BeginSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool);
		static void EndSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer);

		VkBuffer GetBuffer(void* handle)
		{
			return ((Invision::LinkedListNode<VulkanAllocation>*)(handle))->mData.mBuffer;
		}

		VkDeviceSize GetOffset(void* handle)
		{
			return ((Invision::LinkedListNode<VulkanAllocation>*)(handle))->mData.mBufferOffset;
		}

	private:

		uint32_t findMemoryType(const VkPhysicalDevice& device, VkMemoryPropertyFlags properties);
		void AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, size_t size, VkDeviceMemory &memory);
		void CreateBuffer(const SVulkanBase &vulkanInstance, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset);

		//void* BindBufferToMemory(const SVulkanBase &vulkanInstance, VulkanBaseMemory &memory, VkDeviceSize size, MemoryType memType);
		void* BindBufferToMemory(const SVulkanBase &vulkanInstance, VulkanChunk &memory, VkDeviceSize size, MemoryType memType);

		VulkanChunk mLocalChunk;
		VulkanChunk mSharedChunk;
	};
}

#endif // VULKAN_BASE_MEMORY_MANAGER_H
