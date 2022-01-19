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

	bool VulkanBaseDevice::CreateSurface(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, HWND hwnd)
	{
		bool surfaceQueueWithGfxAvailable = false;

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
		for (unsigned int i = 0; i < vulkanContext.queueFamilies.size(); i++)
		{
			VkBool32 presentSupport;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(vulkanInstance.physicalDeviceStruct.physicalDevice, i, vulkanContext.surface, &presentSupport);
			if (presentSupport)
			{
				vulkanContext.queueFamilies[i].SetPresentQueueFlag(presentSupport);
			}

			if (presentSupport && vulkanContext.queueFamilies[i].GraphicsFamilyIsSet())
			{
				vkGetDeviceQueue(vulkanContext.logicalDevice, i, 0, &vulkanContext.presentQueue);
				surfaceQueueWithGfxAvailable = true;
			}
		}
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif

		return surfaceQueueWithGfxAvailable;
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
		//context.indices = FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, VK_QUEUE_GRAPHICS_BIT);  //VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT

		if (!QueryQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, context))
		{
			return false;
		}

		//std::set<int> uniqueQueueFamilies = { context.indices.graphicsFamily, context.indices.computeFamily, context.indices.transferFamily };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = CreateQueueCreateInfos(context.queueFamilies);
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

		//CreateDeviceQueues
		for (unsigned int i = 0; i < context.queueFamilies.size(); i++)
		{
			if (context.queueFamilies[i].GraphicsFamilyIsSet())
			{
				vkGetDeviceQueue(context.logicalDevice, i, 0, &context.graphicsQueue);
			}

			if (context.queueFamilies[i].TransferFamilyIsSet())
			{
				vkGetDeviceQueue(context.logicalDevice, i, 0, &context.transferQueue);
			}

			if (context.queueFamilies[i].ComputeFamilyIsSet())
			{
				vkGetDeviceQueue(context.logicalDevice, i, 0, &context.computeQueue);
			}
		}

		return true;
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
		std::vector<QueueFamily> queueFamilies) const noexcept
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		for (unsigned int i = 0; i < queueFamilies.size(); i++) {
		
			
			//if (queueFamilies[i].GraphicsFamilyIsSet() && queueFamilies[i].TransferFamilyIsSet() && queueFamilies[i].ComputeFamilyIsSet())
			//{
				VkDeviceQueueCreateInfo queueCreateInfo = CreateDeviceQueueCreateInfo(i);
				queueCreateInfos.push_back(queueCreateInfo);
			//}
		}
		return queueCreateInfos;
	}

	bool VulkanBaseDevice::QueryQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext)
	{
		bool graphicsFamilyIsSupported = false;
		//SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		for (const auto& queueFamily : queueFamilies)
		{
			QueueFamily family;

			family.SetFamilyFlags(queueFamily.queueFlags);
			family.SetQueueCount(queueFamily.queueCount);
			vulkanContext.queueFamilies.push_back(family);

			if (family.GraphicsFamilyIsSet())
			{
				graphicsFamilyIsSupported = true;
			}
		}

		return graphicsFamilyIsSupported;
	}
}