#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"
#include "VulkanBaseShader.h"
#include "VulkanBaseRenderPass.h"
#include "VulkanBaseVertexBuffer.h"
#include "VulkanBaseUniformBuffer.h"
#include "VulkanBasePushConstant.h"

#include "VulkanBasePipeline.h"

namespace Invision
{

	VkPipelineCache CreatePipelineCache(const SVulkanBase &vulkanInstance)
	{
		VkPipelineCache cache;

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		if (vkCreatePipelineCache(vulkanInstance.logicalDevice, &pipelineCacheCreateInfo, nullptr, &cache) != VK_SUCCESS)
		{
			throw VulkanBaseException("failed to create pipeline cache!");
		}

		return cache;
	}

	void DestroyPipelineCache(const SVulkanBase &vulkanInstance, VkPipelineCache cache)
	{
		vkDestroyPipelineCache(vulkanInstance.logicalDevice, cache, nullptr);
	}

	void VulkanBasePipeline::AddShader(VulkanBaseShader shader)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shader.GetShaderStageFlatBits();
		shaderStageInfo.module = shader.GetShaderModule();
		shaderStageInfo.pName = "main";

		mShaderStages.push_back(shaderStageInfo);
	}

	void VulkanBasePipeline::SetRenderProperties(VkPrimitiveTopology primitiveTopology, VkPolygonMode polygonMode, VkCullModeFlags cullModeFlags, VkFrontFace frontFace, float lineWidth)
	{
		mPrimitiveTopology = primitiveTopology;
		mPolygonMode = polygonMode;
		mCullModeFlags = cullModeFlags;
		mFrontFace = frontFace;
		mLineWidth = lineWidth;
	}

	void VulkanBasePipeline::SetColorBlendFunction(VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp)
	{
		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
		pipelineColorBlendAttachmentState.blendEnable = blendEnable;
		pipelineColorBlendAttachmentState.srcColorBlendFactor = srcColorBlendFactor;
		pipelineColorBlendAttachmentState.dstColorBlendFactor = dstColorBlendFactor;
		pipelineColorBlendAttachmentState.colorBlendOp = colorBlendOp;
		pipelineColorBlendAttachmentState.srcAlphaBlendFactor = srcAlphaBlendFactor;
		pipelineColorBlendAttachmentState.dstAlphaBlendFactor = dstAlphaBlendFactor;
		pipelineColorBlendAttachmentState.alphaBlendOp = alphaBlendOp;
		pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		mColorBlendAttachments.insert(std::pair<unsigned int, VkPipelineColorBlendAttachmentState>(0, pipelineColorBlendAttachmentState));
	}

	void VulkanBasePipeline::SetDepthTest(bool enable, VkCompareOp compareOp)
	{
		mDepthTest = enable;
		mCompareOp = compareOp;
	}

	void VulkanBasePipeline::SetDepthWrite(bool enable)
	{
		mDepthWrite = enable;
	}


	
	void VulkanBasePipeline::ClearUniformsBuffer()
	{
		mDescriptorSetLayout.clear();
	}

	void VulkanBasePipeline::AddUniformBuffer(VulkanBaseUniformBuffer& uniformBuffer)
	{
		for (int i = 0; i < uniformBuffer.GetSets().size(); i++)
		{
			mDescriptorSetLayout.push_back(uniformBuffer.GetSets()[i].mDescriptorSetLayout);
		}
	}


	void VulkanBasePipeline::AddUniformBuffer(VulkanBaseUniformBuffer& uniformBuffer, uint32_t set)
	{
		mDescriptorSetLayout.push_back(uniformBuffer.GetSets()[set].mDescriptorSetLayout);
	}

	void VulkanBasePipeline::AddVertexDescription(VulkanBaseVertexBinding& vertexBinding)
	{
		// Insert Attribute Descriptions
		for (unsigned int i = 0; i < vertexBinding.GetAttributeDescriptions().size(); i++)
		{
			mVertexInputAttributeDescriptions.push_back(vertexBinding.GetAttributeDescriptions()[i]);
		}

		// Insert Binding Descriptions
		for (unsigned int i = 0; i < vertexBinding.GetBindingDescriptions().size(); i++)
		{
			mVertexInputBindingDescriptions.push_back(vertexBinding.GetBindingDescriptions()[i]);
		}
	}

	void VulkanBasePipeline::BindPushConstant(VulkanBasePushConstant& pushConstant)
	{
		if (mPushConstRange.size() == 0)
		{
			mPushConstRange.push_back(pushConstant.ConstructPushConstantRange());
		}
		else
		{
			mPushConstRange[0] = pushConstant.ConstructPushConstantRange();
		}
	}

	void VulkanBasePipeline::UpdateVertexInputConfiguration()
	{
		mVertexInputConfig.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mVertexInputConfig.vertexBindingDescriptionCount = static_cast<unsigned int>(mVertexInputBindingDescriptions.size());
		mVertexInputConfig.pVertexBindingDescriptions = mVertexInputBindingDescriptions.data();
		mVertexInputConfig.vertexAttributeDescriptionCount = static_cast<unsigned int>(mVertexInputAttributeDescriptions.size());
		mVertexInputConfig.pVertexAttributeDescriptions = mVertexInputAttributeDescriptions.data();
	}

	void VulkanBasePipeline::UpdateInputAssemblyConfiguration(VkPrimitiveTopology primitiveTopology)
	{
		mInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mInputAssembly.topology = primitiveTopology; // default: VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		mInputAssembly.primitiveRestartEnable = VK_FALSE;
	}

	void VulkanBasePipeline::UpdateViewPortConfiguration(const SVulkanBase &vulkanInstance)
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

	void VulkanBasePipeline::UpdateRasterizerConfiguration(VkPolygonMode polygonFillMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace)
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

	void VulkanBasePipeline::UpdateMultisamplingConfiguration(VkSampleCountFlagBits numSamples, VkBool32 sampleShadingEnable, float minSampleShading)
	{
		mMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		mMultisampling.sampleShadingEnable = sampleShadingEnable;
		mMultisampling.rasterizationSamples = numSamples;
		mMultisampling.minSampleShading = minSampleShading; // Optional
		mMultisampling.pSampleMask = nullptr; // Optional
		mMultisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		mMultisampling.alphaToOneEnable = VK_FALSE; // Optional
	}

	void VulkanBasePipeline::UpdateDepthStencilConfiguration( )
	{
		mDepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		mDepthStencil.depthTestEnable = mDepthTest;
		mDepthStencil.depthWriteEnable = mDepthWrite;
		mDepthStencil.depthCompareOp = mCompareOp; // VK_COMPARE_OP_LESS
		mDepthStencil.depthBoundsTestEnable = VK_FALSE;
		mDepthStencil.minDepthBounds = 0.0f; // Optional
		mDepthStencil.maxDepthBounds = 1.0f; // Optional
		mDepthStencil.stencilTestEnable = VK_FALSE;
		mDepthStencil.front = {}; // Optional
		mDepthStencil.back = {}; // Optional

	}

	void VulkanBasePipeline::UpdateColorBlendingAttachmentConfiguration(unsigned int colorAttachmentCount)
	{
		VkPipelineColorBlendAttachmentState mColorBlendAttachmentState = {};
		mColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		mColorBlendAttachmentState.blendEnable = VK_FALSE;
		
		for (unsigned int i = 0; i < colorAttachmentCount; i++)
		{
			if (mColorBlendAttachments.find(i) == mColorBlendAttachments.end())
			{
				mColorBlendAttachmentStates.push_back(mColorBlendAttachmentState);
			}
			else
			{
				mColorBlendAttachmentStates.push_back(mColorBlendAttachments.at(i));
			}
			
		}

		mColorBlendAttachment.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mColorBlendAttachment.logicOpEnable = VK_FALSE;
		mColorBlendAttachment.logicOp = VK_LOGIC_OP_COPY;
		mColorBlendAttachment.attachmentCount = static_cast<unsigned int>(mColorBlendAttachmentStates.size());
		mColorBlendAttachment.pAttachments = mColorBlendAttachmentStates.data();
		mColorBlendAttachment.blendConstants[0] = 0.0f;
		mColorBlendAttachment.blendConstants[1] = 0.0f;
		mColorBlendAttachment.blendConstants[2] = 0.0f;
		mColorBlendAttachment.blendConstants[3] = 0.0f;
	}

	void VulkanBasePipeline::UpdateDynamicStatesConfiguration()
	{
		mDynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		mDynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);

		mDynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		mDynamicState.pDynamicStates = mDynamicStateEnables.data();
		mDynamicState.dynamicStateCount = static_cast<uint32_t>(mDynamicStateEnables.size());
	}

	void VulkanBasePipeline::UpdatePipelineLayoutConfiguration()
	{
		mPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		mPipelineLayoutInfo.pNext = VK_NULL_HANDLE;
		mPipelineLayoutInfo.flags = 0;
		mPipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(mDescriptorSetLayout.size()); // Optional
		mPipelineLayoutInfo.pSetLayouts = mDescriptorSetLayout.data(); // Optional
		mPipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(mPushConstRange.size()); // Optional
		mPipelineLayoutInfo.pPushConstantRanges = mPushConstRange.data(); // Optional
	}

	VkPipeline VulkanBasePipeline::GetPipeline()
	{
		return mGraphicsPipeline;
	}

	VkPipelineLayout VulkanBasePipeline::GetPipelineLayout()
	{
		return mPipelineLayout;
	}

	void VulkanBasePipeline::CreatePipeline(const SVulkanBase &vulkanInstance, VulkanBaseRenderPass &renderPass, uint32_t subpassIndex, bool useDepthRessource, unsigned int colorAttachmentCount, VkSampleCountFlagBits numSamples, float minDepthBound, float maxDepthBound, VkPipelineCache pipelineCache)
	{
		UpdateVertexInputConfiguration();
		UpdateInputAssemblyConfiguration(mPrimitiveTopology);
		UpdateViewPortConfiguration(vulkanInstance);
		UpdateRasterizerConfiguration(mPolygonMode, mLineWidth, mCullModeFlags, mFrontFace);

		if (vulkanInstance.UseMSAA)
		{
			UpdateMultisamplingConfiguration(numSamples, VK_TRUE, .2f);
		}
		else
		{
			UpdateMultisamplingConfiguration(numSamples);
		}

		if(useDepthRessource) UpdateDepthStencilConfiguration();
		UpdateColorBlendingAttachmentConfiguration(colorAttachmentCount);
		UpdateDynamicStatesConfiguration();
		UpdatePipelineLayoutConfiguration();
		
		if (vkCreatePipelineLayout(vulkanInstance.logicalDevice, &mPipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<unsigned int>(mShaderStages.size());
		pipelineInfo.pStages = mShaderStages.data();
		pipelineInfo.pVertexInputState = &mVertexInputConfig;
		pipelineInfo.pInputAssemblyState = &mInputAssembly;
		pipelineInfo.pViewportState = &mViewportState;
		pipelineInfo.pRasterizationState = &mRasterizer;
		pipelineInfo.pMultisampleState = &mMultisampling;
		pipelineInfo.pColorBlendState = &mColorBlendAttachment;
		pipelineInfo.pDynamicState = &mDynamicState;
		if (useDepthRessource)
		{
			pipelineInfo.pDepthStencilState = &mDepthStencil;
		}
		else
		{
			pipelineInfo.pDepthStencilState = nullptr;
		}
		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = renderPass.GetRenderPass();
		pipelineInfo.subpass = subpassIndex;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(vulkanInstance.logicalDevice, pipelineCache, 1, &pipelineInfo, nullptr, &mGraphicsPipeline) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create graphics pipeline!");
		}
	}

	void VulkanBasePipeline::DestroyPipeline(const SVulkanBase &vulkanInstance)
	{
		if (mGraphicsPipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(vulkanInstance.logicalDevice, mGraphicsPipeline, nullptr);
		}

		if (mPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(vulkanInstance.logicalDevice, mPipelineLayout, nullptr);
		}

		mShaderStages.clear();
	}
}
