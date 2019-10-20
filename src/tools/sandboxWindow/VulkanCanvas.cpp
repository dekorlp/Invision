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
}

VulkanCanvas::~VulkanCanvas() noexcept
{
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