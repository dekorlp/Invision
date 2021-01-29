//#include "Vertex.h"

#include "RenderWidget.h"



void RenderWidget::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsInstance->ResetPresentation({ HWND(winId()), width, height }, renderPass, framebuffer, commandBuffer);

	// setup framebuffer
	//framebuffer.reset();
	//framebuffer = graphicsInstance->CreateFramebuffer(renderPass, graphicsInstance->GetSizeSwapchainImages());

	// setup commandBuffers
	//commandBuffer.reset();
	//commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	BuildCommandBuffer(width, height);
}

void RenderWidget::BuildCommandBuffer(float width, float height)
{
	//commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	commandBuffer->BeginCommandBuffer().
		SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height }).
		BeginRenderPass(renderPass, framebuffer).
		BindPipeline(pipeline).
		BindVertexBuffer({ vertexBuffer }, 0, 1).
		BindDescriptorSets(uniformBuffer, pipeline).
		BindIndexBuffer(indexBuffer, Invision::INDEX_TYPE_UINT32).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).
		EndRenderPass().
		EndCommandBuffer();
}

void RenderWidget::UpdateUniformBuffer(float width, float height)
{
	UniformBufferObject ubo = {};
	
	
	
	float x = radius * sin(theta) * cos(phi);
	float y = radius * sin(theta) * sin(phi);
	float z = radius * cos(theta);

	float dtheta = PI / 2.f;
	float upX = radius * sin(theta + dtheta) * cos(phi);
	float upY = radius * cos(theta + dtheta) * sin(phi);
	float upZ = radius * sin(theta + dtheta);

	if (switchFixedCamera == false)
	{
		ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
		//ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(angle) *  Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
		ubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(x, y, z), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(upX, upY, upZ));
	}
	else
	{
		//Invision::Matrix::Translate(Invision::Vector3(0.0f, 0.0f, 0.0f)) * 
		ubo.model = Invision::Matrix::RotateZ(angle + dt * 90.0);
		ubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	}
	ubo.proj = Invision::Matrix(1.0f) * Invision::Matrix::Perspective(45.0, width / height, 0.1f, 10.0f); // perspective projection
	uniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0, 0);
}

