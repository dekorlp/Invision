/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanPresentation
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANPRESENTATION
#define INCLUDED_VULKANPRESENTATION

#define NOMINMAX

#include <vector>
#include <algorithm>
#define VK_USE_PLATFORM_WIN32_KHR
#include "VulkanException.h"
#include "VulkanDataVariables.h"

class CVulkanPresentation
{
private:
	std::vector<VkImage> m_swapchainImages;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;
	std::vector<VkImageView> m_swapchainImageViews;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapchain;

	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats) const noexcept;
	VkImageViewCreateInfo CreateImageViewCreateInfo(uint32_t swapchainImage) const noexcept;
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
		const int width, const int height) const noexcept;

	VkSwapchainCreateInfoKHR CreateSwapchainCreateInfo(
		const VkPhysicalDevice physicalDevice,
		const SwapChainSupportDetails& swapChainSupport,
		const VkSurfaceFormatKHR& surfaceFormat,
		uint32_t imageCount, const VkExtent2D& extent);

	VkPresentModeKHR ChooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& availablePresentModes) const noexcept;

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device) const;

public:
	CVulkanPresentation();
	void CreateSwapChain(VkPhysicalDevice physicalDevice, VulkanLogicalDevice logicalDevice, VkSurfaceKHR surface, VulkanSwapchain* swapChain, const int width, const int height);
	void CreateImageViews(VulkanLogicalDevice logicalDevice, VulkanSwapchain* swapChain);
	void CleanupPresentation(VulkanLogicalDevice logicalDevice);
};

#endif // VULKANPRESENTATION
