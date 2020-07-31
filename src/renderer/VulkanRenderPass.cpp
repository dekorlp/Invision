#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanRenderPass.h"
namespace Invision
{

	VulkanRenderPass::VulkanRenderPass(VulkanEngine* engine) :
		IRenderPass(engine)
	{
		renderPass.AddAttachment(engine->GetVulkanInstance(), engine->GetVulkanContext());
		renderPass.AddSubpass();
		renderPass.CreateRenderPass(engine->GetVulkanInstance());
		vulkanEngine = engine;
	}
	
	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return renderPass;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		renderPass.DestroyRenderPass(vulkanEngine->GetVulkanInstance());
	}
}