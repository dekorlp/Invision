#include "precompiled.h"

#include "VulkanPipeline.h"

namespace Invision
{
	void VulkanPipeline::AddShader(VulkanShader shader)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shader.GetShaderStageFlatBits();
		shaderStageInfo.module = shader.GetShaderModule();
		shaderStageInfo.pName = "main";

		mShaderStages.push_back(shaderStageInfo);
	}

	void VulkanPipeline::UpdateVertexInputConfiguration()
	{
		mVertexInputConfig.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mVertexInputConfig.vertexBindingDescriptionCount = 0;
		mVertexInputConfig.pVertexBindingDescriptions = nullptr;
		mVertexInputConfig.vertexAttributeDescriptionCount = 0;
		mVertexInputConfig.pVertexAttributeDescriptions = nullptr;
	}

	void VulkanPipeline::UpdateInputAssemblyConfiguration(VkPrimitiveTopology primitiveTopology)
	{
		mInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mInputAssembly.topology = primitiveTopology; // default: VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		mInputAssembly.primitiveRestartEnable = VK_FALSE;
	}

	void VulkanPipeline::UpdateViewPortConfiguration(const SVulkan &vulkanInstance,
		float ViewportX,
		float ViewportY,
		float viewPortWidth,
		float viewPortHeight,
		float minDepth,
		float maxDepth,
		VkOffset2D scissorOffset)
	{
		VkViewport viewport = {};
		viewport.x = ViewportX; // default: 0.0f;
		viewport.y = ViewportY; // default: 0.0f;
		viewport.width = viewPortWidth; // default: (float)vulkanInstance.swapchainExtend.Width
		viewport.height = viewPortHeight;// default: (float)vulkanInstance.swapchainExtend.Height
		viewport.minDepth = minDepth; // default: 0.0
		viewport.maxDepth = maxDepth; // default: 1.0

		VkRect2D scissor = {};
		scissor.offset = scissorOffset; // default: { 0, 0 };
		scissor.extent = vulkanInstance.swapChainExtent;

		mViewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mViewport.viewportCount = 1;
		mViewport.pViewports = &viewport;
		mViewport.scissorCount = 1;
		mViewport.pScissors = &scissor;
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
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		mColorBlendAttachment.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mColorBlendAttachment.logicOpEnable = VK_FALSE;
		mColorBlendAttachment.logicOp = VK_LOGIC_OP_COPY;
		mColorBlendAttachment.attachmentCount = 1;
		mColorBlendAttachment.pAttachments = &colorBlendAttachment;
		mColorBlendAttachment.blendConstants[0] = 0.0f;
		mColorBlendAttachment.blendConstants[1] = 0.0f;
		mColorBlendAttachment.blendConstants[2] = 0.0f;
		mColorBlendAttachment.blendConstants[3] = 0.0f;
	}

	void VulkanPipeline::UpdateDynamicStatesConfiguration()
	{
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

	void VulkanPipeline::CreatePipeline(const SVulkan &vulkanInstance)
	{
		UpdateVertexInputConfiguration();
		UpdateInputAssemblyConfiguration(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		UpdateViewPortConfiguration(vulkanInstance, 0.0f, 0.0f, static_cast<float>(vulkanInstance.swapChainExtent.width), static_cast<float>(vulkanInstance.swapChainExtent.height), 0.0f, 1.0f, { 0, 0 });
		UpdateRasterizerConfiguration(VK_POLYGON_MODE_FILL, 1.0, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
		UpdateMultisamplingConfiguration();
		UpdateDepthStencilConfiguration();
		UpdateColorBlendingAttachmentConfiguration();
		UpdateDynamicStatesConfiguration();
		UpdatePipelineLayoutConfiguration();
		
		if (vkCreatePipelineLayout(vulkanInstance.logicalDevice, &mPipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void VulkanPipeline::DestroyPipeline(const SVulkan &vulkanInstance)
	{
		vkDestroyPipelineLayout(vulkanInstance.logicalDevice, pipelineLayout, nullptr);
	}
}
