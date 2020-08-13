#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanRenderPass.h"
namespace Invision
{

	/*VulkanRenderPass::VulkanRenderPass(VulkanEngine* engine) :
		IRenderPass(engine)
	{
		renderPass.AddAttachment(engine->GetVulkanInstance(), engine->GetVulkanContext());
		renderPass.AddSubpass();
		renderPass.CreateRenderPass(engine->GetVulkanInstance());
		vulkanEngine = engine;
	}*/
	
	VulkanRenderPass::VulkanRenderPass(VulkanInstance* instance)
		:IRenderPass(instance)
	{
		
		renderPass.AddAttachment(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext());
		renderPass.AddSubpass();
		renderPass.CreateRenderPass(instance->GetCoreEngine()->GetVulkanInstance());
		vulkanInstance = instance;
		//vulkanEngine = instance->GetCoreEngine();
	}

	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return renderPass;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		renderPass.DestroyRenderPass(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}