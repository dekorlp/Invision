#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	// Binding Description
	class  VulkanVertexBuffer
	{
	public:
		INVISION_API VulkanVertexBuffer();
		INVISION_API VulkanVertexBuffer& CreateVertexInputDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API VulkanVertexBuffer& VulkanVertexBuffer::CreateAttributeDescription(uint32_t location, VkFormat format, uint32_t offset);
		INVISION_API std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		INVISION_API VkVertexInputBindingDescription GetBindingDescription();
		
		
	private:
		unsigned int mBinding;
		VkVertexInputBindingDescription mVertexBindingDescription;
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		bool mExistsVkVertexInputBindingDescription;

	};
}
#endif // VULKAN_VERTEX_BUFFER_H