/////////////////////////////////////////////////////////////////////////////
// Name:	  Vulkan
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

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

class Vulkan
{
	public:
		Vulkan();
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

		SVulkanPipeline m_pipeline;
		SVulkanLogicalDevice m_logicalDevice;
		SVulkanSwapchain m_swapchain;

		std::vector<ShaderPipeline> m_ShaderPipelines;

		VulkanDevice vulkanDevice;
		CVulkanPresentation vulkanPresentation;
		VulkanPipeline vulkanPipeline;
		VulkanDrawing vulkanDrawing;
		VulkanMemoryBuffer vulkanMemoryBuffer;
};

#endif // INCLUDED_VULKAN