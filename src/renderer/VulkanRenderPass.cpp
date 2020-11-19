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
		
		renderPass.AddAttachment(instance->GetCoreEngine()->GetVulkanInstance(), instance->GetVulkanContext(), instance->GetVulkanContext().swapChainImageFormat,VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		renderPass.AddSubpass({});

		renderPass.AddSubpassDependency(instance->GetCoreEngine()->GetVulkanInstance(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
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