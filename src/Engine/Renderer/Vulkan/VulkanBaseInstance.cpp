#include "precompiled.h"

#include "VulkanBaseException.h"
#include "VulkanBase.h"

#include "VulkanBaseInstance.h"

namespace Invision
{
	namespace {
		static void(*invisionDebugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	}
	
	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanBaseInstance::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		if (invisionDebugFunc != nullptr)
		{
			invisionDebugFunc(messageSeverity, messageType, pCallbackData, pUserData);
		}
		else
		{
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}
		return VK_FALSE;
	}

	VulkanBaseInstance::VulkanBaseInstance() : mInstance(nullptr)
	{

	}

	SVulkanBase VulkanBaseInstance::Init(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions, bool debug)
	{
		mEnableValidationLayers = debug;

		CreateInstance(appName, engineName, appVersion, engineVersion, extensions);
		SVulkanBase vulkStruct;
		vulkStruct.enableValidationLayers = mEnableValidationLayers;
		vulkStruct.instance = mInstance;
		vulkStruct.validationLayers = mValidationLayers;
		return vulkStruct;
	}

	void VulkanBaseInstance::SetDebugMessanger(std::ofstream* ofstr, void(*debugFunc)(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData))
	{
		if (!mEnableValidationLayers) return;

		if (debugFunc != nullptr)
		{
			invisionDebugFunc = debugFunc;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo, ofstr);
		mOfstr = ofstr;

		if (CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessanger) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to set up debug messenger!");
		}

	}

	void VulkanBaseInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, std::ofstream* ofstr) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = ofstr;
	}

	void VulkanBaseInstance::Destroy()
	{
		if (mEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessanger, nullptr);
		}

		vkDestroyInstance(mInstance, nullptr);
	}

	void VulkanBaseInstance::CreateInstance(const char* appName, const char* engineName, const uint32_t appVersion, const uint32_t engineVersion, const std::vector<const char*> &extensions)
	{

		mInstance = nullptr;

		// make sure that the Vulkan library is available on this system
#ifdef _WIN32
		HMODULE vulkanModule = ::LoadLibraryA("vulkan-1.dll");
		if (vulkanModule == NULL) {
			throw InvisionBaseRendererException("Vulkan library is not available on this system, so program cannot run.\n"
				"You must install the appropriate Vulkan library and also have a graphics card that supports Vulkan.");
		}
#else
#error Only Win32 is currently supported. To see how to support other windowing systems, \
 see the definition of _glfw_dlopen in XXX_platform.h and its use in vulkan.c in the glfw\
 source code. XXX specifies the windowing system (e.g. x11 for X11, and wl for Wayland).
#endif

		mValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = appName;
		appInfo.applicationVersion = appVersion;
		appInfo.pEngineName = engineName;
		appInfo.engineVersion = engineVersion;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		std::vector<const char*> requiredExtensions = extensions;

		//std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		// extension is Required!
		if (mEnableValidationLayers)
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

		if (mEnableValidationLayers && !CheckValidationLayerSupport(mValidationLayers))
		{
			throw InvisionBaseRendererException("validation layers requested, but not available!");
		}


		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (mEnableValidationLayers)
		{
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
			
			PopulateDebugMessengerCreateInfo(debugCreateInfo, mOfstr);
			instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.ppEnabledLayerNames = nullptr;

			instanceCreateInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) != VK_SUCCESS)
		{
			throw InvisionBaseRendererException("failed to create instance!");
		}
	}

	void VulkanBaseInstance::CheckExtensions(std::vector<const char*> &requiredExtensions)
	{
	
		uint32 numExtensions = 0;

		// make sure that the correct extensions are available
		uint32_t count;
		VkResult err = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		if (err != VK_SUCCESS) {
			throw VulkanBaseException(err, "Failed to retrieve the instance extension properties:");
		}
		std::vector<VkExtensionProperties> extensions(count);
		err = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
		if (err != VK_SUCCESS) {
			throw VulkanBaseException(err, "Failed to retrieve the instance extension properties:");
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
			const std::string tmp = ss.str();
			throw InvisionBaseRendererException(tmp.c_str());
		}
	}

	bool VulkanBaseInstance::CheckValidationLayerSupport(std::vector<const char*> validationLayers)
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

	VkResult VulkanBaseInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanBaseInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}


}