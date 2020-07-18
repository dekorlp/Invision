#ifndef IGRAPHICS_ENGINE_H
#define IGRAPHICS_ENGINE_H

#include <string>
#include "IRenderer.h"
#include "IRenderPass.h"
#include "IVertexBuffer.h"
#include "IUniformBuffer.h"
#include "IIndexBuffer.h"
#include "IPipeline.h"
#include "IFramebuffer.h"
#include "ICommandBuffer.h"

namespace Invision
{
	enum PhysicalDeviceType
	{
		PHYSICAL_DEVICE_TYPE_OTHER = 0,
		PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,
		PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,
		PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
		PHYSICAL_DEVICE_TYPE_CPU = 4,
	};

	enum ShaderStage
	{
		SHADER_STAGE_VERTEX_BIT = 0,
		SHADER_STAGE_GEOMETRY_BIT = 2,
		SHADER_STAGE_FRAGMENT_BIT = 3,
		SHADER_STAGE_COMPUTE_BIT = 4
	};
	namespace GPU
	{
		enum GPUDeviceVendor
		{
			AMD = 0x1002,
			IMGTEC = 0x1010,
			NVIDIA = 0x10DE,
			ARM = 0x13B5,
			QUALCOMM = 0x5143,
			INTEL = 0x8086,
			UNKNOWN = 0x9999
		};
	}

	enum VertexFormat
	{
		FORMAT_UNEDFINED = 0,
		FORMAT_R32G32_SFLOAT = 1,
		FORMAT_R32G32B32_SFLOAT = 2,
	};

	enum VertexInputRate
	{
		VERTEX_INPUT_RATE_VERTEX = 0,
		VERTEX_INPUT_RATE_INSTANCE = 1
	};

	struct PhysicalDeviceProperties
	{
		uint32_t index;
		uint32_t apiVersion;
		uint32_t driverVersion;
		uint32_t vendorID;
		GPU::GPUDeviceVendor vendorType;
		uint32_t deviceID;
		PhysicalDeviceType deviceType;
		char deviceName[256];
	};

	struct CanvasDimensions
	{
		HWND hwnd;
		int width;
		int height;

	};

	namespace EngineType
	{
		enum Type
		{
			Unknown,
			DirectX,
			Vulkan
		};
	}

	class IGraphicsEngine
	{
	public:
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version);

		 INVISION_API virtual void Init(CanvasDimensions canvas) = 0;
		 INVISION_API virtual void Init(unsigned int index, CanvasDimensions canvas) = 0;


		 INVISION_API EngineType::Type type() const;

		 INVISION_API std::string const& name() const;

		 INVISION_API std::string const& version() const;

		 INVISION_API virtual std::shared_ptr<IRenderer> create_renderer() = 0;
		 INVISION_API virtual std::shared_ptr<IRenderPass> CreateRenderPass() = 0;
		 INVISION_API virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
		 INVISION_API virtual std::shared_ptr<IUniformBuffer> CreateUniformBuffer() = 0;
		 INVISION_API virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
		 INVISION_API virtual std::shared_ptr<IPipeline> CreatePipeline() = 0;
		 INVISION_API virtual std::shared_ptr<IFramebuffer> CreateFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass) = 0;
		 INVISION_API virtual std::shared_ptr<ICommandBuffer> CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer) = 0;

		 INVISION_API virtual std::vector< PhysicalDeviceProperties> GetPhysicalDevices() = 0;


		 INVISION_API ~IGraphicsEngine();
	protected:
		EngineType::Type Type_ = EngineType::Unknown;
		PhysicalDeviceProperties deviceProperties;
		std::string const Name_;
		std::string const Version_;
	};

}

#endif // IGRAPHICS_ENGINE_H