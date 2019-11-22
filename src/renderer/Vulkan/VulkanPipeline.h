#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanShader.h"


namespace Invision
{
	class  VulkanPipeline
	{
	public:
		void INVISION_API AddShader(VulkanShader shader);
	private:
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages; // Shaders
		VkPipelineVertexInputStateCreateInfo mVertexInputState; // Vertex configuration
		VkPipelineInputAssemblyStateCreateInfo mInputAssemblyState; // Input assembly configuration
		VkPipelineViewportStateCreateInfo mViewportState; // Viewport & Scissor configuration
		VkPipelineRasterizationStateCreateInfo mRasterizer; // Rasterizer configuration
		VkPipelineMultisampleStateCreateInfo mMultisampling; // Multisampling configuration
		//VkPipelineDepthStencilStateCreateFlags mDepthStencil; // Depthstencil Testing configuration
		VkPipelineColorBlendAttachmentState mColorBlendAttachment; // Color Blending
		//VkPipelineDynamicStateCreateInfo mDynamicState; // Dynamic State configuration
		VkPipelineLayoutCreateInfo mPipelineLayout; // Pipeline Layout

		
	};

}


#endif // VULKAN_PIPELINE_H