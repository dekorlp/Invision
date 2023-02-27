#include "precompiled.h"
#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"


namespace Invision
{
	VulkanBaseMemoryManager::VulkanBaseMemoryManager()
	{

	}

	void VulkanBaseMemoryManager::Init(const SVulkanBase &vulkanInstance, const SVulkanContext& vulkanContext, size_t size)
	{
		// Allocate dedicated Memory
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE);
		
		for (unsigned int i = 0; i < ((size / pageSize) + 1); i++)
		{
			VulkanPage page;
			page.mInUse = false;

			mLocalChunk.mPages.push_back(page);
		}

		mLocalChunk.mPageSize = pageSize;
		mLocalChunk.mMemType = MEMORY_TYPE_DEDICATED;
		mLocalChunk.mTotalAllocatedSize = size;
		mLocalChunk.mAllocatedSize = 0;
		AllocateMemory(vulkanInstance, vulkanContext, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, mLocalChunk.mMemory);


		// Allocate shared Memory
		size_t sizeShared = 512 * 1024 * 1024;

		for (unsigned int i = 0; i < ((sizeShared / pageSize) + 1); i++)
		{
			VulkanPage page;
			page.mInUse = false;

			mSharedChunk.mPages.push_back(page);
		}

		mSharedChunk.mPageSize = pageSize;
		mSharedChunk.mMemType = MEMORY_TYPE_SHARED;
		mSharedChunk.mTotalAllocatedSize = sizeShared;
		mSharedChunk.mAllocatedSize = 0;
		AllocateMemory(vulkanInstance, vulkanContext, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, size, mSharedChunk.mMemory);
	}
	

	void* VulkanBaseMemoryManager::BindToSharedMemory(const SVulkanContext &vulkanContext, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		void* selectedPage = BindBufferToMemory(mSharedChunk, size, MEMORY_TYPE_SHARED);

		uint32_t pageSize = (((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;
	
		CreateBuffer(vulkanContext, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mBuffer, mSharedChunk.mMemory, size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mPageIndex * pageSize);
	
		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindToDedicatedMemory(const SVulkanContext& vulkanContext, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		void* selectedPage = BindBufferToMemory(mLocalChunk, size, MEMORY_TYPE_DEDICATED);

		uint32_t pageSize = (((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;

		CreateBuffer(vulkanContext, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mBuffer,
			mLocalChunk.mMemory, size, usage, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sharingMode, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mPageIndex * pageSize);

		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindImageToDedicatedMemory(const SVulkanContext &vulkanContext, VkImage &image, VkDeviceSize size)
	{
		void* selectedPage = BindBufferToMemory(mLocalChunk, size, MEMORY_TYPE_DEDICATED);

		VulkanAllocation allocPage = ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData;
		uint32_t pageSize = (allocPage.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;

		vkBindImageMemory(vulkanContext.logicalDevice, image, mLocalChunk.mMemory, allocPage.mPageIndex * pageSize);
		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindBufferToMemory(VulkanChunk &memory, VkDeviceSize size, MemoryType memType)
	{
		uint32_t pageSize = (memType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;
		VkDeviceSize countOfPages = ((size / pageSize) + 1);
		(memType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mAllocatedSize += countOfPages * pageSize : mSharedChunk.mAllocatedSize += countOfPages * pageSize;


		unsigned int indexOfPage = 0;
		bool found;
		unsigned int iterator = 0;

		for (unsigned int i = 0; i < memory.mPages.size(); i++)
		{
			if (iterator == countOfPages)
			{
				break;
			}

			if (memory.mPages[i].mInUse == false)
			{
				//page is unused
				found = true;
				if (iterator == 0)
				{
					indexOfPage = i;
				}

				iterator++;
			}
			else
			{
				found = false;
				iterator = 0;
				// page is in use
			}
		}

		for (unsigned int i = indexOfPage; i < memory.mPages.size(); i++)
		{
			if (i - indexOfPage == countOfPages)
			{
				break;
			}

			memory.mPages[i].mInUse = true;
		}

		VulkanAllocation alloc;
		alloc.mBuffer = VK_NULL_HANDLE;
		alloc.mBufferOffset = 0;
		alloc.mPageIndex = indexOfPage;
		alloc.mMemType = memType;
		alloc.mSize = size;

		return memory.mAllocations.pushBack(alloc);
	}

	void VulkanBaseMemoryManager::CopyBufferToBuffer(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, void* src, void* dest)
	{
		// check size
		if (((Invision::LinkedListNode<VulkanAllocation>*)(dest))->mData.mSize < ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.mSize)
		{
			throw std::runtime_error("Destination size is smaller than source size!");
		}


		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(vulkanContext, commandPool);

		VkBufferCopy copyRegion{};
		copyRegion.size = ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.mSize;
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		vkCmdCopyBuffer(commandBuffer, ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.mBuffer, ((Invision::LinkedListNode<VulkanAllocation>*)(dest))->mData.mBuffer, 1, &copyRegion);

		EndSingleTimeCommands(vulkanContext, commandPool, commandBuffer);
	}

	void VulkanBaseMemoryManager::CopyBufferToImage(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, void* src, VkImage& image, uint32_t baseArrayLayer, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(vulkanContext, commandPool);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = baseArrayLayer;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.mBuffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		EndSingleTimeCommands(vulkanContext, commandPool, commandBuffer);
	}

	void VulkanBaseMemoryManager::Unbind(const SVulkanContext &vulkanContext, void* memory)
	{

		if (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(vulkanContext.logicalDevice, ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer, nullptr);
			((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer = VK_NULL_HANDLE;
		}

		uint32_t pageSize = (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;
		VkDeviceSize countOfPages = ((((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mSize / pageSize) + 1);

		(((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mAllocatedSize -= countOfPages * pageSize : mSharedChunk.mAllocatedSize -= countOfPages * pageSize;

		VkDeviceSize endIt = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mPageIndex + countOfPages;

		if (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mMemType == MEMORY_TYPE_DEDICATED)
		{
			for (unsigned int i = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mPageIndex; i < endIt; i++)
			{
				if (i >= mLocalChunk.mPages.size())
				{
					throw std::out_of_range("Page Index is out of Range!");
				}

				mLocalChunk.mPages[i].mInUse = false;
			}

			mLocalChunk.mAllocations.remove(memory);

		}
		else // MEMORY_TYPE_SHARED
		{
			for (unsigned int i = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mPageIndex; i < endIt; i++)
			{
				if (i >= mSharedChunk.mPages.size())
				{
					throw std::out_of_range("Page Index is out of Range!");
				}

				mSharedChunk.mPages[i].mInUse = false;
			}

			mSharedChunk.mAllocations.remove(memory);
		}
	}

	void VulkanBaseMemoryManager::CopyDataToBuffer(const SVulkanContext &vulkanContext, void* memory, const void* data)
	{
		uint32_t pageSize = (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mMemType == MEMORY_TYPE_DEDICATED) ? mLocalChunk.mPageSize : mSharedChunk.mPageSize;

		VkDeviceMemory deviceMemory = VK_NULL_HANDLE;

		VulkanAllocation allocPage = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData;

		if (allocPage.mMemType == MEMORY_TYPE_DEDICATED)
		{
			deviceMemory = mLocalChunk.mMemory;
		}
		else // MEMORY_TYPE_SHARED
		{
			deviceMemory = mSharedChunk.mMemory;
		}

		void* dataUnused;
		vkMapMemory(vulkanContext.logicalDevice, deviceMemory, allocPage.mPageIndex * pageSize, allocPage.mSize, 0, &dataUnused);
		memcpy(dataUnused, data, allocPage.mSize);
		vkUnmapMemory(vulkanContext.logicalDevice, deviceMemory);
	}

	void VulkanBaseMemoryManager::Destroy(const SVulkanContext &vulkanContext)
	{
		vkFreeMemory(vulkanContext.logicalDevice, mLocalChunk.mMemory, nullptr);
		vkFreeMemory(vulkanContext.logicalDevice, mSharedChunk.mMemory, nullptr);
	}

	void VulkanBaseMemoryManager::AllocateMemory(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VkMemoryPropertyFlags properties, size_t size, VkDeviceMemory &memory)
	{
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, properties); // Device Memory = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | shared Memory = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		if (vkAllocateMemory(vulkanContext.logicalDevice, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
			throw VulkanBaseException("failed to allocate image memory!");
		}
	}

	uint32_t VulkanBaseMemoryManager::findMemoryType(const VkPhysicalDevice& device, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void VulkanBaseMemoryManager::CreateBuffer(const SVulkanContext &vulkanContext, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset)
	{
		// create Vertex Buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;

		if (vkCreateBuffer(vulkanContext.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		vkBindBufferMemory(vulkanContext.logicalDevice, buffer, memory, memoryOffset);
	}

	VkCommandBuffer VulkanBaseMemoryManager::BeginSingleTimeCommands(const SVulkanContext &vulkanContext, VulkanBaseCommandPool &commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool.GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vulkanContext.logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanBaseMemoryManager::EndSingleTimeCommands(const SVulkanContext &vulkanContext, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
	
		vkQueueSubmit(vulkanContext.queueFamilies[0].GetQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkanContext.queueFamilies[0].GetQueue());

		vkFreeCommandBuffers(vulkanContext.logicalDevice, commandPool.GetCommandPool(), 1, &commandBuffer);
	}

}