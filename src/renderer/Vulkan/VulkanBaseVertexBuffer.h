#ifndef VULKAN_BASE_VERTEX_BUFFER_H
#define VULKAN_BASE_VERTEX_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBaseBuffer.h"

namespace Invision
{
	// Binding Description
	class  VulkanBaseVertexBuffer
	{
	public:
		INVISION_API VulkanBaseVertexBuffer();
		INVISION_API VulkanBaseVertexBuffer& CreateVertexBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VkDeviceSize size, const void* source, VkDeviceSize offset);
		INVISION_API VulkanBaseVertexBuffer& CreateVertexInputDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API VulkanBaseVertexBuffer& VulkanBaseVertexBuffer::CreateAttributeDescription(uint32_t location, VkFormat format, uint32_t offset);
		INVISION_API std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		INVISION_API VkVertexInputBindingDescription GetBindingDescription();
		INVISION_API VkBuffer GetBuffer();
		INVISION_API VkDeviceSize GetOffset();

		INVISION_API void DestroyVertexBuffer(const SVulkanBase &vulkanInstance);
		
		
	private:
		// Vertex Buffer Creation (Memory)
		VkDeviceSize mOffset;
		//VkBuffer mVertexBuffer;
		//VkDeviceMemory mVertexBufferMemory;

		VulkanBaseBuffer mVertexBuffer;

		// Vertex Shader Binding Description 
		unsigned int mBinding;
		VkVertexInputBindingDescription mVertexBindingDescription;
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		bool mExistsVkVertexInputBindingDescription;

	};
}
#endif // VULKAN_BASE_VERTEX_BUFFER_H