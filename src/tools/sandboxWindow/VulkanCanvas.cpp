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

	mCache = Invision::CreatePipelineCache(vulkInstance);

	auto vertShaderCode = readFile(std::string(ROOT).append("/src/tools/sandboxWindow/Shader/DrawIndexBuffer/vert.spv"));
	auto fragShaderCode	= readFile(std::string(ROOT).append("/src/tools/sandboxWindow/Shader/DrawIndexBuffer/frag.spv"));

	Invision::VulkanShader vertShader(vulkInstance, vertShaderCode, VK_SHADER_STAGE_VERTEX_BIT);
	Invision::VulkanShader fragShader(vulkInstance, fragShaderCode, VK_SHADER_STAGE_FRAGMENT_BIT);


	commandPool.CreateCommandPool(vulkInstance);
	vertexBuffer.CreateVertexBuffer(vulkInstance, commandPool, sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);
	vertexBuffer.CreateVertexInputDescription(0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX)
		.CreateAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
		.CreateAttributeDescription(1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));
	indexBuffer.CreateIndexBuffer(vulkInstance, commandPool, sizeof(indices[0]) * indices.size(), indices.data(), 0);

	descriptorPool.CreateDescriptorPool(vulkInstance);
	uniformBuffer.CreateUniformBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject)).CreateUniformBuffer(vulkInstance);
	pipeline.AddUniformBuffer(uniformBuffer);

	pipeline.AddShader(vertShader);
	pipeline.AddShader(fragShader);
	pipeline.AddVertexBuffer(vertexBuffer);
	pipeline.CreatePipeline(vulkInstance, renderPass, 0, mCache);
	vertShader.Destroy(vulkInstance);
	fragShader.Destroy(vulkInstance);

	framebuffer.CreateFramebuffer(vulkInstance, renderPass);
	
	

	//commandBuffer.CreateCommandBuffers(vulkInstance, commandPool, framebuffer, pipeline, renderPass);
	BuildCommandBuffer(size.GetWidth(), size.GetHeight());
	//commandBuffer.CreateSyncObjects(vulkInstance);
	renderer.CreateSyncObjects(vulkInstance);

	m_timer.SetOwner(this);
	m_timer.Start(5);
	this->Bind(wxEVT_TIMER, &VulkanCanvas::OnTimer, this);
}

void VulkanCanvas::BuildCommandBuffer(float width, float height)
{
	VkViewport viewport = {};
	viewport.width = (float)width; // default: (float)vulkanInstance.swapchainExtend.Width
	viewport.height = (float)height;// default: (float)vulkanInstance.swapchainExtend.Height
	viewport.minDepth = 0.0; // default: 0.0
	viewport.maxDepth = 1.0; // default: 1.0


	VkRect2D scissor = {};
	scissor.offset = { 0, 0 }; // default: { 0, 0 };
	scissor.extent.width = width;
	scissor.extent.height = height;
	commandBuffer.CreateCommandBuffer(vulkInstance, commandPool, framebuffer.GetFramebuffers().size()).
		BeginCommandBuffer().
		SetViewport(viewport).
		SetScissor(scissor).
		BeginRenderPass(vulkInstance, renderPass, framebuffer).
		BindPipeline(pipeline, VK_PIPELINE_BIND_POINT_GRAPHICS).
		BindVertexBuffer({vertexBuffer}, 0, 1).
		BindIndexBuffer(indexBuffer, VK_INDEX_TYPE_UINT16).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).
		EndRenderPass().
		EndCommandBuffer();


	/*commandBuffer.CreateCommandBuffer(vulkInstance, commandPool, framebuffer.GetFramebuffers().size());
	commandBuffer.BeginCommandBuffer();
	// there is no viewport used
	VkViewport viewport = {};
	viewport.x = 0.0f; // default: 0.0f;
	viewport.y = 0.0f; // default: 0.0f;

	viewport.width = (float)width; // default: (float)vulkanInstance.swapchainExtend.Width
	viewport.height = (float)height;// default: (float)vulkanInstance.swapchainExtend.Height
	viewport.minDepth = 0.0; // default: 0.0
	viewport.maxDepth = 1.0; // default: 1.0
	commandBuffer.SetViewport(viewport);

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 }; // default: { 0, 0 };
	scissor.extent.width = width;
	scissor.extent.height = height;
	commandBuffer.SetScissor(scissor);

	commandBuffer.BeginRenderPass(vulkInstance, renderPass, framebuffer);
	commandBuffer.BindPipeline(pipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
	commandBuffer.Draw(3, 1, 0, 0);
	commandBuffer.EndRenderPass();
	commandBuffer.EndCommandBuffer();*/
}

VulkanCanvas::~VulkanCanvas() noexcept
{
	// wait for idle status before destroying
	//vkDeviceWaitIdle(vulkInstance.logicalDevice);

	//commandBuffer.DestroySemaphores(vulkInstance);
	renderer.DestroySemaphores(vulkInstance);

	framebuffer.DestroyFramebuffer(vulkInstance);
	pipeline.DestroyPipeline(vulkInstance);
	uniformBuffer.DestroyUniformBuffer(vulkInstance);
	descriptorPool.DestroyDescriptorPool(vulkInstance);
	vertexBuffer.DestroyVertexBuffer(vulkInstance);
	indexBuffer.DestroyIndexBuffer(vulkInstance);
	commandPool.DestroyCommandPool(vulkInstance);
	Invision::DestroyPipelineCache(vulkInstance, mCache);
	renderPass.DestroyRenderPass(vulkInstance);
	Invision::DestroyPresentationSystem(vulkInstance);
	Invision::DestroyVulkanDevice(vulkInstance);
	Invision::DestroySurface(vulkInstance);
	vulkanInstance.Destroy();
}

void VulkanCanvas::OnTimer(wxTimerEvent& event)
{
	Render();
}

void VulkanCanvas::UpdateUniformBuffer(float width, float height)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = Invision::Matrix::RotateZ(time * 90.0);
	ubo.view = Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Invision::Matrix::Perspective(45.0, width / height, 0.1f, 10.0f);

	uniformBuffer.UpdateUniform(vulkInstance, &ubo, sizeof(ubo), 0);
		
}

void VulkanCanvas::Render()
{
	//VkResult nextImageResult = commandBuffer.AquireNextImage(vulkInstance);
	VkResult nextImageResult = renderer.AquireNextImage(vulkInstance);
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
	}
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
	// first Destroy
	//commandBuffer.DestroySemaphores(vulkInstance);
	renderer.DestroySemaphores(vulkInstance);
	commandPool.DestroyCommandPool(vulkInstance);
	uniformBuffer.DestroyUniformBuffer(vulkInstance);
	descriptorPool.DestroyDescriptorPool(vulkInstance);
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
	descriptorPool.CreateDescriptorPool(vulkInstance);
	uniformBuffer.CreateUniformBuffer(vulkInstance);
	
	//commandBuffer.CreateCommandBuffers(vulkInstance, commandPool, framebuffer, pipeline, renderPass);
	BuildCommandBuffer(width, height);
	//commandBuffer.CreateSyncObjects(vulkInstance);
	renderer.CreateSyncObjects(vulkInstance);
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