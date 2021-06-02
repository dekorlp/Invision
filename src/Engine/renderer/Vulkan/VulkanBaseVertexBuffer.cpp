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

	VulkanBaseBindingDescription& VulkanBaseVertexBuffer::CreateBinding(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint32_t binding, uint64_t size, const void *source, uint32_t stride, VkVertexInputRate inputRate)
	{
		mMemoryManager = &memoryManager;
		VulkanBaseBindingDescription description(vulkanInstance, commandPool, memoryManager, mAttributeDescriptions, mBindingDescriptions,  mVertexBuffers, binding, size, source, stride, inputRate);
		mBaseBindingDescriptions.push_back(description);

		return mBaseBindingDescriptions.at(mBaseBindingDescriptions.size() - 1);
	}

	std::vector<VkVertexInputAttributeDescription>& VulkanBaseVertexBuffer::GetAttributeDescriptions()
	{
		return mAttributeDescriptions;
	}

	std::vector<VkVertexInputBindingDescription>& VulkanBaseVertexBuffer::GetBindingDescriptions()
	{
		return mBindingDescriptions;
	}

	std::vector<void*>& VulkanBaseVertexBuffer::GetBuffers()
	{
		return mVertexBuffers;
	}

	void VulkanBaseVertexBuffer::DestroyVertexBuffers(const SVulkanBase &vulkanInstance)
	{
		for (int  i = 0; i < mVertexBuffers.size(); i++)
		{

			mMemoryManager->Unbind(vulkanInstance, mVertexBuffers[i]);
		}
		mVertexBuffers.clear();
	}


	VulkanBaseBindingDescription::VulkanBaseBindingDescription()
	{

	}

	VulkanBaseBindingDescription::VulkanBaseBindingDescription(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,	std::vector<VkVertexInputBindingDescription> &bindingDescriptions, std::vector<void*> &vertexBuffers, uint32_t binding, uint64_t size, const void *source, uint32_t stride, VkVertexInputRate inputRate)
	{

		mBindingDescription.binding = binding;
		mBindingDescription.inputRate = inputRate;
		mBindingDescription.stride = stride;

		mAttributeDescriptions = &attributeDescriptions;
		//if (inputRate == VK_VERTEX_INPUT_RATE_VERTEX)
		//{
			AllocateDedicatedMemory(vulkanInstance, commandPool, memoryManager, size, source);
		//}
		//else // inputRate == VK_VERTEX_INPUT_RATE_INSTANCE
		//{
			//AllocateSharedMemory(vulkanInstance, commandPool, memoryManager, size, source);
		//}

		bindingDescriptions.push_back(mBindingDescription);
		vertexBuffers.push_back(mVertexBuffer);

	
	}

	VkVertexInputBindingDescription VulkanBaseBindingDescription::GetBindingDescription()
	{
		return mBindingDescription;
	}

	void VulkanBaseBindingDescription::AllocateDedicatedMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source)
	{
		void* stagingBuffer = memoryManager.BindToSharedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanInstance, stagingBuffer, source);
		mVertexBuffer = memoryManager.BindToDedicatedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyBufferToBuffer(vulkanInstance, commandPool, stagingBuffer, mVertexBuffer);
		memoryManager.Unbind(vulkanInstance, stagingBuffer);
	}

	void VulkanBaseBindingDescription::AllocateSharedMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source)
	{
		mVertexBuffer = memoryManager.BindToSharedMemory(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		memoryManager.CopyDataToBuffer(vulkanInstance, mVertexBuffer, source);
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