#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"

#include "VulkanVertexBuffer.h"

namespace Invision
{
	VulkanVertexBuffer::VulkanVertexBuffer()
	{
		mExistsVkVertexInputBindingDescription = false;
	}

	VulkanVertexBuffer& VulkanVertexBuffer::CreateVertexBuffer(const SVulkan &vulkanInstance, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, const void* source, VkDeviceSize offset)
	{
		mOffset = offset;
		// create Vertex Buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;
		
		if (vkCreateBuffer(vulkanInstance.logicalDevice, &bufferInfo, nullptr, &mVertexBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		//allocate Vertex Buffer Memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanInstance.logicalDevice, mVertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Invision::findMemoryType(vulkanInstance.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(vulkanInstance.logicalDevice, &allocInfo, nullptr, &mVertexBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(vulkanInstance.logicalDevice, mVertexBuffer, mVertexBufferMemory, 0);

		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, mVertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, source, (size_t)bufferInfo.size);
		vkUnmapMemory(vulkanInstance.logicalDevice, mVertexBufferMemory);



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
		return mVertexBuffer;
	}

	VkDeviceSize VulkanVertexBuffer::GetOffset()
	{
		return mOffset;
	}

	void VulkanVertexBuffer::DestroyVertexBuffer(const SVulkan &vulkanInstance)
	{
		vkDestroyBuffer(vulkanInstance.logicalDevice, mVertexBuffer, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mVertexBufferMemory, nullptr);
	}
}