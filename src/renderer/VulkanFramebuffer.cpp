#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"

namespace Invision
{

	VulkanFramebuffer::VulkanFramebuffer(VulkanInstance* instance, std::shared_ptr<Invision::IRenderPass> renderPass) :
		IFramebuffer(instance, renderPass)
	{
		vulkanInstance = instance;
		framebuffer.CreateFramebuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(), dynamic_pointer_cast<Invision::VulkanRenderPass>(renderPass)->GetRenderPass());
	}

	VulkanBaseFramebuffer VulkanFramebuffer::GetFramebuffer()
	{
		return framebuffer;
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		framebuffer.DestroyFramebuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

}