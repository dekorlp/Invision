#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"


namespace Invision
{
	class  VulkanPipeline
	{
	public:
		void INVISION_API AddShader(VulkanShader shader);
		void INVISION_API UpdateVertexInputConfiguration();
		void INVISION_API UpdateInputAssemblyConfiguration(VkPrimitiveTopology primitiveTopology);
		/*void INVISION_API UpdateViewPortConfiguration(const SVulkan &vulkanInstance,
			float ViewportX,
			float ViewportY,
			float viewPortWidth,
			float viewPortHeight,
			float minDepth,
			float maxDepth,
			VkOffset2D scissorOffset);*/
		void INVISION_API UpdateViewPortConfiguration(const SVulkan &vulkanInstance);
		void INVISION_API UpdateRasterizerConfiguration(VkPolygonMode fillMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace);
		void INVISION_API UpdateMultisamplingConfiguration();
		void INVISION_API UpdateDepthStencilConfiguration();
		void INVISION_API UpdateColorBlendingAttachmentConfiguration();
		void INVISION_API UpdateDynamicStatesConfiguration();
		void INVISION_API UpdatePipelineLayoutConfiguration();
		VkPipeline INVISION_API GetPipeline();

		void INVISION_API CreatePipeline(const SVulkan &vulkanInstance, VulkanRenderPass &renderPass, uint32_t subpassIndex);
		void INVISION_API DestroyPipeline(const SVulkan &vulkanInstance);
	private:

		// Configurations
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages = {}; // Shaders
		VkPipelineVertexInputStateCreateInfo mVertexInputConfig = {}; // Vertex configuration
		VkPipelineInputAssemblyStateCreateInfo mInputAssembly = {}; // Input assembly configuration
		VkPipelineViewportStateCreateInfo mViewportState = {}; // Viewport & Scissor configuration
		VkPipelineRasterizationStateCreateInfo mRasterizer = {}; // Rasterizer configuration
		VkPipelineMultisampleStateCreateInfo mMultisampling = {}; // Multisampling configuration
		VkPipelineDepthStencilStateCreateFlags mDepthStencil = {}; // Depthstencil Testing configuration
		VkPipelineColorBlendStateCreateInfo mColorBlendAttachment = {}; // Color Blending
		VkPipelineDynamicStateCreateInfo mDynamicState = {}; // Dynamic State configuration
		VkPipelineLayoutCreateInfo mPipelineLayoutInfo = {}; // Pipeline Layout

		// states
		VkPipelineColorBlendAttachmentState mColorBlendAttachmentState = {};
		VkViewport mViewport = {};
		VkRect2D mScissor = {};
		std::vector<VkDynamicState> mDynamicStateEnables = {};

		// pipeline
		VkPipelineLayout mPipelineLayout; // pipelineLayout
		VkPipeline mGraphicsPipeline; // pipeline

		
	};

}


#endif // VULKAN_PIPELINE_H