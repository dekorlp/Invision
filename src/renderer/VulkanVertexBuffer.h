#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H


#include "IVertexBuffer.h"
#include "renderer\Vulkan\VulkanBaseVertexBuffer.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:

		INVISION_API VulkanVertexBuffer() = delete;

		INVISION_API VulkanVertexBuffer(VulkanEngine* engine);

		INVISION_API VulkanVertexBuffer& CreateVertexBuffer(uint64_t size, const void* source, uint64_t offset);
		INVISION_API VulkanVertexBuffer& CreateVertexInput(uint32_t binding, uint32_t stride, VertexInputRate vertexInputRate);
		INVISION_API VulkanVertexBuffer& CreateAttribute(uint32_t location, VertexFormat format, uint32_t offset );
		INVISION_API VulkanBaseVertexBuffer GetBuffer();

		INVISION_API ~VulkanVertexBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanBaseVertexBuffer vertexBuffer;

	};

}

#endif // VULKAN_VERTEX_BUFFER_H