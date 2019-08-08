#include "precompiled.h"

#include "Vulkan.h"

Vulkan::Vulkan() 
	: m_vulkanInitialized(false), m_instance(VK_NULL_HANDLE),
	m_surface(VK_NULL_HANDLE), m_physicalDevice(VK_NULL_HANDLE)
	//m_commandPool(VK_NULL_HANDLE), m_imageAvailableSemaphore(VK_NULL_HANDLE),
	//m_renderFinishedSemaphore(VK_NULL_HANDLE)
{
	m_logicalDevice = { };
	m_swapchain = { };
	m_pipeline = { };
}

void Vulkan::run(HWND hwnd, const int width, const int height)
{
	//-------------------------------------------------------------
	// for debug create Shader

	ShaderType vertexShaderShaderType;
	vertexShaderShaderType.shaderName = "vertShader";
	vertexShaderShaderType.shaderPath = "Shader/vert.spv";
	vertexShaderShaderType.shaderState = VK_SHADER_STAGE_VERTEX_BIT;

	ShaderType fragmentShaderShaderType;
	fragmentShaderShaderType.shaderName = "fragShader";
	fragmentShaderShaderType.shaderPath = "Shader/frag.spv";
	fragmentShaderShaderType.shaderState = VK_SHADER_STAGE_FRAGMENT_BIT;

	ShaderType vertexTestShaderType;
	vertexTestShaderType.shaderName = "vertShader";
	vertexTestShaderType.shaderPath = "Shader/test_vert.spv";
	vertexTestShaderType.shaderState = VK_SHADER_STAGE_VERTEX_BIT;

	ShaderType fragmentTestShaderType;
	fragmentTestShaderType.shaderName = "fragShader";
	fragmentTestShaderType.shaderPath = "Shader/test_frag.spv";
	fragmentTestShaderType.shaderState = VK_SHADER_STAGE_FRAGMENT_BIT;

	

	const std::vector<Vertex> vertices = {
		{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
	};


	ShaderPipeline shaderShaderPipeline1;
	shaderShaderPipeline1.pipeline = VK_NULL_HANDLE;
	shaderShaderPipeline1.pipelineLayout = VK_NULL_HANDLE;
	shaderShaderPipeline1.draw = true;
	shaderShaderPipeline1.shaderType.push_back(vertexShaderShaderType);
	shaderShaderPipeline1.shaderType.push_back(fragmentShaderShaderType);
	shaderShaderPipeline1.vertexSource = IN_VERTEX_SOURCE_STRUCT;
	shaderShaderPipeline1.meshData.meshVertizes = vertices;
	shaderShaderPipeline1.meshData.vertexBufferMemory = VK_NULL_HANDLE;
	shaderShaderPipeline1.meshData.VertexBuffer = VK_NULL_HANDLE;

	

	this->CreateShaderPipeline(shaderShaderPipeline1);

	ShaderPipeline testShaderPipeline2;
	testShaderPipeline2.pipeline = VK_NULL_HANDLE;
	testShaderPipeline2.pipelineLayout = VK_NULL_HANDLE;
	testShaderPipeline2.draw = true;
	testShaderPipeline2.shaderType.push_back(vertexTestShaderType);
	testShaderPipeline2.shaderType.push_back(fragmentTestShaderType);
	testShaderPipeline2.vertexSource = IN_VERTEX_SOURCE_SHADER;
	testShaderPipeline2.shaderCountOfVertizes = 3;

	
	this->CreateShaderPipeline(testShaderPipeline2);

	//-------------------------------------------------------------

	std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
	initVulkan(requiredExtensions);
	VkApplicationInfo appInfo = CreateApplicationInfo("VulkanApp1", VK_MAKE_VERSION(1, 0, 0), "C", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);
	std::vector<const char*> layerNames;
	if (enableValidationLayers) {
		layerNames = validationLayers;
	}
	VkInstanceCreateInfo createInfo = CreateInstanceCreateInfo(appInfo, requiredExtensions, layerNames);
	CreateInstance(createInfo);

	vulkanDevice.CreateWindowSurface(m_instance, hwnd, &m_surface);
	vulkanDevice.PickPhysicalDevice(m_instance, &m_physicalDevice);
	vulkanDevice.CreateLogicalDevice(m_instance, m_physicalDevice, &m_logicalDevice);

	vulkanPresentation.CreateSwapChain(m_physicalDevice, m_logicalDevice, m_surface, &m_swapchain, width, height);
	vulkanPresentation.CreateImageViews(m_logicalDevice, &m_swapchain);

	vulkanPipeline.CreateRenderPass(m_logicalDevice, &m_pipeline, &m_swapchain);
	vulkanPipeline.CreateGraphicsPipeline(m_logicalDevice, &m_ShaderPipelines, &m_pipeline);


	vulkanDrawing.CreateFrameBuffers(m_logicalDevice, m_surface, &m_swapchain, &m_pipeline);
	vulkanDrawing.CreateCommandPool(m_physicalDevice, m_logicalDevice.m_logicalDevice);
	
	vulkanMemoryBuffer.createVertexBuffer(m_physicalDevice, m_logicalDevice, &m_ShaderPipelines);
	vulkanDrawing.CreateCommandBuffers(m_logicalDevice, &m_commandBuffers, &m_pipeline, &m_ShaderPipelines);
	vulkanDrawing.CreateSemaphores(m_logicalDevice, &m_imageAvailableSemaphore);
}

void Vulkan::Destroy() noexcept
{
	if (m_instance != VK_NULL_HANDLE) {
		if (m_logicalDevice.m_logicalDevice != VK_NULL_HANDLE) {
			vkDeviceWaitIdle(m_logicalDevice.m_logicalDevice);
			
			vulkanMemoryBuffer.cleanUpBuffers(m_logicalDevice, &m_ShaderPipelines);

			vulkanPipeline.CleanupPipeline(&m_ShaderPipelines);

			vulkanPresentation.CleanupPresentation(m_logicalDevice);

			vulkanDrawing.cleanUpFrameBuffers(m_logicalDevice);

			vulkanDrawing.cleanUpCommandBuffers(m_logicalDevice);

			vulkanDrawing.cleanUpSamaphores(m_logicalDevice);

			vkDestroyDevice(m_logicalDevice.m_logicalDevice, nullptr);
		}
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}
}

void Vulkan::initVulkan(std::vector<const char*> requiredExtensions)
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
				requiredExtensions.erase(iter);
				break;
			}
		}
	};
	if (!requiredExtensions.empty()) {
		std::stringstream ss;
		ss << "The following required Vulkan extensions could not be found:\n";
		for (unsigned int extNum = 0; extNum < requiredExtensions.size(); ++extNum) {
			ss << requiredExtensions[extNum] << "\n";
		}
		ss << "Program cannot continue.";
		throw std::runtime_error(ss.str());
	}

	m_vulkanInitialized = true;
}

