#include "precompiled.h"
#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"


namespace Invision
{
	VulkanBaseMemoryManager::VulkanBaseMemoryManager()
	{
		/*// Calculate Memory size
		uint32_t sizeExperimental = 2 * 1024 * 1024; // 2 GB
		uint32_t pageSize = 1024 * 10;

		mAlloc.Init(sizeExperimental / pageSize, sizeof(VulkanBaseBuffer2));*/
		
		/*uint32* Pol = (uint32*)mAlloc.Allocate();
		*Pol = 1024;

		uint32* Pol1 = (uint32*)mAlloc.Allocate();
		*Pol1 = 1024;
		uint32* Pol2 = (uint32*)mAlloc.Allocate();
		*Pol2 = 2048;
		uint32* Pol3 = (uint32*)mAlloc.Allocate();
		*Pol3 = 3000;
		uint32* Pol4 = (uint32*)mAlloc.Allocate();
		*Pol4 = 4000;
		uint32* Pol5 = (uint32*)mAlloc.Allocate();
		*Pol5 = 5000;
		uint32* Pol6 = (uint32*)mAlloc.Allocate();
		*Pol6 = 7000;
		mAlloc.Deallocate(Pol2);
		mAlloc.Deallocate(Pol3);
		uint32* Pol7 = (uint32*)mAlloc.Allocate();
		*Pol7 = 8000;
		uint32* Pol8 = (uint32*)mAlloc.Allocate();
		*Pol8 = 9000;


		mAlloc.Destroy();*/
	}

	void VulkanBaseMemoryManager::Init(const SVulkanBase &vulkanInstance, uint32_t size)
	{
		// Allocate dedicated Memory
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * 10);
		mLocalMemory.mMappedMemory.Init(((size / pageSize) + 1) * (sizeof(VulkanBaseBuffer2) + mLocalMemory.mMappedMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));

		for (int i = 0; i < ((size / pageSize) + 1); i++)
		{
			void* actualLocalMemPosition = mLocalMemory.mMappedMemory.Allocate();

			((VulkanBaseBuffer2*)(actualLocalMemPosition))->inUse = false;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mSize = 0;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mBuffer = VK_NULL_HANDLE;
			((VulkanBaseBuffer2*)(actualLocalMemPosition))->mOffset = pageSize * i;

			if (i == 0)
			{
				mLocalMemory.mStartPosition = &actualLocalMemPosition;
			}
		}
		
		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, mLocalMemory.mMemory);

		// Allocate shared Memory
		uint32_t sizeShared = 512 * 1024 * 1024;
		mSharedMemory.mMappedMemory.Init(((sizeShared / pageSize) + 1) * (sizeof(VulkanBaseBuffer2) + mSharedMemory.mMappedMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));

		

		for (int i = 0; i < ((sizeShared / pageSize) + 1); i++)
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
		
		//VulkanBaseBuffer2* test = reinterpret_cast<VulkanBaseBuffer2*>(mSharedMemory.mStartPosition);
		//VulkanBaseBuffer2* test2 = reinterpret_cast<VulkanBaseBuffer2*>(MemoryBlock::GetPoolHeader(test)->next);
		
		/*void* currPos = mLocalMemory.mStartPosition;
		while(currPos != nullptr)
		{
			VulkanBaseBuffer2* test2 = reinterpret_cast<VulkanBaseBuffer2*>(currPos);
			
			if (MemoryBlock::GetPoolHeader(currPos)->next == nullptr)
			{
				int test = 0;
			}

			currPos = MemoryBlock::GetPoolHeader(currPos)->next;
		};*/
	}
	

	void* VulkanBaseMemoryManager::BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
	{
		uint32_t pageSize = static_cast<uint32_t>(vulkanInstance.physicalDeviceStruct.deviceProperties.limits.bufferImageGranularity * 10);
		unsigned int countOfPages = ((size / pageSize) + 1);
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
				}
				else
				{
					selectedPage->mAllocatedPages = 0;
					selectedPage->inUse = false;
					selectedPage = nullptr;
				}

				if (iterator == countOfPages)
				{
					break;
				}

				iterator++;
			}

			//if (MemoryBlock::GetPoolHeader(currPos)->next == nullptr)
			//{
			//	int test = 0;
			//}

			currPos = MemoryBlock::GetPoolHeader(currPos)->next;
		};

		// create Vulkan Buffer
		CreateBuffer(vulkanInstance, selectedPage->mBuffer, mSharedMemory.mMemory, size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode, selectedPage->mOffset);

		return selectedPage;
	}

	void VulkanBaseMemoryManager::Unbind(const SVulkanBase &vulkanInstance, void* memory)
	{
		// free allocated pages
		unsigned int countOfPages = 0;

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

	void VulkanBaseMemoryManager::CopyDataToMemory(const SVulkanBase &vulkanInstance, void* memory, const void* data)
	{
		VkDeviceMemory deviceMemory = VK_NULL_HANDLE;

		if (((VulkanBaseBuffer2*)(memory))->mMemType == MEMPORY_TYPE_DEDICATED)
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

	void VulkanBaseMemoryManager::AllocateMemory(const SVulkanBase &vulkanInstance, VkMemoryPropertyFlags properties, uint32_t size, VkDeviceMemory &memory)
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

		/*//allocate Vertex Buffer Memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanInstance.logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Invision::findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}*/

		vkBindBufferMemory(vulkanInstance.logicalDevice, buffer, memory, memoryOffset);
	}


}