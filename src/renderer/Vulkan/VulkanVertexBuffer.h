#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H

#include "vulkan\vulkan.h"

namespace Invision
{
	// Binding Description
	class  VulkanVertexBuffer
	{
	public:
		INVISION_API VulkanVertexBuffer();
		INVISION_API VulkanVertexBuffer& CreateVertexBuffer(const SVulkan &vulkanInstance, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, const void* source, VkDeviceSize offset);
		INVISION_API VulkanVertexBuffer& CreateVertexInputDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API VulkanVertexBuffer& VulkanVertexBuffer::CreateAttributeDescription(uint32_t location, VkFormat format, uint32_t offset);
		INVISION_API std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		INVISION_API VkVertexInputBindingDescription GetBindingDescription();
		INVISION_API VkBuffer GetBuffer();
		INVISION_API VkDeviceSize GetOffset();

		INVISION_API void DestroyVertexBuffer(const SVulkan &vulkanInstance);
		
		
	private:
		// Vertex Buffer Creation (Memory)
		VkDeviceSize mOffset;
		VkBuffer mVertexBuffer;
		VkDeviceMemory mVertexBufferMemory;

		// Vertex Shader Binding Description 
		unsigned int mBinding;
		VkVertexInputBindingDescription mVertexBindingDescription;
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		bool mExistsVkVertexInputBindingDescription;

	};
}
#endif // VULKAN_VERTEX_BUFFER_H