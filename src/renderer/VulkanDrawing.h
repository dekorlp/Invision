/* Copyright (C) 2019 Wildfire Games.
* This file is part of 0 A.D.
*
* 0 A.D. is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* 0 A.D. is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_VULKANDRAWING
#define INCLUDED_VULKANDRAWING

#include <vector>
#include "VulkanException.h"
#include "VulkanDataVariables.h"
#include "VulkanQueueFamily.h"

class CVulkanDrawing
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

	CVulkanQueueFamily m_queueFamily;

	VkFramebufferCreateInfo CreateFramebufferCreateInfo(
		const VkImageView& attachments) const noexcept;

	VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(
		QueueFamilyIndices& queueFamilyIndices) const noexcept;

	VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo() const noexcept;

	VkCommandBufferBeginInfo CreateCommandBufferBeginInfo() const noexcept;

	VkRenderPassBeginInfo CreateRenderPassBeginInfo(size_t swapchainBufferNumber,
		const VkClearValue& clearValue) const noexcept;

	VkSemaphoreCreateInfo CreateSemaphoreCreateInfo() const noexcept;

public:
	CVulkanDrawing();
	void CreateFrameBuffers(VulkanLogicalDevice logicalDevice, VkSurfaceKHR surface, VulkanSwapchain* swapChain, VulkanPipeline* pipeline);
	void CreateCommandPool(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);
	void CreateCommandBuffers(VulkanLogicalDevice logicalDevice, std::vector<VkCommandBuffer> *commandBuffer, VulkanPipeline* pipeline, std::vector<ShaderPipeline> *shaderPipeline);
	void CreateSemaphores(VulkanLogicalDevice logicalDevice, VkSemaphore *semaphore);
	
	VkSubmitInfo CreateSubmitInfo(uint32_t imageIndex,
		VkPipelineStageFlags* waitStageFlags) const noexcept;

	VkPresentInfoKHR CreatePresentInfoKHR(uint32_t& imageIndex, VulkanSwapchain* swapchain) noexcept;

	void cleanUpCommandBuffers(VulkanLogicalDevice logicalDevice);
	void cleanUpFrameBuffers(VulkanLogicalDevice logicalDevice);
	void cleanUpSamaphores(VulkanLogicalDevice logicalDevice);
};

#endif // INCLUDED_VULKAN