/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanDrawing
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANDRAWING
#define INCLUDED_VULKANDRAWING

#include <vector>
//#include "VulkanException.h"
//#include "VulkanDataVariables.h"
#include "VulkanQueueFamily.h"

class VulkanDrawing
{
private:
	std::vector<VkFramebuffer> m_swapchainFramebuffers;
	std::vector<VkCommandBuffer> m_commandBuffers;
	VkCommandPool m_commandPool;
	VkSemaphore m_imageAvailableSemaphore;
	VkSemaphore m_renderFinishedSemaphore;
	std::vector<VkImageView> m_swapchainImageViews;
	VkExtent2D m_swapchainExtent;
	VkRenderPass m_renderPass;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapchain;

	VulkanQueueFamily m_queueFamily;

	VkFramebufferCreateInfo CreateFramebufferCreateInfo(
		const VkImageView& attachments) const noexcept;

	VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(
		SQueueFamilyIndices& queueFamilyIndices) const noexcept;

	VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo() const noexcept;

	VkCommandBufferBeginInfo CreateCommandBufferBeginInfo() const noexcept;

	VkRenderPassBeginInfo CreateRenderPassBeginInfo(size_t swapchainBufferNumber,
		const VkClearValue& clearValue) const noexcept;

	VkSemaphoreCreateInfo CreateSemaphoreCreateInfo() const noexcept;

public:
	VulkanDrawing();
	void CreateFrameBuffers(SVulkanLogicalDevice logicalDevice, VkSurfaceKHR surface, SVulkanSwapchain* swapChain, SVulkanPipeline* pipeline);
	void CreateCommandPool(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);
	void CreateCommandBuffers(SVulkanLogicalDevice logicalDevice, std::vector<VkCommandBuffer> *commandBuffer, SVulkanPipeline* pipeline, std::vector<ShaderPipeline> *shaderPipeline);
	void CreateSemaphores(SVulkanLogicalDevice logicalDevice, VkSemaphore *semaphore);
	
	VkSubmitInfo CreateSubmitInfo(uint32_t imageIndex,
		VkPipelineStageFlags* waitStageFlags) const noexcept;

	VkPresentInfoKHR CreatePresentInfoKHR(uint32_t& imageIndex, SVulkanSwapchain* swapchain) noexcept;

	void cleanUpCommandBuffers(SVulkanLogicalDevice logicalDevice);
	void cleanUpFrameBuffers(SVulkanLogicalDevice logicalDevice);
	void cleanUpSamaphores(SVulkanLogicalDevice logicalDevice);
};

#endif // INCLUDED_VULKAN