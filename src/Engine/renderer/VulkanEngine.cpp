#include "precompiled.h"

#include "VulkanInstance.h"
#include "VulkanRender.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandBuffer.h"

#include "VulkanEngine.h"



namespace Invision
{


	VulkanEngine::VulkanEngine() :
		IGraphicsEngine(EngineType::Vulkan, "Vulkan", "1.2.137")
	{
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
		//vulkanInstance.SetDebugMessanger(nullptr, VulkanDebug::debugCallback);
		
		
	}

	VulkanEngine::VulkanEngine(std::ofstream* ofstr) :
		IGraphicsEngine(EngineType::Vulkan, "Vulkan", "1.2.137")
	{
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions, true);
		vulkanInstance.SetDebugMessanger(ofstr, VulkanDebug::debugCallback);


	}

	void VulkanEngine::Init()
	{
		Invision::VulkanBaseDevice().PickPhysicalDevice(vulkInstance);
		deviceProperties = ConvertPhysicalDeviceParameters(vulkInstance.physicalDeviceStruct);
		Invision::VulkanBaseDevice().CreateLogicalDevice(vulkInstance);

		commandPool.CreateCommandPool(vulkInstance);
	}

	void VulkanEngine::Init(unsigned int index)
	{
		Invision::VulkanBaseDevice().PickPhysicalDevice(vulkInstance, index);
		deviceProperties = ConvertPhysicalDeviceParameters(vulkInstance.physicalDeviceStruct);
		Invision::VulkanBaseDevice().CreateLogicalDevice(vulkInstance);

		commandPool.CreateCommandPool(vulkInstance);
	}

	std::shared_ptr<IGraphicsInstance> VulkanEngine::CreateInstance(CanvasDimensions canvas, bool activateDepthTest)
	{
		return  std::make_shared<VulkanInstance>(this, canvas, activateDepthTest);
	}

	Invision::SVulkanBase& VulkanEngine::GetVulkanInstance()
	{
		return vulkInstance;
	}

	Invision::VulkanBaseCommandPool VulkanEngine::GetCommandPool()
	{
		return commandPool;
	}

	PhysicalDeviceProperties VulkanEngine::ConvertPhysicalDeviceParameters(SVulkanBasePhysicalDevice physicalDeviceStruct)
	{
		PhysicalDeviceProperties locDeviceProperties;

		locDeviceProperties.apiVersion = physicalDeviceStruct.deviceProperties.apiVersion;
		locDeviceProperties.deviceID = physicalDeviceStruct.deviceProperties.deviceID;
		strcpy_s(locDeviceProperties.deviceName, 256, physicalDeviceStruct.deviceProperties.deviceName);

		switch (physicalDeviceStruct.deviceProperties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			locDeviceProperties.deviceType = PHYSICAL_DEVICE_TYPE_OTHER;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			locDeviceProperties.deviceType = PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			locDeviceProperties.deviceType = PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			locDeviceProperties.deviceType = PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			locDeviceProperties.deviceType = PHYSICAL_DEVICE_TYPE_CPU;
			break;
		}

		locDeviceProperties.driverVersion = physicalDeviceStruct.deviceProperties.driverVersion;
		locDeviceProperties.index = physicalDeviceStruct.index;
		locDeviceProperties.vendorID = physicalDeviceStruct.deviceProperties.vendorID;


		switch (physicalDeviceStruct.deviceProperties.vendorID)
		{
		case GPU::GPUDeviceVendor::AMD:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::AMD;
				break;
		case GPU::GPUDeviceVendor::ARM:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::ARM;
			break;
		case GPU::GPUDeviceVendor::IMGTEC:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::IMGTEC;
			break;
		case GPU::GPUDeviceVendor::INTEL:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::INTEL;
			break;
		case GPU::GPUDeviceVendor::NVIDIA:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::NVIDIA;
			break;
		case GPU::GPUDeviceVendor::QUALCOMM:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::QUALCOMM;
			break;
		default:
			locDeviceProperties.vendorType = GPU::GPUDeviceVendor::UNKNOWN;
		}

		return locDeviceProperties;
	}

	std::vector< PhysicalDeviceProperties> VulkanEngine::GetPhysicalDevices()
	{
		std::vector< PhysicalDeviceProperties> physicalDevices;

		if (!vulkInstance.instance) {
			throw InvisionBaseRendererException("Programming Error:\n"
				"Attempted to get a Vulkan physical device before the Vulkan instance was created.");
		}
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkInstance.instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw InvisionBaseRendererException("Failed to find a GPU with Vulkan support.");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkInstance.instance, &deviceCount, devices.data());
		int index = 0;
		for (const auto& device : devices) {
			SVulkanBasePhysicalDevice deviceProperties;

			vkGetPhysicalDeviceProperties(device, &(deviceProperties.deviceProperties));
			vkGetPhysicalDeviceFeatures(device, &deviceProperties.deviceFeatures);
			deviceProperties.index = index;
			physicalDevices.push_back(ConvertPhysicalDeviceParameters(deviceProperties));
			
			index++;
		}

		return physicalDevices;
	}


	VulkanEngine::~VulkanEngine()
	{
		commandPool.DestroyCommandPool(vulkInstance);
		//Invision::DestroyPresentationSystem(vulkInstance, vulkanContext);
		Invision::DestroyVulkanDevice(vulkInstance);
		//Invision::DestroySurface(vulkInstance, vulkanContext);
		vulkanInstance.Destroy();
	}

}