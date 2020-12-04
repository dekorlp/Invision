//#include "Vertex.h"

#include "RenderWidget.h"



void RenderWidget::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsInstance->ResetPresentation({ HWND(winId()), width, height });

	// setup framebuffer
	framebuffer.reset();
	framebuffer = graphicsInstance->CreateFramebuffer(renderPass);

	// setup commandBuffers
	commandBuffer.reset();
	commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	BuildCommandBuffer(width, height);
}

void RenderWidget::BuildCommandBuffer(float width, float height)
{
	commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	commandBuffer->BeginCommandBuffer().
		SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height }).
		BeginRenderPass(renderPass, framebuffer).
		BindPipeline(pipeline).
		BindVertexBuffer({ vertexBuffer }, 0, 1).
		BindDescriptorSets(uniformBuffer, pipeline).
		BindIndexBuffer(indexBuffer).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).
		EndRenderPass().
		EndCommandBuffer();
}

void RenderWidget::UpdateUniformBuffer(float width, float height)
{
	UniformBufferObject ubo = {};
	//ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::TranslateVK(pos) *  Invision::Matrix::Scale(scale);
	ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(angle) *  Invision::Matrix::TranslateVK(pos) *  Invision::Matrix::Scale(scale);
	
	float x = radius * sin(theta) * cos(phi);
	float y = radius * sin(theta) * sin(phi);
	float z = radius * cos(theta);

	float dtheta = PI / 2.f;
	float upX = radius * sin(theta + dtheta) * cos(phi);
	float upY = radius * cos(theta + dtheta) * sin(phi);
	float upZ = radius * sin(theta + dtheta);

	ubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::CameraVK(Invision::Vector3(x, y, z), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(upX, upY, upZ)) ;
	ubo.proj = Invision::Matrix(1.0f) * Invision::Matrix::PerspectiveVK(45.0, width / height, 0.1f, 10.0f);
	uniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0);
}

