#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H


#include "IVertexBuffer.h"


namespace Invision
{
	class VulkanEngine;
	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:

		INVISION_API VulkanVertexBuffer() = delete;

		INVISION_API VulkanVertexBuffer(VulkanEngine* engine);

		INVISION_API ~VulkanVertexBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;

	};

}

#endif // VULKAN_VERTEX_BUFFER_H