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
		vulkanInstance = instance;

		
		
		//vulkanEngine = instance->GetCoreEngine();
	}

	void VulkanRenderPass::CreateMainRenderPass()
	{
		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;

		basePass.AddAttachment(vulkanInstance->GetCoreEngine()->GetVulkanInstance(),
			vulkanInstance->GetVulkanContext(),
			vulkanInstance->GetVulkanContext().swapChainImageFormat,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			, { subPassIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

		if (vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			basePass.AddAttachment(vulkanInstance->GetCoreEngine()->GetVulkanInstance(),
				vulkanInstance->GetVulkanContext(),
				vulkanInstance->GetDepthRessources().findDepthFormat(vulkanInstance->GetCoreEngine()->GetVulkanInstance()),
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				{ subPassIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
		}

		renderPass.AddSubpass(basePass);

		if (vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			renderPass.AddSubpassDependency(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		}
		else
		{
			renderPass.AddSubpassDependency(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}




		renderPass.CreateRenderPass(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
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