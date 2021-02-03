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
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * 10);
		mLocalMemory.mMappedMemory.Init(((size / pageSize) + 1) * (sizeof(VulkanBaseBuffer2) + mLocalMemory.mMappedMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));

		for (unsigned int i = 0; i < ((size / pageSize) + 1); i++)
		{
			void* actualLocalMemPosition = mLocalMemory.mMappedMemory.Allocate();

			((VulkanBaseBuffer2*)(actualLocalMemPosition))->inUse = false;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mSize = 0;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mBuffer = VK_NULL_HANDLE;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mAllocatedPages = 0;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mOffset = pageSize * i;

			if (i == 0)
			{
				mLocalMemory.mStartPosition = (void*)actualLocalMemPosition;
			}
		}
		
		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, mLocalMemory.mMemory);

		// Allocate shared Memory
		uint32_t sizeShared = 512 * 1024 * 1024;
		mSharedMemory.mMappedMemory.Init(((sizeShared / pageSize) + 1) * (sizeof(VulkanBaseBuffer2) + mSharedMemory.mMappedMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));

		

		for (unsigned int i = 0; i < ((sizeShared / pageSize) + 1); i++)
		{
			void* actualSharedMemPosition = mSharedMemory.mMappedMemory.Allocate();

			((VulkanBaseBuffer2*)(actualSharedMemPosition))->inUse = false;
			((VulkanBaseBuffer2*)(actualSharedMemPosition))->mSize = 0;
			((VulkanBaseBuffer2*)(actualSharedMemPosition))->mBuffer = VK_NULL_HANDLE;
			((VulkanBaseBuffer2*)(actualSharedMemPosition))->mAllocatedPages = 0;
			((VulkanBaseBuffer2*)(actualSharedMemPosition))->mOffset = pageSize * i;

			if (i == 0)
			{
				mSharedMemory.mStartPosition = (void*)actualSharedMemPosition;
			}
		}
		
		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeShared, mSharedMemory.mMemory);
	}
	

	void* VulkanBaseMemoryManager::BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * 10);
		VkDeviceSize countOfPages = ((size / pageSize) + 1);
		unsigned int iterator = 0;

		//
		VulkanBaseBuffer2 *selectedPage = nullptr;

		// find space in allocation table
		void* currPos = mSharedMemory.mStartPosition;
		while (currPos != nullptr)
		{
			if (iterator <= countOfPages)
			{
				//VulkanBaseBuffer2* buffer = reinterpret_cast<VulkanBaseBuffer2*>(currPos);
				if (((VulkanBaseBuffer2*)(currPos))->inUse == false)
				{
					if (selectedPage == nullptr)
					{

						((VulkanBaseBuffer2*)(currPos))->inUse = true;
						((VulkanBaseBuffer2*)(currPos))->mSize = size;
						((VulkanBaseBuffer2*)(currPos))->mAllocatedPages = countOfPages - 1;
						((VulkanBaseBuffer2*)(currPos))->mMemType = MEMORY_TYPE_SHARED;
						selectedPage = ((VulkanBaseBuffer2*)(currPos));
						
					}
					else
					{
						((VulkanBaseBuffer2*)(currPos))->inUse = true;
						((VulkanBaseBuffer2*)(currPos))->mSize = size;
						((VulkanBaseBuffer2*)(currPos))->mSize = MEMORY_TYPE_SHARED;
						
					}

					iterator++;
				}
				else
				{
					if (selectedPage != nullptr)
					{
						selectedPage->mAllocatedPages = 0;
						selectedPage->inUse = false;
						selectedPage = nullptr;
						iterator = 0;
					}
				}

				if (iterator == countOfPages)
				{
					break;
				}

				
			}

			currPos = MemoryBlock::GetPoolHeader(currPos)->next;
		};

		// create Vulkan Buffer
		CreateBuffer(vulkanInstance, selectedPage->mBuffer, mSharedMemory.mMemory, size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode, selectedPage->mOffset);

		return selectedPage;
	}

	void* VulkanBaseMemoryManager::BindToDedicatedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * 10);
		VkDeviceSize countOfPages = ((size / pageSize) + 1);
		unsigned int iterator = 0;

		//
		VulkanBaseBuffer2 *selectedPage = nullptr;

		// find space in allocation table
		void* currPos = mLocalMemory.mStartPosition;
		while (currPos != nullptr)
		{
			if (iterator <= countOfPages)
			{
				//VulkanBaseBuffer2* buffer = reinterpret_cast<VulkanBaseBuffer2*>(currPos);
				if (((VulkanBaseBuffer2*)(currPos))->inUse == false)
				{
					if (selectedPage == nullptr)
					{

						((VulkanBaseBuffer2*)(currPos))->inUse = true;
						((VulkanBaseBuffer2*)(currPos))->mSize = size;
						((VulkanBaseBuffer2*)(currPos))->mAllocatedPages = countOfPages - 1;
						((VulkanBaseBuffer2*)(currPos))->mMemType = MEMORY_TYPE_DEDICATED;
						((VulkanBaseBuffer2*)(currPos))->mBufferOffset = 0;
						selectedPage = ((VulkanBaseBuffer2*)(currPos));

					}
					else
					{
						((VulkanBaseBuffer2*)(currPos))->inUse = true;
						((VulkanBaseBuffer2*)(currPos))->mSize = size;
						((VulkanBaseBuffer2*)(currPos))->mSize = MEMORY_TYPE_DEDICATED;

					}

					iterator++;
				}
				else
				{
					if (selectedPage != nullptr)
					{
						selectedPage->mAllocatedPages = 0;
						selectedPage->inUse = false;
						selectedPage = nullptr;
						iterator = 0;
					}
				}

				if (iterator == countOfPages)
				{
					break;
				}

				
			}

			currPos = MemoryBlock::GetPoolHeader(currPos)->next;
		};

		// create Vulkan Buffer
		CreateBuffer(vulkanInstance, selectedPage->mBuffer, mLocalMemory.mMemory, size, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sharingMode, selectedPage->mOffset);

		return selectedPage;
	}

	void VulkanBaseMemoryManager::CopyBufferToBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, void* src, void* dest)
	{
		// check size
		if (((VulkanBaseBuffer2*)(dest))->mSize < ((VulkanBaseBuffer2*)(src))->mSize)
		{
			throw std::runtime_error("Destination size is smaller than source size!");
		}


		VkCommandBuffer commandBuffer = beginSingleTimeCommands(vulkanInstance, commandPool);

		VkBufferCopy copyRegion{};
		copyRegion.size = ((VulkanBaseBuffer2*)(src))->mSize;
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		vkCmdCopyBuffer(commandBuffer, ((VulkanBaseBuffer2*)(src))->mBuffer, ((VulkanBaseBuffer2*)(dest))->mBuffer, 1, &copyRegion);

		endSingleTimeCommands(vulkanInstance, commandPool, commandBuffer);
	}

	void VulkanBaseMemoryManager::Unbind(const SVulkanBase &vulkanInstance, void* memory)
	{
		// free allocated pages
		VkDeviceSize countOfPages = 0;

		((VulkanBaseBuffer2*)(memory))->inUse = false;
		countOfPages = ((VulkanBaseBuffer2*)(memory))->mAllocatedPages;

		unsigned int iterator = 0;
		void* currPos = memory;
		while (currPos != nullptr)
		{
			currPos = MemoryBlock::GetPoolHeader(currPos)->next;

			if (iterator <= countOfPages)
			{
				((VulkanBaseBuffer2*)(currPos))->inUse = false;
			}
			else
			{
				break;
			}

			iterator++;
		}

		// free buffer
		if (((VulkanBaseBuffer2*)(memory))->mBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(vulkanInstance.logicalDevice, ((VulkanBaseBuffer2*)(memory))->mBuffer, nullptr);
			((VulkanBaseBuffer2*)(memory))->mBuffer = VK_NULL_HANDLE;
		}
	}

	void VulkanBaseMemoryManager::CopyDataToBuffer(const SVulkanBase &vulkanInstance, void* memory, const void* data)
	{
		VkDeviceMemory deviceMemory = VK_NULL_HANDLE;

		if (((VulkanBaseBuffer2*)(memory))->mMemType == MEMORY_TYPE_DEDICATED)
		{
			deviceMemory = mLocalMemory.mMemory;
		}
		else // MEMORY_TYPE_SHARED
		{
			deviceMemory = mSharedMemory.mMemory;
		}

		void* dataUnused;
		vkMapMemory(vulkanInstance.logicalDevice, deviceMemory, ((VulkanBaseBuffer2*)(memory))->mOffset, ((VulkanBaseBuffer2*)(memory))->mSize, 0, &dataUnused);
		memcpy(dataUnused, data, (size_t)((VulkanBaseBuffer2*)(memory))->mSize);
		vkUnmapMemory(vulkanInstance.logicalDevice, deviceMemory);
	}

	void VulkanBaseMemoryManager::Destroy(const SVulkanBase &vulkanInstance)
	{
		vkFreeMemory(vulkanInstance.logicalDevice, mLocalMemory.mMemory, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mSharedMemory.mMemory, nullptr);
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

	VkCommandBuffer VulkanBaseMemoryManager::beginSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool)
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

	void VulkanBaseMemoryManager::endSingleTimeCommands(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, VkCommandBuffer &commandBuffer)
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