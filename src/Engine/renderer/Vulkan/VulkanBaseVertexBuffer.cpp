#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"


#include "VulkanBaseVertexBuffer.h"

namespace Invision
{

	VulkanBaseVertexBuffer::VulkanBaseVertexBuffer()
	{

	}

	VulkanBaseBindingDescription& VulkanBaseVertexBuffer::CreateBinding(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, uint64_t size, const void *source, uint64_t offset, uint32_t stride, VkVertexInputRate inputRate)
	{
		VulkanBaseBindingDescription description(vulkanInstance, commandPool, mAttributeDescriptions, mBindingDescriptions,  mVertexBuffers, maxAllocatedBinding, size, source, offset, stride, inputRate);
		maxAllocatedBinding++;
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

	std::vector<VulkanBaseBuffer>& VulkanBaseVertexBuffer::GetBuffers()
	{
		return mVertexBuffers;
	}

	uint64_t VulkanBaseVertexBuffer::GetOffset(uint32_t binding)
	{
		return mBaseBindingDescriptions[binding].GetOffset();
	}

	void VulkanBaseVertexBuffer::DestroyVertexBuffers(const SVulkanBase &vulkanInstance)
	{
		for (int  i = 0; i < mVertexBuffers.size(); i++)
		{
			mVertexBuffers[i].DestroyBuffer(vulkanInstance);
		}
		mVertexBuffers.clear();
	}


	VulkanBaseBindingDescription::VulkanBaseBindingDescription()
	{

	}

	VulkanBaseBindingDescription::VulkanBaseBindingDescription(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,	std::vector<VkVertexInputBindingDescription> &bindingDescriptions, std::vector<VulkanBaseBuffer> &vertexBuffers, uint32_t binding, uint64_t size, const void *source, uint64_t offset, uint32_t stride, VkVertexInputRate inputRate)
	{

		mBindingDescription.binding = binding;
		mBindingDescription.inputRate = inputRate;
		mBindingDescription.stride = stride;

		mOffset = offset;
		mAttributeDescriptions = &attributeDescriptions;
		AllocateMemory(vulkanInstance, commandPool, size, source, offset);

		bindingDescriptions.push_back(mBindingDescription);
		vertexBuffers.push_back(mBuffer);

	
	}

	VkVertexInputBindingDescription VulkanBaseBindingDescription::GetBindingDescription()
	{
		return mBindingDescription;
	}

	uint64_t VulkanBaseBindingDescription::GetOffset()
	{
		return mOffset;
	}

	void VulkanBaseBindingDescription::AllocateMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, uint64_t size, const void *source, uint64_t offset)
	{
		VulkanBaseBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE);


		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, (size_t)size);
		vkUnmapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory());

		mBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);

		stagingBuffer.CopyBuffer(vulkanInstance, commandPool, mBuffer, 0, 0, size);

		stagingBuffer.DestroyBuffer(vulkanInstance);

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