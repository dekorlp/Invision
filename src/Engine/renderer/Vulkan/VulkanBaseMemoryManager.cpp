#include "precompiled.h"
#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"


namespace Invision
{
	VulkanBaseMemoryManager::VulkanBaseMemoryManager()
	{

	}

	void VulkanBaseMemoryManager::Init(const SVulkanBase &vulkanInstance, size_t size)
	{
		// Allocate dedicated Memory
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE);
		
		for (unsigned int i = 0; i < ((size / pageSize) + 1); i++)
		{
			VulkanPage page;
			page.mInUse = false;

			mLocalChunk.mPages.push_back(page);
		}

		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, mLocalChunk.mMemory);


		// Allocate shared Memory
		uint32_t sizeShared = 512 * 1024 * 1024;

		for (unsigned int i = 0; i < ((sizeShared / pageSize) + 1); i++)
		{
			VulkanPage page;
			page.mInUse = false;

			mSharedChunk.mPages.push_back(page);
		}

		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, size, mSharedChunk.mMemory);
	}
	

	void* VulkanBaseMemoryManager::BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		void* selectedPage = BindBufferToMemory(vulkanInstance, mSharedChunk, size, MEMORY_TYPE_SHARED);

	
		CreateBuffer(vulkanInstance, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mBuffer, mSharedChunk.mMemory, size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.pageIndex * (vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE));
	
		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindToDedicatedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		void* selectedPage = BindBufferToMemory(vulkanInstance, mLocalChunk, size, MEMORY_TYPE_DEDICATED);


		CreateBuffer(vulkanInstance, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.mBuffer,
			mLocalChunk.mMemory, size, usage, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sharingMode, ((Invision::LinkedListNode<VulkanAllocation>*)(selectedPage))->mData.pageIndex * (vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE));

		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindImageToDedicatedMemory(const SVulkanBase &vulkanInstance, VkImage &image, VkDeviceSize size)
	{
		void* selectedPage = BindBufferToMemory(vulkanInstance, mLocalChunk, size, MEMORY_TYPE_DEDICATED);

		VulkanAllocation allocPage = ((Invision::LinkedListNode<VulkanAllocation>*)(BindBufferToMemory(vulkanInstance, mLocalChunk, size, MEMORY_TYPE_DEDICATED)))->mData;

		vkBindImageMemory(vulkanInstance.logicalDevice, image, mLocalChunk.mMemory, allocPage.pageIndex * (vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE));
		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindBufferToMemory(const SVulkanBase &vulkanInstance, VulkanChunk &memory, VkDeviceSize size, MemoryType memType)
	{
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE);
		VkDeviceSize countOfPages = ((size / pageSize) + 1);

		unsigned int indexOfPage = 0;
		bool found;
		unsigned int iterator = 0;

		for (unsigned int i = 0; i < memory.mPages.size(); i++)
		{
			if (memory.mPages[i].mInUse == false)
			{
				//page is unused
				found = true;
				if (iterator == 0)
				{
					indexOfPage = i;
				}

				if (iterator == countOfPages)
				{
					break;
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
			memory.mPages[i].mInUse = true;
			if (i - indexOfPage == countOfPages)
			{
				break;
			}
		}

		VulkanAllocation alloc;
		alloc.mBuffer = VK_NULL_HANDLE;
		alloc.mBufferOffset = 0;
		alloc.pageIndex = indexOfPage;
		alloc.mMemType = memType;
		alloc.size = size;

		if (size > 10000000)
		{
			int test = 0;
		}

		return memory.mAllocations.pushBack(alloc);
	}

	void VulkanBaseMemoryManager::CopyBufferToBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, void* dest)
	{
		// check size
		if (((Invision::LinkedListNode<VulkanAllocation>*)(dest))->mData.size < ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.size)
		{
			throw std::runtime_error("Destination size is smaller than source size!");
		}


		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(vulkanInstance, commandPool);

		VkBufferCopy copyRegion{};
		copyRegion.size = ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.size;
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		vkCmdCopyBuffer(commandBuffer, ((Invision::LinkedListNode<VulkanAllocation>*)(src))->mData.mBuffer, ((Invision::LinkedListNode<VulkanAllocation>*)(dest))->mData.mBuffer, 1, &copyRegion);

		EndSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
	}

	void VulkanBaseMemoryManager::CopyBufferToImage(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, VkImage& image, uint32_t baseArrayLayer, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(vulkanInstance, commandPool);

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

		EndSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
	}

	void VulkanBaseMemoryManager::Unbind(const SVulkanBase &vulkanInstance, void* memory)
	{

		if (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(vulkanInstance.logicalDevice, ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer, nullptr);
			((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mBuffer = VK_NULL_HANDLE;
		}

		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE);
		VkDeviceSize countOfPages = ((((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.size / pageSize));

		if (((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.mMemType == MEMORY_TYPE_DEDICATED)
		{
			for (unsigned int i = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.pageIndex; i < mLocalChunk.mPages.size(); i++)
			{
				//mLocalChunk.mPages[i].mInUse = false;

				//if (i == countOfPages)
				//{
				//	break;
				//}
			}

			//mLocalChunk.mAllocations.remove(memory);

		}
		else
		{
			for (unsigned int i = ((Invision::LinkedListNode<VulkanAllocation>*)(memory))->mData.pageIndex; i < mLocalChunk.mPages.size(); i++)
			{
				//mSharedChunk.mPages[i].mInUse = false;

				//if (i == countOfPages)
				//{
				//	break;
				//}
			}

			//mSharedChunk.mAllocations.remove(memory);
		}
	}

	void VulkanBaseMemoryManager::CopyDataToBuffer(const SVulkanBase &vulkanInstance, void* memory, const void* data)
	{
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
		vkMapMemory(vulkanInstance.logicalDevice, deviceMemory, allocPage.pageIndex * vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * PAGESIZE, allocPage.size, 0, &dataUnused);
		memcpy(dataUnused, data, allocPage.size);
		vkUnmapMemory(vulkanInstance.logicalDevice, deviceMemory);
	}

	void VulkanBaseMemoryManager::Destroy(const SVulkanBase &vulkanInstance)
	{
		vkFreeMemory(vulkanInstance.logicalDevice, mLocalChunk.mMemory, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mSharedChunk.mMemory, nullptr);
	}

	void VulkanBaseMemoryManager::AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, size_t size, VkDeviceMemory &memory)
	{
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = size;
		allocInfo.memoryTypeIndex = findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, properties); // Device Memory = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | shared Memory = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
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

	void VulkanBaseMemoryManager::CreateBuffer(const SVulkanBase &vulkanInstance, VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode, VkDeviceSize memoryOffset)
	{
		// create Vertex Buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;

		if (vkCreateBuffer(vulkanInstance.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		vkBindBufferMemory(vulkanInstance.logicalDevice, buffer, memory, memoryOffset);
	}

	VkCommandBuffer VulkanBaseMemoryManager::BeginSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool.GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vulkanInstance.logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanBaseMemoryManager::EndSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vulkanInstance.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkanInstance.graphicsQueue);

		vkFreeCommandBuffers(vulkanInstance.logicalDevice, commandPool.GetCommandPool(), 1, &commandBuffer);
	}

}