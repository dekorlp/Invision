#include "precompiled.h"

#include "VulkanVertexBuffer.h"

namespace Invision
{
	VulkanVertexBuffer::VulkanVertexBuffer()
	{
		mExistsVkVertexInputBindingDescription = false;
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
}