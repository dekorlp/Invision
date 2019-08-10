#include "precompiled.h"

#include "VulkanPipeline.h"

VulkanPipeline::VulkanPipeline() : m_logicalDevice(VK_NULL_HANDLE), m_renderPass(VK_NULL_HANDLE),  m_pipelineLayout(VK_NULL_HANDLE)
{
	
}


void VulkanPipeline::CreateRenderPass(SVulkanLogicalDevice logicalDevice, SVulkanPipeline* pipeline, SVulkanSwapchain* swapchain)
{
	m_swapchainImageFormat = swapchain->m_swapchainImageFormat;
	m_swapchainExtent = swapchain->m_swapchainExtent;

	VkAttachmentDescription colorAttachment = CreateAttachmentDescription();
	VkAttachmentReference colorAttachmentRef = CreateAttachmentReference();
	VkSubpassDescription subPass = CreateSubpassDescription(colorAttachmentRef);
	VkSubpassDependency dependency = CreateSubpassDependency();
	VkRenderPassCreateInfo renderPassInfo = CreateRenderPassCreateInfo(colorAttachment,
		subPass, dependency);

	VkResult result = vkCreateRenderPass(logicalDevice.m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create a render pass:");
	}

	pipeline->m_renderPass = m_renderPass;
	
}

void VulkanPipeline::CreateGraphicsPipeline(SVulkanLogicalDevice logicalDevice, std::vector<ShaderPipeline> *shaderPipeline, SVulkanPipeline *pipeline)
{
	m_logicalDevice = logicalDevice.m_logicalDevice;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = CreatePipelineLayoutCreateInfo();
	VkResult result = vkCreatePipelineLayout(logicalDevice.m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create pipeline layout:");
	}

	for(unsigned int i = 0; i < shaderPipeline->size(); i++)
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo;
		for(unsigned int j = 0; j < shaderPipeline->at(i).shaderType.size(); j++)
		{
			const std::string test = shaderPipeline->at(i).shaderType.at(j).shaderPath;
			auto shaderCode = ReadFile(test);
			

			CreateShaderModule(shaderCode, shaderPipeline->at(i).shaderType.at(j).shaderModule);
			
			shaderStageInfo.push_back(CreatePipelineShaderStageCreateInfo(shaderPipeline->at(i).shaderType.at(j).shaderState,
				shaderPipeline->at(i).shaderType.at(j).shaderModule, "main"));
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = CreatePipelineVertexInputStateCreateInfo(shaderPipeline->at(i).vertexSource);


		VkPipelineInputAssemblyStateCreateInfo inputAssembly = CreatePipelineInputAssemblyStateCreateInfo(
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
		VkViewport viewport = CreateViewport();
		VkRect2D scissor = CreateScissor();
		VkPipelineViewportStateCreateInfo viewportState = CreatePipelineViewportStateCreateInfo(
			viewport, scissor);
		VkPipelineRasterizationStateCreateInfo rasterizer = CreatePipelineRasterizationStateCreateInfo();
		VkPipelineMultisampleStateCreateInfo multisampling = CreatePipelineMultisampleStateCreateInfo();
		VkPipelineColorBlendAttachmentState colorBlendAttachment = CreatePipelineColorBlendAttachmentState();
		VkPipelineColorBlendStateCreateInfo colorBlending = CreatePipelineColorBlendStateCreateInfo(
			colorBlendAttachment);
		
		VkGraphicsPipelineCreateInfo pipelineInfo = CreateGraphicsPipelineCreateInfo(shaderStageInfo.data(),
			(vertexInputInfo), inputAssembly, viewportState, rasterizer, multisampling, colorBlending);
		result = vkCreateGraphicsPipelines(logicalDevice.m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &(shaderPipeline->at(i).pipeline));
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Failed to create pipeline layout:");
		}
	}

	// delete all shader modules!
	for (unsigned int i = 0; i < shaderPipeline->size(); i++)
	{
		for (unsigned int j = 0; j < shaderPipeline->at(i).shaderType.size(); j++)
		{
			vkDestroyShaderModule(logicalDevice.m_logicalDevice, shaderPipeline->at(i).shaderType.at(j).shaderModule, nullptr);
		}
	}

	pipeline->m_pipelineLayout = m_pipelineLayout;
}

std::vector<char> VulkanPipeline::ReadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Failed to open file: " << filename;
		throw std::runtime_error(ss.str().c_str());
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void VulkanPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule) const
{
	VkShaderModuleCreateInfo createInfo = CreateShaderModuleCreateInfo(code);

	VkResult result = vkCreateShaderModule(m_logicalDevice, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create shader module:");
	}
}

