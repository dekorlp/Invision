#include "precompiled.h"

#include "VulkanDrawing.h"

VulkanDrawing::VulkanDrawing() : m_renderPass(VK_NULL_HANDLE), m_swapchain(VK_NULL_HANDLE), m_swapchainFramebuffers(VK_NULL_HANDLE),
				m_commandBuffers(VK_NULL_HANDLE), m_commandPool(VK_NULL_HANDLE), m_imageAvailableSemaphore(VK_NULL_HANDLE), m_renderFinishedSemaphore(VK_NULL_HANDLE),
				m_swapchainImageViews(VK_NULL_HANDLE), m_surface(VK_NULL_HANDLE)
{
	
}


void VulkanDrawing::CreateFrameBuffers(SVulkanLogicalDevice logicalDevice, VkSurfaceKHR surface, SVulkanSwapchain* swapChain, SVulkanPipeline* pipeline)
{
	m_swapchainExtent = swapChain->m_swapchainExtent;
	m_swapchainImageViews = swapChain->m_swapchainImageViews;
	m_renderPass = pipeline->m_renderPass;
	VkFramebuffer framebuffer;
	m_swapchainFramebuffers.resize(m_swapchainImageViews.size(), framebuffer);

	for (size_t i = 0; i < m_swapchainImageViews.size(); i++) {
		VkImageView attachments[] = {
			m_swapchainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = CreateFramebufferCreateInfo(*attachments);

		VkResult result = vkCreateFramebuffer(logicalDevice.m_logicalDevice, &framebufferInfo, nullptr, &m_swapchainFramebuffers[i]);
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Failed to create framebuffer:");
		}
	}
	m_surface = surface;
}

void VulkanDrawing::CreateCommandPool(VkPhysicalDevice physicalDevice, VkDevice logicalDevice) {
	SQueueFamilyIndices queueFamilyIndices = m_queueFamily.FindQueueFamilies(physicalDevice, m_surface);
	VkCommandPoolCreateInfo poolInfo = CreateCommandPoolCreateInfo(queueFamilyIndices);
	VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &m_commandPool);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create command pool:");
	}
}

void VulkanDrawing::CreateCommandBuffers(SVulkanLogicalDevice logicalDevice, std::vector<VkCommandBuffer> *commandBuffer, SVulkanPipeline* pipeline, std::vector<ShaderPipeline> *shaderPipeline)
{
	m_renderPass = pipeline->m_renderPass;
	if (m_commandBuffers.size() > 0) {
		vkFreeCommandBuffers(logicalDevice.m_logicalDevice, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
	}
	m_commandBuffers.resize(m_swapchainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = CreateCommandBufferAllocateInfo();
	VkResult result = vkAllocateCommandBuffers(logicalDevice.m_logicalDevice, &allocInfo, m_commandBuffers.data());
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to allocate command buffers:");
	}

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (size_t i = 0; i < m_commandBuffers.size(); i++) {

		VkCommandBufferBeginInfo beginInfo = CreateCommandBufferBeginInfo();
		vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = CreateRenderPassBeginInfo(i, clearColor);
		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		for(unsigned int j = 0; j < shaderPipeline->size(); j++)
		{
			if (shaderPipeline->at(j).draw)
			{
				if (shaderPipeline->at(j).vertexSource == IN_VERTEX_SOURCE_SHADER)
				{
					vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline->at(j).pipeline);
					vkCmdDraw(m_commandBuffers[i], shaderPipeline->at(j).shaderCountOfVertizes, 1, 0, 0);
				}
				else if(shaderPipeline->at(j).vertexSource == IN_VERTEX_SOURCE_STRUCT)
				{
					vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline->at(j).pipeline);
					VkBuffer vertexBuffers[] = { shaderPipeline->at(j).meshData.VertexBuffer };
					VkDeviceSize offsets[] = { 0 };
					vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

					vkCmdDraw(m_commandBuffers[i], static_cast<uint32_t>(shaderPipeline->at(j).meshData.meshVertizes.size()), 1, 0, 0);
				}
				else
				{
					// TODO IMPLEMENT C EXCEPTION
				}
			}
		}

		vkCmdEndRenderPass(m_commandBuffers[i]);

		result = vkEndCommandBuffer(m_commandBuffers[i]);
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Failed to record command buffer:");
		}
	}

	*commandBuffer = m_commandBuffers;
}

