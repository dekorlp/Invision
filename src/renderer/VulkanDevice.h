/////////////////////////////////////////////////////////////////////////////
// Name:	  VulkanDevice
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VULKANDEVICE
#define INCLUDED_VULKANDEVICE

#include <string>
#include <vector>
#include <set>
//#include <fstream>
//#include <wx/wx.h>


#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"
#include "VulkanDataVariables.h"
#include "VulkanQueueFamily.h"
#include "VulkanException.h"

class CVulkanDevice
{
	private:
		VkSurfaceKHR m_surface;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		CVulkanQueueFamily m_queueFamily;

		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
		SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;

		VkDeviceCreateInfo CreateDeviceCreateInfo(
			const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
			const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;

	public:
		CVulkanDevice();
		void CreateWindowSurface(VkInstance instance, HWND hwnd, VkSurfaceKHR *surface);
		void PickPhysicalDevice(VkInstance instance, VkPhysicalDevice *physicalDevice);
		void CreateLogicalDevice(VkInstance instance, VkPhysicalDevice physicalDevice, VulkanLogicalDevice* logicalDevice);
		VkWin32SurfaceCreateInfoKHR CreateWin32SurfaceCreateInfo(HWND hwnd) const noexcept;		
};

#endif // INCLUDED_VULKAN