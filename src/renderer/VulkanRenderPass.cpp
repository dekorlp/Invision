#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanRenderPass.h"
namespace Invision
{

	VulkanRenderPass::VulkanRenderPass(VulkanEngine* engine) :
		IRenderPass(engine)
	{
		renderPass.AddAttachment(engine->GetVulkanInstance());
		renderPass.AddSubpass();
		renderPass.CreateRenderPass(engine->GetVulkanInstance());
		vulkanEngine = engine;
	}
	


	VulkanRenderPass::~VulkanRenderPass()
	{
		renderPass.DestroyRenderPass(vulkanEngine->GetVulkanInstance());
	}
}