#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H


#include "IVertexBuffer.h"
#include "renderer\Vulkan\VulkanBaseVertexBuffer.h"

namespace Invision
{
	class VulkanInstance;

	class VulkanAttributeDescription : public IAttributeDescription
	{
	public:
		INVISION_API VulkanAttributeDescription() = delete;
		INVISION_API VulkanAttributeDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, VulkanBaseBindingDescription& baseBindingDesc, uint32_t location, VertexFormat format, uint32_t offset);

	private:
		Invision::VulkanInstance *vulkanInstance;
	};

	class VulkanBindingDescription : public IBindingDescription
	{
	public:

		INVISION_API VulkanBindingDescription() = delete;
		INVISION_API VulkanBindingDescription(VulkanInstance* instance, VulkanBaseVertexBuffer &baseVertexBuffer, uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate);
		INVISION_API IBindingDescription& CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset);

	private:
		Invision::VulkanInstance *vulkanInstance;
		VulkanBaseVertexBuffer *mBaseVertexBuffer;
		VulkanBaseBindingDescription mBaseBindingDesc;
		std::vector<VulkanAttributeDescription> mAttributeDesc;
	};

	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:
		INVISION_API VulkanVertexBuffer() = delete;
		INVISION_API VulkanVertexBuffer(VulkanInstance* instance);
		
		std::shared_ptr < IBindingDescription> CreateVertexBinding(uint64_t size, const void *source, uint32_t stride, VertexInputRate vertexInputRate);

		INVISION_API VulkanBaseVertexBuffer  GetBaseVertexBuffer();

		INVISION_API ~VulkanVertexBuffer();

	private:
		Invision::VulkanInstance *vulkanInstance;

		VulkanBaseVertexBuffer mBaseVertexBuffer;
	};

}

#endif // VULKAN_VERTEX_BUFFER_H