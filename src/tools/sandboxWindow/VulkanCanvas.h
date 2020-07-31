#pragma once

#include "wx\wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <chrono>

#include "AdditionalFunctions.h"
#include "InCommon.h"
#include "renderer\Vulkan\VulkanBase.h"
#include "renderer\Vulkan\VulkanBaseException.h"
#include "renderer\Vulkan\VulkanBaseInstance.h"
#include "renderer\Vulkan\VulkanBaseDevice.h"
#include "renderer\Vulkan\VulkanBasePresentation.h"
#include "renderer\Vulkan\VulkanBaseShader.h"
#include "renderer\Vulkan\VulkanBasePipeline.h"
#include "renderer\Vulkan\VulkanBaseRenderPass.h"
#include "renderer\Vulkan\VulkanBaseFramebuffer.h"
#include "renderer\Vulkan\VulkanBaseCommandPool.h"
#include "renderer\Vulkan\VulkanBaseCommandBuffer.h"
#include "renderer\Vulkan\VulkanBaseRenderer.h"
#include "renderer\Vulkan\VulkanBaseVertexBuffer.h"
#include "renderer\Vulkan\VulkanBaseIndexBuffer.h"
#include "renderer\Vulkan\VulkanBaseUniformBuffer.h"
#include "renderer\Vulkan\VulkanBaseDescriptorPool.h"
#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Matrix.h"



static void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	std::stringstream ss;
	ss << "validation layer: " << pCallbackData->pMessage;
	INVISION_LOG_RAWTEXT(ss.str());
}

struct Vertex {
	Invision::Vector2 position;
	Invision::Vector3 color;
};

const std::vector<Vertex> vertices = {
	{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
Invision::Matrix model;
Invision::Matrix view;
Invision::Matrix proj;
};

class VulkanCanvas : public wxWindow
{
public:
	VulkanCanvas(wxWindow* pParent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = "No Name");

	virtual ~VulkanCanvas() noexcept;

private:

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnResize(wxSizeEvent& event);
	void OnPaintException(const std::string& msg);
	void RecreateSwapChain(const int width, const int height);
	void BuildCommandBuffer(float width, float height);
	void UpdateUniformBuffer(float width, float height);
	void Render();

	wxSize m_Size;

	Invision::VulkanBaseInstance vulkanInstance;
	Invision::VulkanBasePipeline pipeline;
	Invision::VulkanBaseRenderPass renderPass;
	Invision::VulkanBaseFramebuffer framebuffer;
	Invision::VulkanBaseCommandPool commandPool;
	Invision::VulkanBaseCommandBuffer commandBuffer;
	Invision::VulkanBaseRenderer renderer;

	Invision::VulkanBaseVertexBuffer vertexBuffer;
	Invision::VulkanBaseIndexBuffer indexBuffer;
	Invision::VulkanBaseUniformBuffer uniformBuffer;

	VkPipelineCache mCache;
	Invision::SVulkanBase vulkInstance;
	Invision::SVulkanContext vulkanContext;
	wxTimer m_timer;
};