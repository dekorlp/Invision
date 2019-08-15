#include "precompiled.h"

#include "Vulkan.h"

namespace Invision
{

	void Vulkan::Init()
	{
		CreateInstance();
	}

	void Vulkan::Destroy()
	{
		vkDestroyInstance(mVkInstance, nullptr);
	}

	void Vulkan::CreateInstance()
	{
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

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Hello World";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Invision";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
		CheckExtensions(requiredExtensions);

		VkInstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;

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

}