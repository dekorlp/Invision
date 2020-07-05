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

	//vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
	//vulkanInstance.SetDebugMessanger(debugCallback);

	Invision::CanvasDimensions dim = { hwnd, size.GetWidth(), size.GetHeight() };
	//graphicsEngine = std::make_shared<Invision::VulkanEngine>(dim);
	graphicsEngine = Invision::create_engine(Invision::EngineType::Vulkan, dim);
	renderPass = graphicsEngine->CreateRenderPass();
	vertexBuffer = graphicsEngine->CreateVertexBuffer();
	uniformBuffer = graphicsEngine->CreateUniformBuffer();
	indexBuffer = graphicsEngine->CreateIndexBuffer();


	vertexBuffer->CreateVertexBuffer(sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);
	vertexBuffer->CreateVertexInput(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
		.CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
		.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

	indexBuffer->CreateIndexBuffer(sizeof(indices[0]) * indices.size(), indices.data(), 0);

	uniformBuffer->CreateUniformBinding(0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 0).CreateUniformBuffer();

	m_timer.SetOwner(this);
	m_timer.Start(5);
	this->Bind(wxEVT_TIMER, &VulkanCanvas::OnTimer, this);
}


VulkanCanvas::~VulkanCanvas() noexcept
{
	//vulkanInstance.Destroy();
	
}

void VulkanCanvas::OnTimer(wxTimerEvent& event)
{
	Render();
}

void VulkanCanvas::UpdateUniformBuffer(float width, float height)
{
	/*static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(time * 90.0);
	ubo.view = Invision::Matrix::CameraVK(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Invision::Matrix::PerspectiveVK(45.0, width / height, 0.1f, 10.0f);
	uniformBuffer.UpdateUniform(vulkInstance, &ubo, sizeof(ubo), 0); */
}

void VulkanCanvas::Render()
{
	//VkResult nextImageResult = commandBuffer.AquireNextImage(vulkInstance);
	/*VkResult nextImageResult = renderer.AquireNextImage(vulkInstance);
	if (nextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
		return;
	}
	else if (nextImageResult != VK_SUCCESS) {
		throw Invision::VulkanException("failed to acquire swap chain image!");
	}

	UpdateUniformBuffer(m_Size.GetWidth(), m_Size.GetHeight());

	//VkResult drawFrameResult = commandBuffer.DrawFrame(vulkInstance);
	VkResult drawFrameResult = renderer.DrawFrame(vulkInstance, commandBuffer);
	if (drawFrameResult == VK_ERROR_OUT_OF_DATE_KHR || drawFrameResult == VK_SUBOPTIMAL_KHR) {
		RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
	}
	else if (drawFrameResult != VK_SUCCESS) {
		throw Invision::VulkanException("failed to present swap chain image!");
	}*/
}

void VulkanCanvas::OnPaint(wxPaintEvent& event)
{
	//while (1)
	//{
	//	commandBuffer.DrawFrame(vulkInstance);
	//	Sleep(1);
	//}
}

void VulkanCanvas::RecreateSwapChain(const int width, const int height)
{
	/*// first Destroy
	//commandBuffer.DestroySemaphores(vulkInstance);
	renderer.DestroySemaphores(vulkInstance);
	commandPool.DestroyCommandPool(vulkInstance);
	uniformBuffer.DestroyUniformBuffer(vulkInstance);
	framebuffer.DestroyFramebuffer(vulkInstance);
	renderPass.DestroyRenderPass(vulkInstance);
	Invision::DestroyPresentationSystem(vulkInstance);

	//Recreate
	commandPool.CreateCommandPool(vulkInstance);
	Invision::CreatePresentationSystem(vulkInstance, width, height);
	renderPass.AddAttachment(vulkInstance);
	renderPass.AddSubpass();
	renderPass.CreateRenderPass(vulkInstance);
	framebuffer.CreateFramebuffer(vulkInstance, renderPass);
	uniformBuffer.CreateUniformBuffer(vulkInstance);
	
	//commandBuffer.CreateCommandBuffers(vulkInstance, commandPool, framebuffer, pipeline, renderPass);
	BuildCommandBuffer(width, height);
	//commandBuffer.CreateSyncObjects(vulkInstance);
	renderer.CreateSyncObjects(vulkInstance);*/
}

void VulkanCanvas::OnResize(wxSizeEvent& event)
{

	wxSize size = GetSize();
	if (size.GetWidth() == 0 || size.GetHeight() == 0) {
		return;
	}

	RecreateSwapChain(size.GetWidth(), size.GetHeight());
	wxRect refreshRect(size);

	RefreshRect(refreshRect, false);
}

void VulkanCanvas::OnPaintException(const std::string& msg)
{
	wxMessageBox(msg, "Vulkan Error");
	wxTheApp->ExitMainLoop();
}