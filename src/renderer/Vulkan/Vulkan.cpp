#include "precompiled.h"

#include "Vulkan.h"

namespace Invision
{
	namespace {
		static void(*invisionDebugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
	}
	
	VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		if (invisionDebugFunc != nullptr)
		{
			invisionDebugFunc(messageSeverity, messageType, pCallbackData);
		}
		else
		{
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}
		return VK_FALSE;
	}

	void Vulkan::Init()
	{
		CreateInstance();
	}

	void Vulkan::SetDebugMessanger(void(*debugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData))
	{
		if (!enableValidationLayers) return;

		if (debugFunc != nullptr)
		{
			invisionDebugFunc = debugFunc;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(mVkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}

	}

	void Vulkan::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void Vulkan::Destroy()
	{
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(mVkInstance, debugMessenger, nullptr);
		}

		vkDestroyInstance(mVkInstance, nullptr);
	}

	void Vulkan::CreateInstance()
	{

		mVkInstance = nullptr;

		// make sure that the Vulkan library is available on this system
#ifdef _WIN32
		HMODULE vulkanModule = ::LoadLibraryA("vulkan-1.dll");
		if (vulkanModule == NULL) {
			throw std::runtime_error("Vulkan library is not available on this system, so program cannot run.\n"
				"You must install the appropriate Vulkan library and also have a graphics card that supports Vulkan.");
		}
#else
#error Only Win32 is currently supported. To see how to support other windowing systems, \
 see the definition of _glfw_dlopen in XXX_platform.h and its use in vulkan.c in the glfw\
 source code. XXX specifies the windowing system (e.g. x11 for X11, and wl for Wayland).
#endif

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Hello World";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Invision";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		// extension is Required!
		if (enableValidationLayers)
		{
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		CheckExtensions(requiredExtensions);

		VkInstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

		if (enableValidationLayers && !CheckValidationLayerSupport(validationLayers))
		{
			throw VulkanException("validation layers requested, but not available!");
		}


		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers)
		{
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
			
			populateDebugMessengerCreateInfo(debugCreateInfo);
			instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.ppEnabledLayerNames = nullptr;

			instanceCreateInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &mVkInstance) != VK_SUCCESS)
		{
			throw VulkanException("failed to create instance!");
		}
	}

	void Vulkan::CheckExtensions(std::vector<const char*> &requiredExtensions)
	{
	
		uint32 numExtensions = 0;

		// make sure that the correct extensions are available
		uint32_t count;
		VkResult err = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		if (err != VK_SUCCESS) {
			throw VulkanException(err, "Failed to retrieve the instance extension properties:");
		}
		std::vector<VkExtensionProperties> extensions(count);
		err = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
		if (err != VK_SUCCESS) {
			throw VulkanException(err, "Failed to retrieve the instance extension properties:");
		}
		for (unsigned int extNum = 0; extNum < extensions.size(); ++extNum) {
			for (auto iter = requiredExtensions.begin(); iter < requiredExtensions.end(); ++iter) {
				if (std::string(*iter) == extensions[extNum].extensionName) {
					//requiredExtensions.erase(iter);
					numExtensions++;
					break;
				}
			}
		};
		if (numExtensions < requiredExtensions.size()) {
			std::stringstream ss;
			ss << "The following required Vulkan extensions could not be found:\n";
			for (unsigned int extNum = 0; extNum < requiredExtensions.size(); ++extNum) {
				ss << requiredExtensions[extNum] << "\n";
			}
			ss << "Program cannot continue.";
			throw std::runtime_error(ss.str());
		}
	}

	bool Vulkan::CheckValidationLayerSupport(std::vector<const char*> validationLayers)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	VkResult Vulkan::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void Vulkan::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}


}