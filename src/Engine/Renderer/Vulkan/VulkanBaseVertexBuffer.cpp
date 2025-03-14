#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"
#include "VulkanBaseCommandPool.h"

#include "VulkanBaseVertexBuffer.h"

namespace Invision
{

	VulkanBaseVertexBuffer::VulkanBaseVertexBuffer()
	{

	}

	void* VulkanBaseVertexBuffer::AllocateDedicatedMemory(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source)
	{
		

		void* stagingBuffer = memoryManager.BindToSharedMemory(vulkanContext, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);

		VulkanAllocation alloc = ((Invision::LinkedListNode<VulkanAllocation>*)(stagingBuffer))->mData;

		memoryManager.CopyDataToBuffer(vulkanContext, stagingBuffer, source);
		void* vertexBuffer = memoryManager.BindToDedicatedMemory(vulkanContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		VulkanAllocation allocvrt = ((Invision::LinkedListNode<VulkanAllocation>*)(vertexBuffer))->mData;

		memoryManager.CopyBufferToBuffer(vulkanContext, commandPool, stagingBuffer, vertexBuffer);
		memoryManager.Unbind(vulkanContext, stagingBuffer);

		return vertexBuffer;
	}

	void* VulkanBaseVertexBuffer::AllocateSharedMemory(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source)
	{
		void* vertexBuffer = memoryManager.BindToSharedMemory(vulkanContext, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanContext, vertexBuffer, source);

		return vertexBuffer;
	}


	void VulkanBaseVertexBuffer::CreateBuffer(const SVulkanContext &vulkanContext, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint32_t binding, uint64_t size, const void *source)
	{
		mVertexBuffers.push_back(AllocateDedicatedMemory(vulkanContext, commandPool, memoryManager, size, source));
	}

	std::vector<void*>& VulkanBaseVertexBuffer::GetBuffers()
	{
		return mVertexBuffers;
	}

	void VulkanBaseVertexBuffer::DestroyVertexBuffers(const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager)
	{
		for (int  i = 0; i < mVertexBuffers.size(); i++)
		{

			memoryManager.Unbind(vulkanContext, mVertexBuffers[i]);
		}
		mVertexBuffers.clear();
	}

	VulkanBaseVertexBinding::VulkanBaseVertexBinding()
	{

	}

	VulkanBaseBindingDescription& VulkanBaseVertexBinding::CreateBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
	{
		VulkanBaseBindingDescription description(mAttributeDescriptions, mBindingDescriptions, binding, stride, inputRate);
		mBaseBindingDescriptions.push_back(description);

		return mBaseBindingDescriptions.at(mBaseBindingDescriptions.size() - 1);
	}

	std::vector<VkVertexInputAttributeDescription>& VulkanBaseVertexBinding::GetAttributeDescriptions()
	{
		return mAttributeDescriptions;
	}

	std::vector<VkVertexInputBindingDescription>& VulkanBaseVertexBinding::GetBindingDescriptions()
	{
		return mBindingDescriptions;
	}


	VulkanBaseBindingDescription::VulkanBaseBindingDescription()
	{

	}

	VulkanBaseBindingDescription::VulkanBaseBindingDescription( std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,	std::vector<VkVertexInputBindingDescription> &bindingDescriptions, uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
	{

		mBindingDescription.binding = binding;
		mBindingDescription.inputRate = inputRate;
		mBindingDescription.stride = stride;

		mAttributeDescriptions = &attributeDescriptions;
		//if (inputRate == VK_VERTEX_INPUT_RATE_VERTEX)
		//{
			//AllocateDedicatedMemory(vulkanInstance, commandPool, memoryManager, size, source);
		//}
		//else // inputRate == VK_VERTEX_INPUT_RATE_INSTANCE
		//{
			//AllocateSharedMemory(vulkanInstance, commandPool, memoryManager, size, source);
		//}

		bindingDescriptions.push_back(mBindingDescription);
		//vertexBuffers.push_back(mVertexBuffer);

	
	}

	VkVertexInputBindingDescription VulkanBaseBindingDescription::GetBindingDescription()
	{
		return mBindingDescription;
	}

	void VulkanBaseBindingDescription::CreateAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
	{
		VulkanBaseAttributeDescription description(attributeDescriptions, binding, location, format, offset);		
	}

	VulkanBaseAttributeDescription::VulkanBaseAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
	{
		mAttributeDescription.binding = binding;
		mAttributeDescription.format = format;
		mAttributeDescription.location = location;
		mAttributeDescription.offset = offset;

		attributeDescriptions.push_back(mAttributeDescription);
	}
}