/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanPipeline
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANPIPELINE
#define INCLUDED_VULKANPIPELINE

#include <vector>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "VulkanException.h"
#include "VulkanDataVariables.h"

class CVulkanPipeline
{
private:
	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
	//VkPipeline m_graphicsPipeline;
	//VkPipeline m_graphicsPipeline2;
	VkDevice m_logicalDevice;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;

	VkVertexInputBindingDescription m_BindingDescription;
	std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;

	std::vector<char> ReadFile(const std::string& filename);
	void CreateShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule) const;
	VkShaderModuleCreateInfo CreateShaderModuleCreateInfo(
		const std::vector<char>& code) const noexcept;
	VkAttachmentDescription CreateAttachmentDescription() const noexcept;

	VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(
		VkShaderStageFlagBits stage, VkShaderModule& module, const char* entryName) const noexcept;

	VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo(VertexSource vertexSource) noexcept;

	VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo(
		const VkPrimitiveTopology& topology, uint32_t restartEnable) const noexcept;

	VkViewport CreateViewport() const noexcept;

	VkRect2D CreateScissor() const noexcept;

	VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo(
		const VkViewport& viewport, const VkRect2D& scissor) const noexcept;


	VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo() const noexcept;

	VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo() const noexcept;

	VkPipelineColorBlendAttachmentState CreatePipelineColorBlendAttachmentState() const noexcept;

	VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo(
		const VkPipelineColorBlendAttachmentState& colorBlendAttachment) const noexcept;

	VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo() const noexcept;

	VkGraphicsPipelineCreateInfo CreateGraphicsPipelineCreateInfo(
		const VkPipelineShaderStageCreateInfo shaderStages[],
		const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
		const VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
		const VkPipelineViewportStateCreateInfo& viewportState,
		const VkPipelineRasterizationStateCreateInfo& rasterizer,
		const VkPipelineMultisampleStateCreateInfo& multisampling,
		const VkPipelineColorBlendStateCreateInfo& colorBlending) const noexcept;

	VkAttachmentReference CreateAttachmentReference() const noexcept;

	VkSubpassDescription CreateSubpassDescription(
		const VkAttachmentReference& attachmentRef) const noexcept;

	VkSubpassDependency CreateSubpassDependency() const noexcept;

	VkRenderPassCreateInfo CreateRenderPassCreateInfo(
		const VkAttachmentDescription& colorAttachment,
		const VkSubpassDescription& subPass,
		const VkSubpassDependency& dependency) const noexcept;

public:
	CVulkanPipeline();
	void CreateRenderPass(VulkanLogicalDevice logicalDevice, VulkanPipeline* pipeline, VulkanSwapchain* swapchain);
	void CreateGraphicsPipeline(VulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline, VulkanPipeline *pipeline);
	void CleanupPipeline(std::vector<ShaderPipeline> *shaderPipeline);

};

#endif // INCLUDED_VULKANPIPELINE