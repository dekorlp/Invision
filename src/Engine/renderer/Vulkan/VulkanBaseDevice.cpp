#include "precompiled.h"

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif


#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseDevice.h"



namespace Invision
{

	//void VulkanBaseDevice::CreateVulkanDevice(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext)
	//{
	//	GetDevices(vulkanInstance, vulkanContext);
	//}

	


	VulkanBaseDevice::VulkanBaseDevice()
	{
		
	}

	void VulkanBaseDevice::CreateSurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, HWND hwnd)
	{

#ifdef _WIN32
		VkWin32SurfaceCreateInfoKHR sci = {};
		sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		sci.hwnd = hwnd;
		sci.hinstance = GetModuleHandle(NULL);


		VkResult err = vkCreateWin32SurfaceKHR(vulkanInstance.instance, &sci, nullptr, &vulkanContext.surface);
		if (err != VK_SUCCESS) {
			throw VulkanBaseException(err, "Cannot create a Win32 Vulkan surface:");
		}

		IsDeviceSurfaceSuitable(vulkanInstance.physicalDeviceStruct, vulkanContext.surface);

		// Create Present Queue
		SQueueFamilyIndices indices = FindPresentQueueFamiliy(vulkanInstance.physicalDeviceStruct.physicalDevice, vulkanContext, vulkanContext.surface);
		vkGetDeviceQueue(vulkanContext.logicalDevice, vulkanContext.indices.presentFamily, 0, &vulkanContext.presentQueue);

		//SQueueFamilyIndices indices = FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, vulkanContext, vulkanContext.surface);
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif
	}

	bool VulkanBaseDevice::IsDeviceSurfaceSuitable(SVulkanBasePhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR surface)
	{

		//SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);
		//bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		bool swapChainAdequate = false;
		if (vulkanPhysicalDevice.extensionSupported) {
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vulkanPhysicalDevice.physicalDevice, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		return swapChainAdequate;
	}

	void VulkanBaseDevice::DestroySurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext)
	{
		vkDestroySurfaceKHR(vulkanInstance.instance, vulkanContext.surface, nullptr);
	}

	VkDeviceCreateInfo VulkanBaseDevice::CreateDeviceCreateInfo(SVulkanBase &vulkanInstance,
		const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
		const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept
	{
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(vulkanInstance.deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = vulkanInstance.deviceExtensions.data();
		if (vulkanInstance.enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanInstance.validationLayers.size());
			createInfo.ppEnabledLayerNames = vulkanInstance.validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		return createInfo;
	}



	bool VulkanBaseDevice::CreateLogicalDevice(SVulkanBase& vulkanInstance, SVulkanContext& context)
	{
		context.indices = FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, VK_QUEUE_GRAPHICS_BIT);  //VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT

		if (context.indices.graphicsFamily == -1)
		{
			return false;
		}

		std::set<int> uniqueQueueFamilies = { context.indices.graphicsFamily, context.indices.computeFamily, context.indices.transferFamily };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = CreateQueueCreateInfos(uniqueQueueFamilies);
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		if (vulkanInstance.physicalDeviceStruct.deviceFeatures.sampleRateShading == VK_TRUE)
		{
			deviceFeatures.sampleRateShading = VK_TRUE;
		}

		if (vulkanInstance.physicalDeviceStruct.deviceFeatures.geometryShader == VK_TRUE)
		{
			deviceFeatures.geometryShader = VK_TRUE;
		}
		else
		{
			throw VulkanBaseException("Geometry Shader are not supported by graphics device.");
		}

		if (vulkanInstance.physicalDeviceStruct.deviceFeatures.tessellationShader == VK_TRUE)
		{
			deviceFeatures.tessellationShader = VK_TRUE;
		}
		else
		{
			throw VulkanBaseException("Tesselation Shader are not supported by graphics device.");
		}

		if (vulkanInstance.physicalDeviceStruct.deviceFeatures.fillModeNonSolid == VK_TRUE)
		{
			deviceFeatures.fillModeNonSolid = VK_TRUE;
		}
		else
		{
			throw VulkanBaseException("Solid Fill Mode is not supported by graphics device.");
		}

		VkDeviceCreateInfo createInfo = CreateDeviceCreateInfo(vulkanInstance, queueCreateInfos, deviceFeatures);

		VkResult result = vkCreateDevice(vulkanInstance.physicalDeviceStruct.physicalDevice, &createInfo, nullptr, &(context.logicalDevice));
		if (result != VK_SUCCESS) {
			throw VulkanBaseException(result, "Unable to create a logical device");
		}

		vkGetDeviceQueue(context.logicalDevice, context.indices.graphicsFamily, 0, &context.graphicsQueue);
		//vkGetDeviceQueue(vulkanInstance.logicalDevice, indices.computeFamily, 0, &vulkanInstance.computeQueue);
		//vkGetDeviceQueue(vulkanInstance.logicalDevice, indices.transferFamily, 0, &vulkanInstance.transferQueue);
	}

	void VulkanBaseDevice::DestroyVulkanDevice(SVulkanContext& vulkanContext)
	{
		vkDestroyDevice(vulkanContext.logicalDevice, nullptr);
	}

	VkDeviceQueueCreateInfo VulkanBaseDevice::CreateDeviceQueueCreateInfo(int queueFamily) const noexcept
	{
		float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		return queueCreateInfo;
	}

	std::vector<VkDeviceQueueCreateInfo> VulkanBaseDevice::CreateQueueCreateInfos(
		const std::set<int>& uniqueQueueFamilies) const noexcept
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		for (int queueFamily : uniqueQueueFamilies) {
			if (queueFamily != -1)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = CreateDeviceQueueCreateInfo(queueFamily);
				queueCreateInfos.push_back(queueCreateInfo);
			}
		}
		return queueCreateInfos;
	}

	SQueueFamilyIndices VulkanBaseDevice::FindPresentQueueFamiliy(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface)
	{
		SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Error while attempting to check if a surface supports presentation:");
			}
			if (queueFamily.queueCount > 0 && presentSupport && VK_QUEUE_GRAPHICS_BIT) {
				vulkanContext.indices.presentFamily = i;
			}
			if (vulkanContext.indices.PresentFamilyIsSet()) {
				break;
			}
			++i;
		}
		return indices;

	}

	SQueueFamilyIndices VulkanBaseDevice::FindQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface)
	{
		SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Error while attempting to check if a surface supports presentation:");
			}
			if (queueFamily.queueCount > 0 && presentSupport) {
				vulkanContext.indices.presentFamily = i;
			}
			if (indices.GraphicsFamilyIsSet() && vulkanContext.indices.PresentFamilyIsSet()) {
				break;
			}
			++i;
		}
		return indices;
	}

	SQueueFamilyIndices VulkanBaseDevice::FindQueueFamilies(const VkPhysicalDevice& device, VkQueueFlags queueFlags)
	{
		SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		for (unsigned int i = 0; i < static_cast<uint32_t>(queueFamilies.size()); i++)
		{
			if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && !indices.GraphicsFamilyIsSet())
				{

					indices.graphicsFamily = i;
				}
			}
			if (queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && !indices.ComputeFamilyIsSet())
				{
					indices.computeFamily = i;
				}
			}

			if (queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) && !indices.TransferFamilyIsSet())
				{
					indices.transferFamily = i;
				}

			}

			if ((indices.GraphicsFamilyIsSet() || !(queueFlags & VK_QUEUE_GRAPHICS_BIT))
				&& (indices.TransferFamilyIsSet() || !(queueFlags & VK_QUEUE_TRANSFER_BIT))
				&& (indices.ComputeFamilyIsSet()) || !(queueFlags & VK_QUEUE_COMPUTE_BIT))
			{
				break;
			}
		}

		if (!indices.GraphicsFamilyIsSet() || !indices.TransferFamilyIsSet() || !indices.TransferFamilyIsSet())
		{
			for (unsigned int i = 0; i < static_cast<uint32_t>(queueFamilies.size()); i++)
			{
				if (queueFamilies[i].queueFlags & queueFlags)
				{
					if (!indices.GraphicsFamilyIsSet() && (queueFlags & VK_QUEUE_GRAPHICS_BIT) && !indices.GraphicsFamilyIsSet())
					{
						indices.graphicsFamily = i;
					}

					if (!indices.TransferFamilyIsSet() && (queueFlags & VK_QUEUE_TRANSFER_BIT) && indices.TransferFamilyIsSet())
					{
						indices.transferFamily = i;
					}

					if (!indices.ComputeFamilyIsSet() && (queueFlags & VK_QUEUE_COMPUTE_BIT) && !indices.ComputeFamilyIsSet())
					{
						indices.computeFamily = i;
					}

				}

				if ((indices.GraphicsFamilyIsSet() || !(queueFlags & VK_QUEUE_GRAPHICS_BIT))
					&& (indices.TransferFamilyIsSet() || !(queueFlags & VK_QUEUE_TRANSFER_BIT))
					&& (indices.ComputeFamilyIsSet()) || !(queueFlags & VK_QUEUE_COMPUTE_BIT))
				{
					break;
				}
			}
		}

		if (!(indices.GraphicsFamilyIsSet()) && (queueFlags & VK_QUEUE_GRAPHICS_BIT)
			|| !(indices.TransferFamilyIsSet()) && (queueFlags & VK_QUEUE_TRANSFER_BIT)
			|| !(indices.ComputeFamilyIsSet()) && (queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			throw Invision::VulkanBaseException("Could not find a matching Queue Family index");
		}

		return indices;
	}

	SQueueFamilyIndices VulkanBaseDevice::FindQueueFamilies(VkPhysicalDevice device) {
		SQueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
				indices.graphicsFamily = i;
			}

			if (indices.GraphicsFamilyIsSet()) {
				break;
			}

			i++;
		}

		return indices;
	}

}