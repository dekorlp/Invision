#ifndef VULKAN_BASE_VERTEX_BUFFER_H
#define VULKAN_BASE_VERTEX_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBaseBuffer.h"

namespace Invision
{
	class VulkanBaseBindingDescription
	{
	public:
		INVISION_API VulkanBaseBindingDescription();
		INVISION_API VulkanBaseBindingDescription(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,
			std::vector<VkVertexInputBindingDescription> &bindingDescriptions, std::vector<VulkanBaseBuffer> &vertexBuffers, uint32_t binding, uint64_t size, const void *source, uint64_t offset, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API void CreateAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
		INVISION_API VkVertexInputBindingDescription GetBindingDescription();
		INVISION_API uint64_t GetOffset();
	private:
		INVISION_API void AllocateMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, uint64_t size, const void *source, uint64_t offset);

		VkVertexInputBindingDescription mBindingDescription;
		uint64_t mOffset;
		VulkanBaseBuffer mBuffer;
		std::vector<VkVertexInputAttributeDescription> *mAttributeDescriptions;		
	};

	class VulkanBaseAttributeDescription
	{
		public:
			INVISION_API VulkanBaseAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
		private:
			VkVertexInputAttributeDescription mAttributeDescription;
	};

	// Binding Description
	class  VulkanBaseVertexBuffer
	{
	public:
		INVISION_API VulkanBaseVertexBuffer();
		INVISION_API VulkanBaseBindingDescription& CreateBinding(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, uint64_t size, const void *source, uint64_t offset, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions();
		INVISION_API std::vector<VkVertexInputBindingDescription>& GetBindingDescriptions();
		INVISION_API uint64_t GetOffset(uint32_t binding);

		INVISION_API std::vector<VulkanBaseBuffer> &GetBuffers();

		INVISION_API void DestroyVertexBuffers(const SVulkanBase &vulkanInstance);		
		
	private:
		uint32_t maxAllocatedBinding = 0;
		std::vector<VulkanBaseBuffer> mVertexBuffers; // index is Binding ID
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription> mBindingDescriptions; // index is Binding ID

		std::vector< VulkanBaseBindingDescription> mBaseBindingDescriptions;
	};
}
#endif // VULKAN_BASE_VERTEX_BUFFER_H