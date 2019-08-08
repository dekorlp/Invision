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