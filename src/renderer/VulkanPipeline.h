#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H


#include "IPipeline.h"
namespace Invision
{
	class VulkanEngine;
	class VulkanPipeline : public IPipeline
	{
	public:

		INVISION_API VulkanPipeline() = delete;

		INVISION_API VulkanPipeline(VulkanEngine* engine);

		INVISION_API void render() override;
	};

}

#endif // VULKAN_PIPELINE_H