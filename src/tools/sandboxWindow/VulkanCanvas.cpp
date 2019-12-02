#include "VulkanCanvas.h"



VulkanCanvas::VulkanCanvas(wxWindow* pParent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	: wxWindow(pParent, id, pos, size, style, name)
{
	m_Size = size;
	Bind(wxEVT_PAINT, &VulkanCanvas::OnPaint, this);
	Bind(wxEVT_SIZE, &VulkanCanvas::OnResize, this);
	WXHWND hwnd = this->GetHandle(); // get window handle


	std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

	vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
	vulkanInstance.SetDebugMessanger(debugCallback);
	Invision::CreateSurface(vulkInstance, hwnd);
	Invision::CreateVulkanDevice(vulkInstance);
	Invision::CreatePresentationSystem(vulkInstance, size.GetWidth(), size.GetHeight());

	renderPass.AddAttachment(vulkInstance);
	renderPass.AddSubpass();
	renderPass.CreateRenderPass(vulkInstance);

	// Pipeline creation
	auto vertShaderCode = readFile(std::string(ROOT).append("/src/tools/sandboxWindow/Shader/vert.spv"));
	auto fragShaderCode	= readFile(std::string(ROOT).append("/src/tools/sandboxWindow/Shader/frag.spv"));

	Invision::VulkanShader vertShader(vulkInstance, vertShaderCode, VK_SHADER_STAGE_VERTEX_BIT);
	Invision::VulkanShader fragShader(vulkInstance, fragShaderCode, VK_SHADER_STAGE_FRAGMENT_BIT);

	pipeline.AddShader(vertShader);
	pipeline.AddShader(fragShader);
	pipeline.CreatePipeline(vulkInstance, renderPass, 0);
	vertShader.Destroy(vulkInstance);
	fragShader.Destroy(vulkInstance);

	framebuffer.CreateFramebuffer(vulkInstance, renderPass);
	
}

VulkanCanvas::~VulkanCanvas() noexcept
{
	framebuffer.DestroyFramebuffer(vulkInstance);
	pipeline.DestroyPipeline(vulkInstance);
	renderPass.DestroyRenderPass(vulkInstance);
	Invision::DestroyPresentationSystem(vulkInstance);
	Invision::DestroyVulkanDevice(vulkInstance);
	Invision::DestroySurface(vulkInstance);
	vulkanInstance.Destroy();
}

void VulkanCanvas::OnPaint(wxPaintEvent& event)
{
	
}

void VulkanCanvas::OnResize(wxSizeEvent& event)
{

	wxSize size = m_Size;
	if (size.GetWidth() == 0 || size.GetHeight() == 0) {
		return;
	}

	wxRect refreshRect(size);

	RefreshRect(refreshRect, false);
}

void VulkanCanvas::OnPaintException(const std::string& msg)
{
	wxMessageBox(msg, "Vulkan Error");
	wxTheApp->ExitMainLoop();
}