void VulkanDrawing::CreateSemaphores(SVulkanLogicalDevice logicalDevice, VkSemaphore *semaphore)
{
	VkSemaphoreCreateInfo semaphoreInfo = CreateSemaphoreCreateInfo();

	VkResult result = vkCreateSemaphore(logicalDevice.m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create image available semaphore:");
	}
	result = vkCreateSemaphore(logicalDevice.m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Failed to create render finished semaphore:");
	}

	*semaphore = m_imageAvailableSemaphore;
}

VkFramebufferCreateInfo VulkanDrawing::CreateFramebufferCreateInfo(
	const VkImageView& attachments) const noexcept
{
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = m_renderPass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = &attachments;
	framebufferInfo.width = m_swapchainExtent.width;
	framebufferInfo.height = m_swapchainExtent.height;
	framebufferInfo.layers = 1;
	return framebufferInfo;
}

VkCommandPoolCreateInfo VulkanDrawing::CreateCommandPoolCreateInfo(
	SQueueFamilyIndices& queueFamilyIndices) const noexcept
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	return poolInfo;
}

VkCommandBufferAllocateInfo VulkanDrawing::CreateCommandBufferAllocateInfo() const noexcept
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();
	return allocInfo;
}

VkCommandBufferBeginInfo VulkanDrawing::CreateCommandBufferBeginInfo() const noexcept
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	return beginInfo;
}

VkRenderPassBeginInfo VulkanDrawing::CreateRenderPassBeginInfo(size_t swapchainBufferNumber,
	const VkClearValue& clearValue) const noexcept
{
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_renderPass;
	renderPassInfo.framebuffer = m_swapchainFramebuffers[swapchainBufferNumber];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_swapchainExtent;

	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearValue;
	return renderPassInfo;
}

VkSemaphoreCreateInfo VulkanDrawing::CreateSemaphoreCreateInfo() const noexcept
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	return semaphoreInfo;
}

VkSubmitInfo VulkanDrawing::CreateSubmitInfo(uint32_t imageIndex,
	VkPipelineStageFlags* waitStageFlags) const noexcept
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &m_imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = waitStageFlags;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &m_renderFinishedSemaphore;
	return submitInfo;
}

VkPresentInfoKHR VulkanDrawing::CreatePresentInfoKHR(uint32_t& imageIndex, SVulkanSwapchain* swapchain) noexcept
{
	m_swapchain = swapchain->m_swapchain;
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_renderFinishedSemaphore;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &(swapchain->m_swapchain);

	presentInfo.pImageIndices = &imageIndex;
	return presentInfo;
}


void VulkanDrawing::cleanUpCommandBuffers(SVulkanLogicalDevice logicalDevice)
{
	if (m_commandPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(logicalDevice.m_logicalDevice, m_commandPool, nullptr);
	}
}

void VulkanDrawing::cleanUpFrameBuffers(SVulkanLogicalDevice logicalDevice)
{
	for (auto& framebuffer : m_swapchainFramebuffers) {
		vkDestroyFramebuffer(logicalDevice.m_logicalDevice, framebuffer, nullptr);
	}
}

void VulkanDrawing::cleanUpSamaphores(SVulkanLogicalDevice logicalDevice)
{
	if (m_imageAvailableSemaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(logicalDevice.m_logicalDevice, m_imageAvailableSemaphore, nullptr);
	}
	if (m_renderFinishedSemaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(logicalDevice.m_logicalDevice, m_renderFinishedSemaphore, nullptr);
	}
}