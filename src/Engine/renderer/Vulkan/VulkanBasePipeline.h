#ifndef VULKAN_BASE_PIPELINE_H
#define VULKAN_BASE_PIPELINE_H

#include "vulkan\vulkan.h"

namespace Invision
{
	class VulkanBaseShader;
	class VulkanBaseVertexBuffer;
	class VulkanBaseRenderPass;
	class VulkanBaseUniformBuffer;
	class VulkanBasePushConstant;

	VkPipelineCache INVISION_API CreatePipelineCache(const SVulkanBase &vulkanInstance);
	void INVISION_API DestroyPipelineCache(const SVulkanBase &vulkanInstance, VkPipelineCache cache);

	class  VulkanBasePipeline
	{
	public:
		void INVISION_API AddVertexBuffer(VulkanBaseVertexBuffer& vertexBuffer);
		void INVISION_API AddShader(VulkanBaseShader shader);
		void INVISION_API SetRenderProperties(VkPrimitiveTopology primitiveTopology, VkPolygonMode polygonMode, VkCullModeFlags cullModeFlags, VkFrontFace frontFace, float lineWidth);
		void INVISION_API ClearUniformsBuffer();
		void INVISION_API AddUniformBuffer(VulkanBaseUniformBuffer& uniformBuffer);
		void INVISION_API AddUniformBuffer(VulkanBaseUniformBuffer& uniformBuffer, uint32_t set);
		void INVISION_API BindPushConstant(VulkanBasePushConstant& pushConstant);
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
		void INVISION_API UpdateViewPortConfiguration(const SVulkanBase &vulkanInstance);
		void INVISION_API UpdateRasterizerConfiguration(VkPolygonMode fillMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace);
		void INVISION_API UpdateMultisamplingConfiguration(VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT, VkBool32 sampleShadingEnable = VK_FALSE, float minSampleShading = 1.0f);
		void INVISION_API UpdateDepthStencilConfiguration();
		void INVISION_API UpdateColorBlendingAttachmentConfiguration();
		void INVISION_API UpdateDynamicStatesConfiguration();
		void INVISION_API UpdatePipelineLayoutConfiguration();
		VkPipeline INVISION_API GetPipeline();
		VkPipelineLayout INVISION_API GetPipelineLayout();

		void INVISION_API CreatePipeline(const SVulkanBase &vulkanInstance, VulkanBaseRenderPass &renderPass, uint32_t subpassIndex, bool useDepthRessource, VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT, float minDepthBound = 0.0f, float maxDepthBound = 1.0f, VkPipelineCache pipelineCache = VK_NULL_HANDLE);
		void INVISION_API DestroyPipeline(const SVulkanBase &vulkanInstance);
		
	private:

		// Render Properties
		VkPrimitiveTopology mPrimitiveTopology;
		VkPolygonMode mPolygonMode;
		VkCullModeFlags mCullModeFlags;
		VkFrontFace mFrontFace;
		float mLineWidth;

		// Configurations
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages = {}; // Shaders
		std::vector<VkDescriptorSetLayout> mDescriptorSetLayout = {}; // Uniform Buffers
		std::vector<VkPushConstantRange> mPushConstRange = {}; // Push Constant
		std::vector<VkVertexInputBindingDescription> mVertexInputBindingDescriptions = {}; // VertexInputBindingDescriptions
		std::vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescriptions = {}; // VertexInputAttributeDescriptions;
		VkPipelineVertexInputStateCreateInfo mVertexInputConfig = {}; // Vertex configuration
		VkPipelineInputAssemblyStateCreateInfo mInputAssembly = {}; // Input assembly configuration
		VkPipelineViewportStateCreateInfo mViewportState = {}; // Viewport & Scissor configuration
		VkPipelineRasterizationStateCreateInfo mRasterizer = {}; // Rasterizer configuration
		VkPipelineMultisampleStateCreateInfo mMultisampling = {}; // Multisampling configuration
		VkPipelineDepthStencilStateCreateInfo mDepthStencil = {}; // Depthstencil Testing configuration
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


#endif // VULKAN_BASE_PIPELINE_H