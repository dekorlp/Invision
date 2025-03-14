#ifndef VULKAN_BASE_MEMORY_MANAGER_H
#define VULKAN_BASE_MEMORY_MANAGER_H

#include "vulkan\vulkan.h"
#include "allocator\MemoryBlock.h"
#include "allocator\PoolAllocator.h"
#include "common/Types/DoubleLinkedList.h"
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
		unsigned int mPageIndex;
		VkDeviceSize mSize;
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
		uint32_t mPageSize;
		size_t mTotalAllocatedSize;
		size_t mAllocatedSize;
	};

#define PAGESIZE 10

	class  VulkanBaseMemoryManager
	{
	public:
		VulkanBaseMemoryManager();
		void Init(const SVulkanBase &vulkanInstance, const SVulkanContext& vulkanContext, size_t size);
		void Destroy(const SVulkanContext& vulkanContext);
		void* BindToSharedMemory(const SVulkanContext& vulkanContext, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void* BindToDedicatedMemory(const SVulkanContext& vulkanContext, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		void* BindImageToDedicatedMemory(const SVulkanContext& vulkanContext, VkImage &image, VkDeviceSize size);
		void Unbind(const SVulkanContext& vulkanContext, void* memory);
		void CopyDataToBuffer(const SVulkanContext& vulkanContext, void* memory, const void* data);
		void CopyBufferToBuffer(const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, void* src, void* dest);
		void CopyBufferToImage(const SVulkanContext& vulkanContext, VulkanBaseCommandPool commandPool, void* src, VkImage& image, uint32_t baseArrayLayer, uint32_t width, uint32_t height);

		static VkCommandBuffer BeginSingleTimeCommands(const SVulkanContext& vulkanContext, VulkanBaseCommandPool &commandPool);
		static void EndSingleTimeCommands(const SVulkanContext& vulkanContext, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer);

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
		void AllocateMemory(const SVulkanBase& vulkanInstance, const SVulkanContext& vulkanContext, VkMemoryPropertyFlags properties, size_t size, VkDeviceMemory &memory);
		void CreateBuffer(const SVulkanContext& vulkanContext, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset);

		//void* BindBufferToMemory(const SVulkanBase &vulkanInstance, VulkanBaseMemory &memory, VkDeviceSize size, MemoryType memType);
		void* BindBufferToMemory(VulkanChunk &memory, VkDeviceSize size, MemoryType memType);

		VulkanChunk mLocalChunk;
		VulkanChunk mSharedChunk;
	};
}

#endif // VULKAN_BASE_MEMORY_MANAGER_H
