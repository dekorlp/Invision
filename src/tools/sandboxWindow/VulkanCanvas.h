#pragma once

#include "wx\wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "InCommon.h"
#include "renderer\Vulkan\Vulkan.h"
#include "renderer\Vulkan\VulkanInstance.h"
#include "renderer\Vulkan\VulkanDevice.h"


static void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	std::stringstream ss;
	ss << "validation layer: " << pCallbackData->pMessage;
	INVISION_LOG_RAWTEXT(ss.str());
}


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
	virtual void OnResize(wxSizeEvent& event);
	void OnPaintException(const std::string& msg);

	wxSize m_Size;

	Invision::VulkanInstance vulkanInstance;
};