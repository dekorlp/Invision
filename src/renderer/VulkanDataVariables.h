/////////////////////////////////////////////////////////////////////////////
// Name:	  Vertex
// Purpose:	  
// Author:	  Dennis Koehler
// Created:	  2018-01-01
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_VERTEX
#define INCLUDED_VERTEX

#include <array>
#include <string>
#include "Vulkan/vulkan.h"
//#include <glm/glm.hpp>
#include "Vector2.h"
#include "Vector3.h"

class Vector3;
class Vector2;

//////////////////////////////////////////////////
/////////////// NEW VERTEX CODE //////////////////
struct Vertex
{
	Invision::Vector2 pos;
	Invision::Vector3 color;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

//////////////////////////////////////////////////

struct SVulkanLogicalDevice
{
	VkDevice m_logicalDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
};

struct SVulkanSwapchain
{
	VkSwapchainKHR m_swapchain;
	std::vector<VkImage> m_swapchainImages;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;
	std::vector<VkImageView> m_swapchainImageViews;
};

struct SVulkanPipeline
{
	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
};

struct SQueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef _DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

enum VertexSource { IN_VERTEX_SOURCE_STRUCT, IN_VERTEX_SOURCE_SHADER };

struct SMeshData
{
	std::vector<Vertex> meshVertizes;
	VkBuffer VertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};

struct ShaderType
{
	std::string shaderName;
	std::string shaderPath;
	VkShaderStageFlagBits shaderState;
	VkShaderModule shaderModule;
};


struct ShaderPipeline
{
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<ShaderType> shaderType;
	VertexSource vertexSource;
	uint32_t shaderCountOfVertizes = 0;
	bool draw = false;
	SMeshData meshData;
};

#endif // INCLUDED_VULKAN