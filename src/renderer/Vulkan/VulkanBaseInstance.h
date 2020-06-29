#ifndef VULKAN_BASE_INSTANCE_H
#define VULKAN_BASE_INSTANCE_H

#include "vulkan\vulkan.h"
namespace Invision
{
	class  VulkanBaseInstance
	{
	public:
		INVISION_API VulkanBaseInstance();
		INVISION_API SVulkanBase Init(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions);
		INVISION_API void SetDebugMessanger(void(*debugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = nullptr);

		INVISION_API void Destroy();
	private:
		INVISION_API void CreateInstance(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions);
		INVISION_API void CheckExtensions(std::vector<const char*> &requiredExtensions);
		INVISION_API bool CheckValidationLayerSupport(std::vector<const char*> validationLayers);
		INVISION_API void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		INVISION_API VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		INVISION_API void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		INVISION_API static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		VkInstance mInstance;

		VkDebugUtilsMessengerEXT mDebugMessanger;

		bool mEnableValidationLayers;

		std::vector<const char*> mValidationLayers;
		

	};
}

#endif // VULKAN_BASE_INSTANCE_H