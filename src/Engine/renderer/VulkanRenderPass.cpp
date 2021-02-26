#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"
#include "VulkanTexture.h"

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

	void VulkanRenderPass::CreateMainRenderPass(VulkanBaseTexture& depthTexture, VulkanBaseTexture& colorTexture)
	{
		// Create RenderPass with MSAA

		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;
		
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
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
		mAttachmentRefTextures.push_back(&colorTexture);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
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
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&depthTexture);
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
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, true);
		

		mRenderPass.AddSubpass(basePass);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
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

	void VulkanRenderPass::CreateMainRenderPass(VulkanBaseTexture& depthTexture)
	{
		// Create RenderPass without MSAA

		std::vector<Invision::VulkanBaseTexture> baseTextures;

		VulkanBaseSubPass basePass;
		basePass.mIsMainSubPass = true;

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
			, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
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
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&depthTexture);
		}

		mRenderPass.AddSubpass(basePass);

		if (mVulkanInstance->GetVulkanContext().mUseDepthRessources == true)
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

	void VulkanRenderPass::AddAttachment(AttachmentType attachmentType, GfxFormat format)
	{
		VulkanBaseSubPass basePass;
		if (attachmentType == ATTACHMENT_TYPE_COLOR)
		{
			VulkanBaseTexture texture;
			//texture.CreateColorRessources(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool(), mVulkanInstance->GetCoreEngine()->GetMemoryManager(), mVulkanInstance->GetVulkanContext(), mVulkanInstance->GetVulkanContext().swapChainImageFormat);
			mAttachmentTextures.push_back(texture);

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
				, { mAttachmentIndex++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&mAttachmentTextures[mAttachmentTextures.size() - 1]);
		}
		else // ATTACHMENT_TYPE_DEPTH
		{
			VulkanBaseTexture texture;
			//texture.CreateDepthRessources(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetCoreEngine()->GetCommandPool(), mVulkanInstance->GetCoreEngine()->GetMemoryManager(), mVulkanInstance->GetVulkanContext());
			mAttachmentTextures.push_back(texture);

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
				{ mAttachmentIndex++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
			mAttachmentRefTextures.push_back(&mAttachmentTextures[mAttachmentTextures.size() - 1]);
		}

		mRenderPass.AddSubpass(basePass);
		
	}

	VulkanBaseRenderPass VulkanRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	std::vector<Invision::VulkanBaseTexture*> VulkanRenderPass::GetAttachmentTextures()
	{
		return mAttachmentRefTextures;
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		mRenderPass.DestroyRenderPass(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}
}