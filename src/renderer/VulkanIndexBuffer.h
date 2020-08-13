#ifndef VULKAN_INDEX_BUFFER_H
#define VULKAN_INDEX_BUFFER_H


#include "IIndexBuffer.h"
#include "renderer\Vulkan\VulkanBaseIndexBuffer.h"

namespace Invision
{
	class VulkanInstance;
	class VulkanIndexBuffer : public IIndexBuffer
	{
	public:

		INVISION_API VulkanIndexBuffer() = delete;

		INVISION_API VulkanIndexBuffer(VulkanInstance* instance);

		INVISION_API void CreateIndexBuffer(uint64_t size, const void* source, uint64_t offset);

		INVISION_API VulkanBaseIndexBuffer GetBuffer();

		INVISION_API ~VulkanIndexBuffer();

	private:
		//Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanInstance *vulkanInstance;

		Invision::VulkanBaseIndexBuffer indexBuffer;
	};

}

#endif // VULKAN_INDEX_BUFFER_H