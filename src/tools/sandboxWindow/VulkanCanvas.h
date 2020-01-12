#pragma once

#include "wx\wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#define ROOT "../../../"

#include "AdditionalFunctions.h"
#include "InCommon.h"
#include "renderer\Vulkan\Vulkan.h"
#include "renderer\Vulkan\VulkanException.h"
#include "renderer\Vulkan\VulkanInstance.h"
#include "renderer\Vulkan\VulkanDevice.h"
#include "renderer\Vulkan\VulkanPresentation.h"
#include "renderer\Vulkan\VulkanShader.h"
#include "renderer\Vulkan\VulkanPipeline.h"
#include "renderer\Vulkan\VulkanRenderPass.h"
#include "renderer\Vulkan\VulkanFramebuffer.h"
#include "renderer\Vulkan\VulkanCommandPool.h"
#include "renderer\Vulkan\VulkanCommandBuffer.h"
#include "renderer\Vulkan\VulkanRenderer.h"
#include "renderer\Vulkan\VulkanVertexBuffer.h"
#include "math\Vector2.h"
#include "math\Vector3.h"



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
	{ { 0.0f, -0.5f },{ 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
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
	void Render();

	wxSize m_Size;

	Invision::VulkanInstance vulkanInstance;
	Invision::VulkanPipeline pipeline;
	Invision::VulkanRenderPass renderPass;
	Invision::VulkanFramebuffer framebuffer;
	Invision::VulkanCommandPool commandPool;
	Invision::VulkanCommandBuffer commandBuffer;
	Invision::VulkanRenderer renderer;

	Invision::VulkanVertexBuffer vertexBuffer;
	VkPipelineCache mCache;
	Invision::SVulkan vulkInstance;
	wxTimer m_timer;
};