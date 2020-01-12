#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanShader.h"

#include "VulkanPipeline.h"

namespace Invision
{

	VkPipelineCache CreatePipelineCache(const SVulkan &vulkanInstance)
	{
		VkPipelineCache cache;

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		if (vkCreatePipelineCache(vulkanInstance.logicalDevice, &pipelineCacheCreateInfo, nullptr, &cache) != VK_SUCCESS)
		{
			throw VulkanException("failed to create pipeline cache!");
		}

		return cache;
	}

	void DestroyPipelineCache(const SVulkan &vulkanInstance, VkPipelineCache cache)
	{
		vkDestroyPipelineCache(vulkanInstance.logicalDevice, cache, nullptr);
	}

	void VulkanPipeline::AddShader(VulkanShader shader)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shader.GetShaderStageFlatBits();
		shaderStageInfo.module = shader.GetShaderModule();
		shaderStageInfo.pName = "main";

		mShaderStages.push_back(shaderStageInfo);
	}

	void INVISION_API VulkanPipeline::AddVertexBuffer(VulkanVertexBuffer& vertexBuffer)
	{
		mVertexInputBindingDescriptions.push_back(vertexBuffer.GetBindingDescription());
		for (int i = 0; i < vertexBuffer.GetAttributeDescriptions().size(); i++)
		{
			mVertexInputAttributeDescriptions.push_back(vertexBuffer.GetAttributeDescriptions()[i]);
		}
	}

	void VulkanPipeline::UpdateVertexInputConfiguration()
	{
		mVertexInputConfig.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mVertexInputConfig.vertexBindingDescriptionCount = mVertexInputBindingDescriptions.size();
		mVertexInputConfig.pVertexBindingDescriptions = mVertexInputBindingDescriptions.data();
		mVertexInputConfig.vertexAttributeDescriptionCount = mVertexInputAttributeDescriptions.size();
		mVertexInputConfig.pVertexAttributeDescriptions = mVertexInputAttributeDescriptions.data();
	}

	void VulkanPipeline::UpdateInputAssemblyConfiguration(VkPrimitiveTopology primitiveTopology)
	{
		mInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mInputAssembly.topology = primitiveTopology; // default: VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		mInputAssembly.primitiveRestartEnable = VK_FALSE;
	}

	void VulkanPipeline::UpdateViewPortConfiguration(const SVulkan &vulkanInstance)
	{
		
		//mViewport.x = ViewportX; // default: 0.0f;
		//mViewport.y = ViewportY; // default: 0.0f;
		//mViewport.width = viewPortWidth; // default: (float)vulkanInstance.swapchainExtend.Width
		//mViewport.height = viewPortHeight;// default: (float)vulkanInstance.swapchainExtend.Height
		//mViewport.minDepth = minDepth; // default: 0.0
		//mViewport.maxDepth = maxDepth; // default: 1.0

		
		//mScissor.offset = scissorOffset; // default: { 0, 0 };
		//mScissor.extent = vulkanInstance.swapChainExtent;

		mViewportState = {};
		mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mViewportState.viewportCount = 1;
		mViewportState.scissorCount = 1;
		//mViewportState.pViewports = &mViewport;
		
		//mViewportState.pScissors = &mScissor;
	}

	void VulkanPipeline::UpdateRasterizerConfiguration(VkPolygonMode polygonFillMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace)
	{		
		mRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		mRasterizer.depthClampEnable = VK_FALSE;
		mRasterizer.rasterizerDiscardEnable = VK_FALSE;
		mRasterizer.polygonMode = polygonFillMode; // default: VK_POLYGON_MODE_FILL;
		mRasterizer.lineWidth = lineWidth; // default: 1.0f;
		mRasterizer.cullMode = cullMode; // default: VK_CULL_MODE_BACK_BIT;
		mRasterizer.frontFace = frontFace; // default: VK_FRONT_FACE_CLOCKWISE;
		mRasterizer.depthBiasEnable = VK_FALSE;
		mRasterizer.depthBiasConstantFactor = 0.0f;
		mRasterizer.depthBiasClamp = 0.0f;
		mRasterizer.depthBiasSlopeFactor = 0.0f;
	}

	void VulkanPipeline::UpdateMultisamplingConfiguration()
	{
		mMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		mMultisampling.sampleShadingEnable = VK_FALSE;
		mMultisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		mMultisampling.minSampleShading = 1.0f; // Optional
		mMultisampling.pSampleMask = nullptr; // Optional
		mMultisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		mMultisampling.alphaToOneEnable = VK_FALSE; // Optional
	}

	void VulkanPipeline::UpdateDepthStencilConfiguration()
	{
	}

	void VulkanPipeline::UpdateColorBlendingAttachmentConfiguration()
	{
		mColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		mColorBlendAttachmentState.blendEnable = VK_FALSE;

		mColorBlendAttachment.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mColorBlendAttachment.logicOpEnable = VK_FALSE;
		mColorBlendAttachment.logicOp = VK_LOGIC_OP_COPY;
		mColorBlendAttachment.attachmentCount = 1;
		mColorBlendAttachment.pAttachments = &mColorBlendAttachmentState;
		mColorBlendAttachment.blendConstants[0] = 0.0f;
		mColorBlendAttachment.blendConstants[1] = 0.0f;
		mColorBlendAttachment.blendConstants[2] = 0.0f;
		mColorBlendAttachment.blendConstants[3] = 0.0f;
	}

	void VulkanPipeline::UpdateDynamicStatesConfiguration()
	{
		mDynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		mDynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);

		mDynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		mDynamicState.pDynamicStates = mDynamicStateEnables.data();
		mDynamicState.dynamicStateCount = static_cast<uint32_t>(mDynamicStateEnables.size());
	}

	void VulkanPipeline::UpdatePipelineLayoutConfiguration()
	{
		mPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		mPipelineLayoutInfo.pNext = VK_NULL_HANDLE;
		mPipelineLayoutInfo.flags = 0;
		mPipelineLayoutInfo.setLayoutCount = 0; // Optional
		mPipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		mPipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		mPipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	}

	VkPipeline VulkanPipeline::GetPipeline()
	{
		return mGraphicsPipeline;
	}

	void VulkanPipeline::CreatePipeline(const SVulkan &vulkanInstance, VulkanRenderPass &renderPass, uint32_t subpassIndex, VkPipelineCache pipelineCache)
	{
		UpdateVertexInputConfiguration();
		UpdateInputAssemblyConfiguration(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		UpdateViewPortConfiguration(vulkanInstance);
		UpdateRasterizerConfiguration(VK_POLYGON_MODE_FILL, 1.0, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
		UpdateMultisamplingConfiguration();
		UpdateDepthStencilConfiguration();
		UpdateColorBlendingAttachmentConfiguration();
		UpdateDynamicStatesConfiguration();
		UpdatePipelineLayoutConfiguration();
		
		if (vkCreatePipelineLayout(vulkanInstance.logicalDevice, &mPipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = mShaderStages.size();
		pipelineInfo.pStages = mShaderStages.data();
		pipelineInfo.pVertexInputState = &mVertexInputConfig;
		pipelineInfo.pInputAssemblyState = &mInputAssembly;
		pipelineInfo.pViewportState = &mViewportState;
		pipelineInfo.pRasterizationState = &mRasterizer;
		pipelineInfo.pMultisampleState = &mMultisampling;
		pipelineInfo.pDepthStencilState = nullptr; // unused
		pipelineInfo.pColorBlendState = &mColorBlendAttachment;
		pipelineInfo.pDynamicState = &mDynamicState;
		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = renderPass.GetRenderPass();
		pipelineInfo.subpass = subpassIndex;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(vulkanInstance.logicalDevice, pipelineCache, 1, &pipelineInfo, nullptr, &mGraphicsPipeline) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create graphics pipeline!");
		}
	}

	void VulkanPipeline::DestroyPipeline(const SVulkan &vulkanInstance)
	{
		vkDestroyPipeline(vulkanInstance.logicalDevice, mGraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(vulkanInstance.logicalDevice, mPipelineLayout, nullptr);
		mShaderStages.clear();
	}
}
