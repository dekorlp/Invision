#ifndef VULKAN_BASE_VERTEX_BUFFER_H
#define VULKAN_BASE_VERTEX_BUFFER_H

#include "vulkan\vulkan.h"

namespace Invision
{
	class VulkanBaseMemoryManager;
	class VulkanBaseCommandPool;
	class VulkanBaseBindingDescription
	{
	public:
		INVISION_API VulkanBaseBindingDescription();
		INVISION_API VulkanBaseBindingDescription( std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,
			std::vector<VkVertexInputBindingDescription> &bindingDescriptions, uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API void CreateAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
		INVISION_API VkVertexInputBindingDescription GetBindingDescription();
	private:
		

		VkVertexInputBindingDescription mBindingDescription;
		//void* mVertexBuffer;
		std::vector<VkVertexInputAttributeDescription> *mAttributeDescriptions;		
	};

	class VulkanBaseAttributeDescription
	{
		public:
			INVISION_API VulkanBaseAttributeDescription(std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
		private:
			VkVertexInputAttributeDescription mAttributeDescription;
	};

	// Vertex Description
	class VulkanBaseVertexBinding
	{
	public:
		INVISION_API VulkanBaseVertexBinding();
		INVISION_API VulkanBaseBindingDescription& CreateBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions();
		INVISION_API std::vector<VkVertexInputBindingDescription>& GetBindingDescriptions();
	private:
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription> mBindingDescriptions; // index is Binding ID

		std::vector< VulkanBaseBindingDescription> mBaseBindingDescriptions;
	};

	// Binding Description
	class  VulkanBaseVertexBuffer
	{
	public:
		INVISION_API VulkanBaseVertexBuffer();
		INVISION_API VulkanBaseBindingDescription& CreateBinding(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint32_t binding, uint64_t size, const void *source, uint32_t stride, VkVertexInputRate inputRate);
		INVISION_API std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions();
		INVISION_API std::vector<VkVertexInputBindingDescription>& GetBindingDescriptions();

		INVISION_API std::vector<void*> &GetBuffers();
		INVISION_API void CreateBuffer(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint32_t binding, uint64_t size, const void *source);

		INVISION_API void DestroyVertexBuffers(const SVulkanBase &vulkanInstance);		
		
	private:
		INVISION_API void* AllocateDedicatedMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source);
		INVISION_API void* AllocateSharedMemory(const SVulkanBase &vulkanInstance, VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, uint64_t size, const void *source);


		VulkanBaseMemoryManager *mMemoryManager;
		std::vector<void*> mVertexBuffers; // index is Binding ID
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription> mBindingDescriptions; // index is Binding ID

		std::vector< VulkanBaseBindingDescription> mBaseBindingDescriptions;
	};
}
#endif // VULKAN_BASE_VERTEX_BUFFER_H