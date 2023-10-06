#include "RenderWidget.h"

void RenderWidget::RecreateSwapChain(const int width, const int height)
{
	// setup swapchain
	graphicsInstance->ResetPresentation({ HWND(winId()), width, height }, renderPass, framebuffer, commandBuffer);

	// setup framebuffer
	//framebuffer.reset();
	//framebuffer = graphicsInstance->CreateFramebuffer(renderPass, graphicsInstance->GetSizeSwapchainImages());

	// setup commandBuffers
	commandBuffer.reset();
	commandBuffer = graphicsInstance->CreateCommandBuffer(framebuffer);
	BuildCommandBuffer(width, height);
}