#include "precompiled.h"

#include "VulkanQueueFamily.h"

VkDeviceQueueCreateInfo VulkanQueueFamily::CreateDeviceQueueCreateInfo(int queueFamily) const noexcept
{
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamily;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	return queueCreateInfo;
}

SQueueFamilyIndices VulkanQueueFamily::FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR surface) const
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
			throw VulkanException(result, "Error while attempting to check if a surface supports presentation:");
		}
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}
		if (indices.IsComplete()) {
			break;
		}
		++i;
	}
	return indices;
}

std::vector<VkDeviceQueueCreateInfo> VulkanQueueFamily::CreateQueueCreateInfos(
	const std::set<int>& uniqueQueueFamilies) const noexcept
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = CreateDeviceQueueCreateInfo(queueFamily);
		queueCreateInfos.push_back(queueCreateInfo);
	}
	return queueCreateInfos;
}