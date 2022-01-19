#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseDevice.h"

namespace Invision
{

	void CreateVulkanDevice(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext)
	{
		VulkanBaseDevice().GetDevices(vulkanInstance, vulkanContext);
	}

	void DestroyVulkanDevice(SVulkanContext &vulkanContext)
	{
		vkDestroyDevice(vulkanContext.logicalDevice, nullptr);
	}


	VulkanBaseDevice::VulkanBaseDevice()
	{
		
	}

	void VulkanBaseDevice::GetDevices(SVulkanBase &vulkanInstance, SVulkanContext& vulkanContext)
	{
		//PickPhysicalDevice(vulkanInstance);
		CreateLogicalDevice(vulkanInstance, vulkanContext);
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



	void VulkanBaseDevice::CreateLogicalDevice(SVulkanBase& vulkanInstance, SVulkanContext& context)
	{
		context.indices = FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, VK_QUEUE_GRAPHICS_BIT);  //VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT
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

	/*bool VulkanBaseDevice::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{

		SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		return indices.GraphicsFamilyIsSet() & extensionsSupported && supportedFeatures.samplerAnisotropy;
	}


	


	bool VulkanBaseDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const
	{
		uint32_t extensionCount;
		VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Cannot retrieve count of properties for a physical device:");
		}
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Cannot retrieve properties for a physical device:");
		}
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}*/

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
}