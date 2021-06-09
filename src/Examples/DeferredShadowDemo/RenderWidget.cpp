//#include "Vertex.h"

#include "RenderWidget.h"



void RenderWidget::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsInstance->ResetPresentation({ HWND(winId()), width, height }, renderPass, framebuffer, commandBuffer);

	// Reset GBuffer (Framebuffer and Commandbuffer)
	mGBuffer.gCommandbuffer.reset();
	mGBuffer.gCommandbuffer = graphicsInstance->CreateCommandBuffer(mGBuffer.gFramebuffer);

	// Reset SBuffer (Shadowbuffer)
	mSBuffer.sCommandbuffer.reset();
	mSBuffer.sCommandbuffer = graphicsInstance->CreateCommandBuffer(mSBuffer.sFramebuffer);

	BuildCommandBuffer(width, height);
}

void RenderWidget::BuildCommandBuffer(float width, float height)
{
	



	// main command Buffer
	commandBuffer->BeginCommandBuffer().

		SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height }).
		BeginRenderPass(renderPass, framebuffer, 0, 0, (uint32_t)width, (uint32_t)height).
		BindDescriptorSets(DeferredUniformBuffer, pipeline).
		BindPipeline(pipeline).
		Draw(3, 1, 0, 0).
		EndRenderPass().
		EndCommandBuffer();

	// shadow command Buffer
	mSBuffer.sCommandbuffer->BeginCommandBuffer().

		BeginRenderPass(mSBuffer.sRenderPass, mSBuffer.sFramebuffer, 0, 0, FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE).
		SetViewport({ 0, 0, (float)FRAMEBUFFER_SIZE, (float)FRAMEBUFFER_SIZE, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)FRAMEBUFFER_SIZE, (uint32_t)FRAMEBUFFER_SIZE }).

		// Draw Mesh
		BindPipeline(mSBuffer.sPipeline).
		BindVertexBuffer({ vertexBuffer }, 0, 1).
		BindDescriptorSets(mSBuffer.sUniformBuffer, mSBuffer.sPipeline).
		BindIndexBuffer(indexBuffer, Invision::INDEX_TYPE_UINT32).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).

		// Draw Plane
		BindPipeline(mSBuffer.sPipeline).
		BindVertexBuffer({ PlaneVertexBuffer }, 0, 1).
		BindDescriptorSets(planeUniformBuffer, mSBuffer.sPipeline).
		BindIndexBuffer(PlaneIndexBuffer, Invision::INDEX_TYPE_UINT32).
		DrawIndexed(static_cast<uint32_t>(planeIndices.size()), 1, 0, 0, 0).

		EndRenderPass().
		EndCommandBuffer();

	// gBuffer command Buffer
	mGBuffer.gCommandbuffer->BeginCommandBuffer().

		BeginRenderPass(mGBuffer.gRenderPass, mGBuffer.gFramebuffer, 0, 0, FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE).
		SetViewport({ 0, 0, (float)FRAMEBUFFER_SIZE, (float)FRAMEBUFFER_SIZE, 0.0, 1.0 }).
		SetScissor({ 0, 0, (uint32_t)FRAMEBUFFER_SIZE, (uint32_t)FRAMEBUFFER_SIZE }).
		
		// Draw Mesh
		BindPipeline(mGBuffer.gPipeline).
		BindVertexBuffer({ vertexBuffer }, 0, 1).
		BindDescriptorSets(uniformBuffer, mGBuffer.gPipeline).
		BindIndexBuffer(indexBuffer, Invision::INDEX_TYPE_UINT32).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).

		// Draw Plane
		BindPipeline(planePipeline).
		BindVertexBuffer({ PlaneVertexBuffer }, 0, 1).
		BindDescriptorSets(planeUniformBuffer, planePipeline).
		BindIndexBuffer(PlaneIndexBuffer, Invision::INDEX_TYPE_UINT32).
		//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		DrawIndexed(static_cast<uint32_t>(planeIndices.size()), 1, 0, 0, 0).

		EndRenderPass().
		EndCommandBuffer();
}

void RenderWidget::UpdateUniformBuffer(float width, float height)
{
	UniformBufferObject ubo = {};
	UniformBufferObject plubo = {};
	
	
	
	float x = radius * sin(theta) * cos(phi);
	float y = radius * sin(theta) * sin(phi);
	float z = radius * cos(theta);

	float dtheta = PI / 2.f;
	float upX = radius * sin(theta + dtheta) * cos(phi);
	float upY = radius * cos(theta + dtheta) * sin(phi);
	float upZ = radius * sin(theta + dtheta);

	Invision::Vector3 posPlane = pos;
	posPlane.SetY(posPlane.getY() - 0.3f);

	if (switchFixedCamera == false)
	{
		ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
		//ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(angle) *  Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
		ubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(x, y, z), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(upX, upY, upZ));

		plubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateX(90) * Invision::Matrix::Translate(posPlane) *  Invision::Matrix::Scale(scale);
		//ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(angle) *  Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
		plubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(x, y, z), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(upX, upY, upZ));
	}
	else
	{
		//Invision::Matrix::Translate(Invision::Vector3(0.0f, 0.0f, 0.0f)) * 
		ubo.model = Invision::Matrix::RotateZ(angle + dt * 90.0);
		ubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));

		plubo.model = Invision::Matrix::RotateZ(angle + dt * 90.0) * Invision::Matrix::RotateX(90) * Invision::Matrix::Translate(posPlane) *  Invision::Matrix::Scale(scale);
		plubo.view = Invision::Matrix(1.0f) *  Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	}
	ubo.proj = Invision::Matrix(1.0f) * Invision::Matrix::Perspective(45.0, width / height, 0.1f, 10.0f); // perspective projection
	plubo.proj = Invision::Matrix(1.0f) * Invision::Matrix::Perspective(45.0, width / height, 0.1f, 10.0f); // perspective projection
	uniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0, 0);
	planeUniformBuffer->UpdateUniform(&plubo, sizeof(plubo), 0, 0);


	// depth ubo creation
	UniformBufferObject depthUniformBuffer;
	depthUniformBuffer.proj = Invision::Matrix::Perspective(45.0, 1.0f, 1.0f, 96.0f);
	depthUniformBuffer.view = Invision::Matrix::Camera(Invision::Vector3(1.2f, 1.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 1.0, 0.0));
	depthUniformBuffer.model = Invision::Matrix(1.0f)  * Invision::Matrix::Translate(pos) *  Invision::Matrix::Scale(scale);
	mSBuffer.sUniformBuffer->UpdateUniform(&depthUniformBuffer, sizeof(depthUniformBuffer), 0, 0);


	// set light
	UniformLightBuffer light;
	light.lightColor = { 1.0, 1.0, 1.0 };
	light.lightPos = { 1.2f, 1.0f, 2.0f };
	light.viewPos = { 0.0f, 0.0f, 0.0f };
	light.lightSpaceMatrix = depthUniformBuffer.proj * depthUniformBuffer.view * depthUniformBuffer.model;
	DeferredUniformBuffer->UpdateUniform(&light, sizeof(light), 0, 5);
}

