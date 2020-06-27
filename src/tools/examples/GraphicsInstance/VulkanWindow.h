#pragma once
#include "wx\wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "VulkanCanvas.h"



class VulkanWindow : public wxFrame
{
public:
	VulkanWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~VulkanWindow();

private:
	void OnResize(wxSizeEvent& event);
	VulkanCanvas* m_canvas;
};