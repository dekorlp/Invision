#ifndef VULKAN_INDEX_BUFFER_H
#define VULKAN_INDEX_BUFFER_H


#include "IIndexBuffer.h"
#include "renderer\Vulkan\VulkanBaseIndexBuffer.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanIndexBuffer : public IIndexBuffer
	{
	public:

		INVISION_API VulkanIndexBuffer() = delete;

		INVISION_API VulkanIndexBuffer(VulkanEngine* engine);

		INVISION_API void CreateIndexBuffer(uint64_t size, const void* source, uint64_t offset);

		INVISION_API ~VulkanIndexBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanBaseIndexBuffer indexBuffer;
	};

}

#endif // VULKAN_INDEX_BUFFER_H