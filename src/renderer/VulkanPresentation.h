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
