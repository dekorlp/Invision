#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"
#include "VulkanTexture.h"

#include "VulkanUniformBuffer.h"
namespace Invision
{

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanInstance* instance) :
		IUniformBuffer(instance)
	{
		vulkanInstance = instance;
	}

	VulkanUniformBuffer& VulkanUniformBuffer::CreateUniformBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStageFlag shaderStage, uint64_t bufferSize, uint64_t offset)
	{
		VkShaderStageFlags vkShaderStage = 0;
		

		if ((shaderStage & SHADER_STAGE_VERTEX_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ((shaderStage & SHADER_STAGE_GEOMETRY_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ((shaderStage & SHADER_STAGE_FRAGMENT_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if (shaderStage & SHADER_STAGE_COMPUTE_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_COMPUTE_BIT;
		}

		if (shaderStage & SHADER_STAGE_TESSELLATION_CONTROL_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if (shaderStage & SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if (shaderStage == 0)
		{
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");
		}

		uniformBuffer.CreateUniformBinding(set, binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount, vkShaderStage, bufferSize, offset);
		return *this;
	}

	VulkanUniformBuffer& VulkanUniformBuffer::CreateImageBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStageFlag shaderStage, std::shared_ptr < Invision::ITexture> texture)
	{
		VkShaderStageFlags vkShaderStage = 0;


		if ((shaderStage & SHADER_STAGE_VERTEX_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ((shaderStage & SHADER_STAGE_GEOMETRY_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ((shaderStage & SHADER_STAGE_FRAGMENT_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if (shaderStage & SHADER_STAGE_COMPUTE_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_COMPUTE_BIT;
		}

		if (shaderStage & SHADER_STAGE_TESSELLATION_CONTROL_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if (shaderStage & SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if (shaderStage == 0)
		{
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");
		}

		
		uniformBuffer.CreateImageBinding(set, binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount, vkShaderStage, dynamic_pointer_cast<VulkanTexture>(texture)->GetBaseTexture().GetImageView(), dynamic_pointer_cast<VulkanTexture>(texture)->GetBaseTexture().GetImageSampler());
		return *this;


	}

	void VulkanUniformBuffer::CreateUniformBuffer()
	{
		uniformBuffer.CreateUniformBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext());
	}

	void VulkanUniformBuffer::UpdateUniform(const void* source, size_t size, uint32_t set, uint32_t binding)
	{
		uniformBuffer.UpdateUniform(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), vulkanInstance->GetVulkanContext(), source, size, set, binding);
	}

	VulkanBaseUniformBuffer VulkanUniformBuffer::GetBuffer()
	{
		return uniformBuffer;
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		uniformBuffer.DestroyUniformBuffer(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}
}