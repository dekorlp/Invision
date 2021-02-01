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
		mAllocLocalMemory.Init(((size / pageSize) + 1) * (sizeof(VulkanBaseBuffer2) + mAllocLocalMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));
		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, mLocalMemory);
		for (int i = 0; i < ((size / pageSize) + 1); i++)
		{
			VulkanBaseBuffer2* buffer = (VulkanBaseBuffer2*)mAllocLocalMemory.Allocate();
			buffer->inUse = false;
			buffer->mSize = 0;
			buffer->mBuffer = VK_NULL_HANDLE;
			buffer->mOffset = ((size / pageSize) + 1) * i;
			
		}
	

		// Allocate shared Memory
		uint32_t sizeShared = 512 * 1024 * 1024;
		mAllocSharedMemory.Init((sizeShared / pageSize + 1) * (sizeof(VulkanBaseBuffer2) + mAllocLocalMemory.GetLayoutSize()), sizeof(VulkanBaseBuffer2));
		AllocateMemory(vulkanInstance, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeShared, mSharedMemory);
		
		void* start;

		for (int i = 0; i < ((sizeShared / pageSize) + 1); i++)
		{
			VulkanBaseBuffer2* buffer = (VulkanBaseBuffer2*)mAllocSharedMemory.Allocate();
			if (i == 0)
			{
				start = buffer;
			}
			buffer->inUse = false;
			buffer->mSize = 0;
			buffer->mBuffer = VK_NULL_HANDLE;
			buffer->mOffset = ((sizeShared / pageSize) + 1) * i;
		}
		
		VulkanBaseBuffer2* test = reinterpret_cast<VulkanBaseBuffer2*>(MemoryBlock::GetPoolHeader(start)->next);
		VulkanBaseBuffer2* test2 = reinterpret_cast<VulkanBaseBuffer2*>(MemoryBlock::GetPoolHeader(test)->next);
	}
	

	void VulkanBaseMemoryManager::BindToSharedMemory(const SVulkanBase &vulkanInstance, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkDeviceSize memoryOffset)
	{
		VkBuffer* buffer;
		CreateBuffer(vulkanInstance, *buffer, mSharedMemory, size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode, memoryOffset);
	}

	void VulkanBaseMemoryManager::Destroy(const SVulkanBase &vulkanInstance)
	{
		vkFreeMemory(vulkanInstance.logicalDevice, mLocalMemory, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mSharedMemory, nullptr);
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

		//allocate Vertex Buffer Memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanInstance.logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Invision::findMemoryType(vulkanInstance.physicalDeviceStruct.physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(vulkanInstance.logicalDevice, buffer, memory, memoryOffset);
	}


}