#include "precompiled.h"

#include "InVulkan.h"
#include "VulkanException.h"


#include "VulkanVertexBuffer.h"

namespace Invision
{
	VulkanVertexBuffer::VulkanVertexBuffer()
	{
		mExistsVkVertexInputBindingDescription = false;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateVertexBuffer(const SVulkan &vulkanInstance, VulkanCommandPool commandPool, VkDeviceSize size, const void* source, VkDeviceSize offset)
	{
		mOffset = offset;

		VulkanBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE);

		
		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, (size_t)size);
		vkUnmapMemory(vulkanInstance.logicalDevice, stagingBuffer.GetDeviceMemory());

		mVertexBuffer.CreateBuffer(vulkanInstance, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);

		stagingBuffer.CopyBuffer(vulkanInstance, commandPool, mVertexBuffer, 0, 0, size);

		stagingBuffer.DestroyBuffer(vulkanInstance);

		return *this;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateVertexInputDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
	{
		mBinding = binding;
		mVertexBindingDescription = {};
		mVertexBindingDescription.binding = binding;
		mVertexBindingDescription.stride = stride;
		mVertexBindingDescription.inputRate = inputRate;
		mExistsVkVertexInputBindingDescription = true;
		return *this;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateAttributeDescription(uint32_t location, VkFormat format, uint32_t offset)
	{
		if (!mExistsVkVertexInputBindingDescription)
		{
			throw VulkanException("There is no existing VertexInputBindingDescription!");
		}

		VkVertexInputAttributeDescription attr = {};
		attr.binding = mBinding;
		attr.format = format;
		attr.location = location;
		attr.offset = offset;

		mAttributeDescriptions.push_back(attr);

		return *this;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanVertexBuffer::GetAttributeDescriptions()
	{
		return mAttributeDescriptions;
	}

	VkVertexInputBindingDescription VulkanVertexBuffer::GetBindingDescription()
	{
		return mVertexBindingDescription;
	}

	VkBuffer VulkanVertexBuffer::GetBuffer()
	{
		return mVertexBuffer.GetBuffer();
	}

	VkDeviceSize VulkanVertexBuffer::GetOffset()
	{
		return mOffset;
	}

	void VulkanVertexBuffer::DestroyVertexBuffer(const SVulkan &vulkanInstance)
	{
		mVertexBuffer.DestroyBuffer(vulkanInstance);
	}
}