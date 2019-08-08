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

#include "precompiled.h"

#include "VulkanDevice.h"

CVulkanDevice::CVulkanDevice() : m_surface(VK_NULL_HANDLE), m_presentQueue(VK_NULL_HANDLE), m_graphicsQueue(VK_NULL_HANDLE)
{
	
}

void CVulkanDevice::PickPhysicalDevice(VkInstance instance, VkPhysicalDevice *physicalDevice)
{
	if (!instance) {
		throw std::runtime_error("Programming Error:\n"
			"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
	}
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find a GPU with Vulkan support.");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			*physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("No physical GPU could be found with the required extensions and swap chain support.");
	}
}

void CVulkanDevice::CreateLogicalDevice(VkInstance instance, VkPhysicalDevice physicalDevice, VulkanLogicalDevice* logicalDevice)
{
	QueueFamilyIndices indices =  m_queueFamily.FindQueueFamilies(physicalDevice, m_surface);
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = m_queueFamily.CreateQueueCreateInfos(uniqueQueueFamilies);
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = CreateDeviceCreateInfo(queueCreateInfos, deviceFeatures);

	VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &(logicalDevice->m_logicalDevice));
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to create a logical device");
	}
	vkGetDeviceQueue(logicalDevice->m_logicalDevice, indices.graphicsFamily, 0, &logicalDevice->m_graphicsQueue);
	vkGetDeviceQueue(logicalDevice->m_logicalDevice, indices.graphicsFamily, 0, &logicalDevice->m_presentQueue);
}


bool CVulkanDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
{
	QueueFamilyIndices indices = m_queueFamily.FindQueueFamilies(physicalDevice, m_surface);
	bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.IsComplete() & extensionsSupported && swapChainAdequate;
}

bool CVulkanDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
{
	uint32_t extensionCount;
	VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Cannot retrieve count of properties for a physical device:");
	}
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Cannot retrieve properties for a physical device:");
	}
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails CVulkanDevice::QuerySwapChainSupport(const VkPhysicalDevice& device) const
{
	SwapChainSupportDetails details;

	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve physical device surface capabilities:");
	}
	uint32_t formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve the number of formats for a surface on a physical device:");
	}
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Unable to retrieve the formats for a surface on a physical device:");
		}
	}

	uint32_t presentModeCount = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if (result != VK_SUCCESS) {
		throw CVulkanException(result, "Unable to retrieve the count of present modes for a surface on a physical device:");
	}
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
		if (result != VK_SUCCESS) {
			throw CVulkanException(result, "Unable to retrieve the present modes for a surface on a physical device:");
		}
	}
	return details;
}

VkDeviceCreateInfo CVulkanDevice::CreateDeviceCreateInfo(
	const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
	const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept
{
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}
	return createInfo;
}

#ifdef _WIN32
VkWin32SurfaceCreateInfoKHR CVulkanDevice::CreateWin32SurfaceCreateInfo(HWND hwnd) const noexcept
{
	VkWin32SurfaceCreateInfoKHR sci = {};
	sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hwnd = hwnd;
	sci.hinstance = GetModuleHandle(NULL);
	return sci;
}
#endif

void CVulkanDevice::CreateWindowSurface(VkInstance instance, HWND hwnd, VkSurfaceKHR *surface)
{
	if (!instance) {
		throw std::runtime_error("Programming Error:\n"
			"Attempted to create a window surface before the Vulkan instance was created.");
	}
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR sci = CreateWin32SurfaceCreateInfo(hwnd);
	VkResult err = vkCreateWin32SurfaceKHR(instance, &sci, nullptr, &m_surface);
	if (err != VK_SUCCESS) {
		throw CVulkanException(err, "Cannot create a Win32 Vulkan surface:");
	}
	*surface = m_surface;
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif
}