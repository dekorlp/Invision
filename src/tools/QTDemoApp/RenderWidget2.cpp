#include "RenderWidget2.h"

void RenderWidget2::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsInstance->ResetPresentation({ HWND(winId()), width, height });

	// setup framebuffer
	framebuffer.reset();
	framebuffer = graphicsInstance->CreateFramebuffer(renderPass, graphicsInstance->GetSizeSwapchainImages());

	// setup commandBuffers
	commandBuffer.reset();
	commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	BuildCommandBuffer(width, height);
}

void RenderWidget2::BuildCommandBuffer(float width, float height)
{
	commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	commandBuffer->BeginCommandBuffer().
		SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height }).
		BeginRenderPass(renderPass, framebuffer).
		BindPipeline(pipeline).
		BindVertexBuffer({ vertexBuffer }, 0, 1).
		BindDescriptorSets(uniformBuffer, pipeline).
		BindIndexBuffer(indexBuffer, Invision::INDEX_TYPE_UINT32).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices2.size()), 1, 0, 0, 0).
		EndRenderPass().
		EndCommandBuffer();
}

void RenderWidget2::UpdateUniformBuffer(float width, float height)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject2 ubo = {};
	ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(time * 90.0);
	ubo.view = Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Invision::Matrix::Perspective(45.0, width / height, 0.1f, 10.0f);
	uniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0, 0);
}