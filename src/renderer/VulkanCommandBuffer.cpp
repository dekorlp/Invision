#include "precompiled.h"

#include "VulkanEngine.h"

#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
namespace Invision
{

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanEngine* engine, std::shared_ptr<Invision::IFramebuffer> framebuffer) :
		ICommandBuffer(engine, framebuffer)
	{
		vulkanEngine = engine;
		
		commandBuffer.CreateCommandBuffer(engine->GetVulkanInstance(), engine->GetCommandPool(), (unsigned int)dynamic_pointer_cast<Invision::VulkanFramebuffer>(framebuffer)->GetFramebuffer().GetFramebuffers().size());
	}

}