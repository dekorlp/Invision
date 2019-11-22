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
		void INVISION_API UpdateVertexInputConfiguration();
		void INVISION_API UpdateInputAssemblyConfiguration(VkPrimitiveTopology primitiveTopology);
		void INVISION_API UpdateViewPortConfiguration(const SVulkan &vulkanInstance,
			float ViewportX,
			float ViewportY,
			float viewPortWidth,
			float viewPortHeight,
			float minDepth,
			float maxDepth,
			VkOffset2D scissorOffset);
		void INVISION_API UpdateRasterizerConfiguration(VkPolygonMode fillMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace);
		void INVISION_API UpdateMultisamplingConfiguration();
		void INVISION_API UpdateDepthStencilConfiguration();
		void INVISION_API UpdateColorBlendingAttachmentConfiguration();
		void INVISION_API UpdateDynamicStatesConfiguration();
		void INVISION_API UpdatePipelineLayoutConfiguration();

		void INVISION_API CreatePipeline(const SVulkan &vulkanInstance);
		void INVISION_API DestroyPipeline(const SVulkan &vulkanInstance);
	private:
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages; // Shaders
		VkPipelineVertexInputStateCreateInfo mVertexInputConfig; // Vertex configuration
		VkPipelineInputAssemblyStateCreateInfo mInputAssembly; // Input assembly configuration
		VkPipelineViewportStateCreateInfo mViewport; // Viewport & Scissor configuration
		VkPipelineRasterizationStateCreateInfo mRasterizer; // Rasterizer configuration
		VkPipelineMultisampleStateCreateInfo mMultisampling; // Multisampling configuration
		VkPipelineDepthStencilStateCreateFlags mDepthStencil; // Depthstencil Testing configuration
		VkPipelineColorBlendStateCreateInfo mColorBlendAttachment; // Color Blending
		VkPipelineDynamicStateCreateInfo mDynamicState; // Dynamic State configuration
		VkPipelineLayoutCreateInfo mPipelineLayoutInfo; // Pipeline Layout

		// pipeline
		VkPipelineLayout pipelineLayout; // pipelineLayout
		VkPipeline pipeline; // pipeline

		
	};

}


#endif // VULKAN_PIPELINE_H