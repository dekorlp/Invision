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
	Bind(wxEVT_SIZE, &VulkanCanvas::OnResize, this);
	WXHWND hwnd = this->GetHandle(); // get window handle


	std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

	//vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
	//vulkanInstance.SetDebugMessanger(debugCallback);

	Invision::CanvasDimensions dim = { hwnd, size.GetWidth(), size.GetHeight() };
	//graphicsEngine = std::make_shared<Invision::VulkanEngine>(dim);
	graphicsEngine = Invision::create_engine(Invision::EngineType::Vulkan);
	graphicsEngine->Init(dim);
	renderPass = graphicsEngine->CreateRenderPass();
	vertexBuffer = graphicsEngine->CreateVertexBuffer();
	uniformBuffer = graphicsEngine->CreateUniformBuffer();
	indexBuffer = graphicsEngine->CreateIndexBuffer();
	pipeline = graphicsEngine->CreatePipeline();
	

	vertexBuffer->CreateVertexBuffer(sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);
	vertexBuffer->CreateVertexInput(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
		.CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
		.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

	indexBuffer->CreateIndexBuffer(sizeof(indices[0]) * indices.size(), indices.data(), 0);

	uniformBuffer->CreateUniformBinding(0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 0).CreateUniformBuffer();

	auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/examples/triangle/Shader/DrawUniformBuffer/vert.spv"));
	auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/examples/triangle/Shader/DrawUniformBuffer/frag.spv"));
	pipeline->AddUniformBuffer(uniformBuffer);
	pipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
	pipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
	pipeline->AddVertexBuffer(vertexBuffer);
	pipeline->CreatePipeline(renderPass);
	framebuffer = graphicsEngine->CreateFramebuffer(renderPass);

	BuildCommandBuffer(size.GetWidth(), size.GetHeight());
	renderer = graphicsEngine->CreateRenderer();

	this->Bind(wxEVT_IDLE, &VulkanCanvas::OnIdle, this);
}

void VulkanCanvas::BuildCommandBuffer(float width, float height)
{
	commandBuffer = graphicsEngine->CreateCommandBuffer(framebuffer);
	commandBuffer->BeginCommandBuffer().
		SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		SetScissor({0, 0, (uint32_t)width, (uint32_t)height}).
		BeginRenderPass(renderPass, framebuffer).
		BindPipeline(pipeline).
		BindVertexBuffer({vertexBuffer}, 0, 1).
		BindDescriptorSets(uniformBuffer, pipeline).
		BindIndexBuffer(indexBuffer).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).
		EndRenderPass().
		EndCommandBuffer();
} 

VulkanCanvas::~VulkanCanvas() noexcept
{
	//vulkanInstance.Destroy();
	

	
}


void VulkanCanvas::OnIdle(wxIdleEvent& event)
{
	Render();
	event.RequestMore();	
}

void VulkanCanvas::UpdateUniformBuffer(float width, float height)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(time * 90.0);
	ubo.view = Invision::Matrix::CameraVK(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Invision::Matrix::PerspectiveVK(45.0, width / height, 0.1f, 10.0f);
	uniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0); 
}

void VulkanCanvas::Render()
{
	

	bool recreateSwapchainIsNecessary = false;

	recreateSwapchainIsNecessary = renderer->PrepareFrame();

	UpdateUniformBuffer(m_Size.GetWidth(), m_Size.GetHeight());

	recreateSwapchainIsNecessary = renderer->SubmitFrame(commandBuffer);


	if (recreateSwapchainIsNecessary) RecreateSwapChain(m_Size.GetWidth(), m_Size.GetHeight());
}

void VulkanCanvas::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsEngine->ResetPresentation({ this->GetHandle(), width, height });

	// setup framebuffer
	framebuffer.reset();
	framebuffer = graphicsEngine->CreateFramebuffer(renderPass);

	// setup commandBuffers
	commandBuffer.reset();
	commandBuffer = graphicsEngine->CreateCommandBuffer(framebuffer);
	BuildCommandBuffer(width, height);
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