#ifndef VULKAN_H
#define VULKAN_H

#include "vulkan\vulkan.h"
#include "VulkanException.h"
namespace Invision
{
	class INVISION_API Vulkan
	{
	public:
		void Init();
		void SetDebugMessanger();

		void Destroy();
	private:
		void CreateInstance();
		void CheckExtensions(std::vector<const char*> &requiredExtensions);
		bool CheckValidationLayerSupport(std::vector<const char*> validationLayers);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkInstance mVkInstance;

		VkDebugUtilsMessengerEXT debugMessenger;


#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

	};
}

#endif //VULKAN_H