VkShaderModuleCreateInfo VulkanPipeline::CreateShaderModuleCreateInfo(
	const std::vector<char>& code) const noexcept
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = (uint32_t*)code.data();
	return createInfo;
}

VkAttachmentDescription VulkanPipeline::CreateAttachmentDescription() const noexcept
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = m_swapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return colorAttachment;
}

VkPipelineShaderStageCreateInfo VulkanPipeline::CreatePipelineShaderStageCreateInfo(
	VkShaderStageFlagBits stage, VkShaderModule& module, const char* entryName) const noexcept
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stage;
	shaderStageInfo.module = module;
	shaderStageInfo.pName = entryName;
	return shaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo VulkanPipeline::CreatePipelineVertexInputStateCreateInfo(VertexSource vertexSource) noexcept
{
	m_BindingDescription = Vertex::getBindingDescription();
	m_AttributeDescriptions = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	if (vertexSource == IN_VERTEX_SOURCE_STRUCT)
	{
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_AttributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &m_BindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
	}
	else if(vertexSource == IN_VERTEX_SOURCE_SHADER)
	{
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
	}
	else
	{
		// TODO IMPLEMENT C EXCEPTION
	}

	

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanPipeline::CreatePipelineInputAssemblyStateCreateInfo(
	const VkPrimitiveTopology& topology, uint32_t restartEnable) const noexcept
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = topology;
	inputAssembly.primitiveRestartEnable = restartEnable;
	return inputAssembly;
}

VkViewport VulkanPipeline::CreateViewport() const noexcept
{
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapchainExtent.width;
	viewport.height = (float)m_swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}

VkRect2D VulkanPipeline::CreateScissor() const noexcept
{
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapchainExtent;
	return scissor;
}

VkPipelineViewportStateCreateInfo VulkanPipeline::CreatePipelineViewportStateCreateInfo(
	const VkViewport& viewport, const VkRect2D& scissor) const noexcept
{
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
	return viewportState;
}

VkPipelineRasterizationStateCreateInfo VulkanPipeline::CreatePipelineRasterizationStateCreateInfo() const noexcept
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo VulkanPipeline::CreatePipelineMultisampleStateCreateInfo() const noexcept
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	return multisampling;
}

VkPipelineColorBlendAttachmentState VulkanPipeline::CreatePipelineColorBlendAttachmentState() const noexcept
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo VulkanPipeline::CreatePipelineColorBlendStateCreateInfo(
	const VkPipelineColorBlendAttachmentState& colorBlendAttachment) const noexcept
{
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;
	return colorBlending;
}

VkPipelineLayoutCreateInfo VulkanPipeline::CreatePipelineLayoutCreateInfo() const noexcept
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	return pipelineLayoutInfo;
}

VkGraphicsPipelineCreateInfo VulkanPipeline::CreateGraphicsPipelineCreateInfo(
	const VkPipelineShaderStageCreateInfo shaderStages[],
	const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
	const VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
	const VkPipelineViewportStateCreateInfo& viewportState,
	const VkPipelineRasterizationStateCreateInfo& rasterizer,
	const VkPipelineMultisampleStateCreateInfo& multisampling,
	const VkPipelineColorBlendStateCreateInfo& colorBlending) const noexcept
{
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;

	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	return pipelineInfo;
}

VkAttachmentReference VulkanPipeline::CreateAttachmentReference() const noexcept
{
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	return colorAttachmentRef;
}

VkSubpassDescription VulkanPipeline::CreateSubpassDescription(
	const VkAttachmentReference& attachmentRef) const noexcept
{
	VkSubpassDescription subPass = {};
	subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments = &attachmentRef;
	return subPass;
}

VkSubpassDependency VulkanPipeline::CreateSubpassDependency() const noexcept
{
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	return dependency;
}

VkRenderPassCreateInfo VulkanPipeline::CreateRenderPassCreateInfo(
	const VkAttachmentDescription& colorAttachment,
	const VkSubpassDescription& subPass,
	const VkSubpassDependency& dependency) const noexcept
{
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subPass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	return renderPassInfo;
}

void VulkanPipeline::CleanupPipeline(std::vector<ShaderPipeline> *shaderPipeline)
{
	for (unsigned int i = 0; i < shaderPipeline->size(); i++)
	{
		if (shaderPipeline->at(i).pipeline != VK_NULL_HANDLE) {
			vkDestroyPipeline(m_logicalDevice, shaderPipeline->at(i).pipeline, nullptr);
		}
	}

	if (m_pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(m_logicalDevice, m_pipelineLayout, nullptr);
	}
	if (m_renderPass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(m_logicalDevice, m_renderPass, nullptr);
	}
	
}