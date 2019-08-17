#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include "vulkan\vulkan.h"
#include "VulkanException.h"
#include "Vulkan.h"
namespace Invision
{
	class INVISION_API VulkanInstance
	{
	public:
		VulkanInstance();
		SVulkan Init(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions);
		void SetDebugMessanger(void(*debugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = nullptr);

		void Destroy();
	private:
		void CreateInstance(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions);
		void CheckExtensions(std::vector<const char*> &requiredExtensions);
		bool CheckValidationLayerSupport(std::vector<const char*> validationLayers);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		VkInstance mInstance;

		VkDebugUtilsMessengerEXT mDebugMessanger;

		bool mEnableValidationLayers;
		

	};
}

#endif // VULKAN_INSTANCE_H