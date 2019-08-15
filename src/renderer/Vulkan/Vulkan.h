#ifndef VULKAN_H
#define VULKAN_H

#include "vulkan\vulkan.h"
#include "VulkanException.h"
namespace Invision
{
	class INVISION_API Vulkan
	{
	public:
		Vulkan();
		void Init();
		void SetDebugMessanger(void(*debugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = nullptr);

		void Destroy();
	private:
		void CreateInstance();
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

#endif //VULKAN_H