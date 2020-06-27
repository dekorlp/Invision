#include "VulkanWindow.h"


VulkanWindow::VulkanWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, id, title, pos, size), m_canvas(nullptr)
{
	Bind(wxEVT_SIZE, &VulkanWindow::OnResize, this);

	m_canvas = new VulkanCanvas(this, wxID_ANY, wxDefaultPosition, { 800,600 });
	Fit();

}

VulkanWindow::~VulkanWindow()
{
}


void VulkanWindow::OnResize(wxSizeEvent& event)
{
	wxSize clientSize = GetClientSize();
	m_canvas->SetSize(clientSize);
}
