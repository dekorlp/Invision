#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"

namespace Invision
{

	VulkanFramebuffer::VulkanFramebuffer(VulkanEngine* engine, std::shared_ptr<Invision::IRenderPass> renderPass) :
		IFramebuffer(engine, renderPass)
	{
		vulkanEngine = engine;
		framebuffer.CreateFramebuffer(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetVulkanContext(), dynamic_pointer_cast<Invision::VulkanRenderPass>(renderPass)->GetRenderPass());
	}

	VulkanBaseFramebuffer VulkanFramebuffer::GetFramebuffer()
	{
		return framebuffer;
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		framebuffer.DestroyFramebuffer(vulkanEngine->GetVulkanInstance());
	}

}