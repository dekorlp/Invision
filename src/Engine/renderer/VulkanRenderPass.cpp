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
		mVulkanInstance = instance;

		
		
		//vulkanEngine = instance->GetCoreEngine();
	}

	void VulkanRenderPass::CreateMainRenderPass()
	{
		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;
		
		if (mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().UseMSAA == false) // MSAA is inactive
		{

			basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetVulkanContext().swapChainImageFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				, { mSubPassIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

			if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
			{
				basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
					mVulkanInstance->GetVulkanContext(),
					mVulkanInstance->GetDepthRessources().FindDepthFormat(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct()),
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
					{ mSubPassIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			}

		}
		else // MSAA is active
		{
			basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetVulkanContext().swapChainImageFormat,
				mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
				, { mSubPassIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

			if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
			{
				basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
					mVulkanInstance->GetVulkanContext(),
					mVulkanInstance->GetDepthRessources().FindDepthFormat(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct()),
					mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
					{ mSubPassIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			}

			basePass.AddAttachment(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(),
				mVulkanInstance->GetVulkanContext(),
				mVulkanInstance->GetVulkanContext().swapChainImageFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				, { mSubPassIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, true);
		}

		mRenderPass.AddSubpass(basePass);

		if (mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated() == true)
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		}
		else
		{
			mRenderPass.AddSubpassDependency(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}




		mRenderPass.CreateRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}

	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		mRenderPass.DestroyRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}
}