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

#ifndef INCLUDED_VULKAN
#define INCLUDED_VULKAN

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"
#include "VulkanException.h"
#include "VulkanDevice.h"
#include "VulkanPresentation.h"
#include "VulkanPipeline.h"
#include "VulkanDrawing.h"
#include "VulkanDataVariables.h"
#include "VulkanMemoryBuffer.h"

class CVulkan
{
	public:
		CVulkan();
		void run(HWND hwnd, const int width, const int height);

		void OnPaint(const int width, const int height);
		void Destroy() noexcept;
		void RecreateSwapchain(const int width, const int height);
		void CreateShaderPipeline(ShaderPipeline shaderPipeline);
		

	private:
		void initVulkan(std::vector<const char*> requiredExtensions);
		void CreateInstance(const VkInstanceCreateInfo& createInfo);

		VkApplicationInfo CreateApplicationInfo(const std::string& appName,
			const int32_t appVersion,
			const std::string& engineName,
			const int32_t engineVersion,
			const int32_t apiVersion) const noexcept;

		VkInstanceCreateInfo CreateInstanceCreateInfo(const VkApplicationInfo& appInfo,
			const std::vector<const char*>& extensionNames,
			const std::vector<const char*>& layerNames) const noexcept;

		VkWin32SurfaceCreateInfoKHR CreateWin32SurfaceCreateInfo(HWND hwnd) const noexcept;

		void cleanupSwapChain();	

		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkSurfaceKHR m_surface;
		bool m_vulkanInitialized;

		std::vector<VkCommandBuffer> m_commandBuffers;
		VkSemaphore m_imageAvailableSemaphore;

		VulkanPipeline m_pipeline;
		VulkanLogicalDevice m_logicalDevice;
		VulkanSwapchain m_swapchain;

		std::vector<ShaderPipeline> m_ShaderPipelines;

		CVulkanDevice vulkanDevice;
		CVulkanPresentation vulkanPresentation;
		CVulkanPipeline vulkanPipeline;
		CVulkanDrawing vulkanDrawing;
		CVulkanMemoryBuffer vulkanMemoryBuffer;
};

#endif // INCLUDED_VULKAN