void Vulkan::CreateInstance(const VkInstanceCreateInfo& createInfo)
{
	if (!m_vulkanInitialized) {
		throw std::runtime_error("Programming Error:\nAttempted to create a Vulkan instance before Vulkan was initialized.");
	}
	VkResult err = vkCreateInstance(&createInfo, nullptr, &m_instance);
	if (err != VK_SUCCESS) {
		throw VulkanException(err, "Unable to create a Vulkan instance:");
	}
}

VkApplicationInfo Vulkan::CreateApplicationInfo(const std::string& appName,
	const int32_t appVersion,
	const std::string& engineName,
	const int32_t engineVersion,
	const int32_t apiVersion) const noexcept
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = appVersion;
	appInfo.pEngineName = engineName.c_str();
	appInfo.engineVersion = engineVersion;
	appInfo.apiVersion = apiVersion;
	return appInfo;
}

VkInstanceCreateInfo Vulkan::CreateInstanceCreateInfo(const VkApplicationInfo& appInfo,
	const std::vector<const char*>& extensionNames,
	const std::vector<const char*>& layerNames) const noexcept
{
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
	createInfo.ppEnabledExtensionNames = extensionNames.data();
	createInfo.enabledLayerCount = static_cast<uint32_t>(layerNames.size());
	createInfo.ppEnabledLayerNames = layerNames.data();
	return createInfo;
}

#ifdef _WIN32
VkWin32SurfaceCreateInfoKHR Vulkan::CreateWin32SurfaceCreateInfo(HWND hwnd) const noexcept
{
	VkWin32SurfaceCreateInfoKHR sci = {};
	sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	sci.hwnd = hwnd;
	sci.hinstance = GetModuleHandle(NULL);
	return sci;
}
#endif

	void Vulkan::RecreateSwapchain(const int width, const int height)
	{
		vkDeviceWaitIdle(m_logicalDevice.m_logicalDevice);

		cleanupSwapChain();

		vulkanPresentation.CreateSwapChain(m_physicalDevice, m_logicalDevice, m_surface, &m_swapchain, width, height);
		vulkanPresentation.CreateImageViews(m_logicalDevice, &m_swapchain);

		vulkanPipeline.CreateRenderPass(m_logicalDevice, &m_pipeline, &m_swapchain);
		vulkanPipeline.CreateGraphicsPipeline(m_logicalDevice, &m_ShaderPipelines, &m_pipeline);

		vulkanDrawing.CreateFrameBuffers(m_logicalDevice, m_surface, &m_swapchain, &m_pipeline);
		vulkanDrawing.CreateCommandBuffers(m_logicalDevice, &m_commandBuffers, &m_pipeline, &m_ShaderPipelines);
	}

	void Vulkan::OnPaint(const int width, const int height)
	{
			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(m_logicalDevice.m_logicalDevice, m_swapchain.m_swapchain,
				std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				RecreateSwapchain(width, height);
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw VulkanException(result, "Failed to acquire swap chain image");
			}
			VkPipelineStageFlags waitFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			VkSubmitInfo submitInfo = vulkanDrawing.CreateSubmitInfo(imageIndex, waitFlags);
			result = vkQueueSubmit(m_logicalDevice.m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			if (result != VK_SUCCESS) {
				throw VulkanException(result, "Failed to submit draw command buffer:");
			}

			VkPresentInfoKHR presentInfo = vulkanDrawing.CreatePresentInfoKHR(imageIndex, &m_swapchain);
			result = vkQueuePresentKHR(m_logicalDevice.m_presentQueue, &presentInfo);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				RecreateSwapchain(width, height);
			}
			else if (result != VK_SUCCESS) {
				throw VulkanException(result, "Failed to present swap chain image:");
			}
	}

	void Vulkan::cleanupSwapChain()
	{
		if (m_instance != VK_NULL_HANDLE) {
			if (m_logicalDevice.m_logicalDevice != VK_NULL_HANDLE) {
				vkDeviceWaitIdle(m_logicalDevice.m_logicalDevice);

				vulkanPipeline.CleanupPipeline(&m_ShaderPipelines);
				vulkanPresentation.CleanupPresentation(m_logicalDevice);
				vulkanDrawing.cleanUpFrameBuffers(m_logicalDevice);
			}
		}
	}

void Vulkan::CreateShaderPipeline(ShaderPipeline shaderPipeline)
{
	m_ShaderPipelines.push_back(shaderPipeline);
}