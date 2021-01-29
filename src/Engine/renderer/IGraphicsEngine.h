#ifndef IGRAPHICS_ENGINE_H
#define IGRAPHICS_ENGINE_H

#include <string>
#include "IGraphicsInstance.h"
//#include "IRenderer.h"
//#include "IRenderPass.h"
//#include "IVertexBuffer.h"
//#include "IUniformBuffer.h"
//#include "IIndexBuffer.h"
//#include "IPipeline.h"
//#include "IFramebuffer.h"
//#include "ICommandBuffer.h"

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
		SHADER_STAGE_VERTEX_BIT = 0x00000001,
		SHADER_STAGE_GEOMETRY_BIT = 0x00000002,
		SHADER_STAGE_FRAGMENT_BIT = 0x00000004,
		SHADER_STAGE_COMPUTE_BIT = 0x00000008,
		SHADER_STAGE_TESSELLATION_CONTROL_BIT = 0x0000010,
		SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 0x00000020
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

	enum IndexType
	{
		INDEX_TYPE_UINT16 = 0,
		INDEX_TYPE_UINT32 = 1,
	};

	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
		PRIMITIVE_TOPOLOGY_LINE_LIST = 1,
		PRIMITIVE_TOPOLOGY_LINE_STRIP = 2,
		PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 3,
		PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 4,
		PRIMITIVE_TOPOLOGY_TRIANGLE_FAN = 5,
		PRIMITIVE_TOPOLOGY_LINE_LIST_ADJ = 6,
		PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJ = 7,
		PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJ = 8,
		PRIMTIVIE_TOPOLOGY_TRIANGLE_STRIP_ADJ = 9
	};

	enum PolygonMode
	{
		POLYGON_MODE_FILL = 0,
		POLYGON_MODE_LINE = 1,
		POLYGON_MODE_POINT = 2
	};

	enum CullMode
	{
		CULL_MODE_NONE = 0x00000000,
		CULL_MODE_FRONT_BIT = 0x00000001,
		CULL_MODE_BACK_BIT = 0x00000002,
		CULL_MODE_FRONT_AND_BACK = 0x00000004
	};

	enum FrontFaceMode
	{
		FRONT_FACE_COUNTER_CLOCKWISE = 0,
		FRONT_FACE_CLOCKWISE = 1
	};

	enum MSAAMode
	{
		MSAAMODE_OFF = 0x00000000,
		MSAAMODE_SAMPLE_COUNT_1 = 0x00000001,
		MSAAMODE_SAMPLE_COUNT_2 = 0x00000002,
		MSAAMODE_SAMPLE_COUNT_4 = 0x00000004,
		MSAAMODE_SAMPLE_COUNT_8 = 0x00000008,
		MSAAMODE_SAMPLE_COUNT_16 = 0x00000010,
		MSAAMODE_SAMPLE_COUNT_32 = 0x00000020,
		MSAAMODE_SAMPLE_COUNT_64 = 0x00000040,
		MSAAMODE_SAMPLE_COUNT_BEST = 0x00000009F
	};

	typedef uint32_t CullModeBits;

	typedef struct PipelineProperties
	{
		PrimitiveTopology mPrimitiveTopology;
		PolygonMode mPolygonMode;
		CullModeBits mCullMode;
		FrontFaceMode mFrontFaceMode;
		float mLineWidth;

		PipelineProperties()
		{
			// Default Properties
			mPrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			mPolygonMode = POLYGON_MODE_FILL;
			mCullMode = CULL_MODE_BACK_BIT;
			mFrontFaceMode = FRONT_FACE_COUNTER_CLOCKWISE;
			mLineWidth = 1.0f;
		}

		PipelineProperties(PrimitiveTopology primitiveTopology,
		PolygonMode polygonMode,
		CullModeBits cullMode,
		FrontFaceMode frontFaceMode,
		float lineWidth)
		{
			mPrimitiveTopology = primitiveTopology;
			mPolygonMode = polygonMode;
			mCullMode = cullMode;
			mFrontFaceMode = frontFaceMode;
			mLineWidth = lineWidth;
		}
	} PipelineProperties;
	

	typedef struct PhysicalDeviceProperties
	{
		uint32_t index;
		uint32_t apiVersion;
		uint32_t driverVersion;
		uint32_t vendorID;
		GPU::GPUDeviceVendor vendorType;
		uint32_t deviceID;
		PhysicalDeviceType deviceType;
		char deviceName[256];
	} PhysicalDeviceProperties;

	typedef struct CanvasDimensions
	{
		HWND hwnd;
		int width;
		int height;

	} CanvasDimensions;

	namespace EngineType
	{
		enum Type
		{
			Unknown,
			DirectX,
			Vulkan
		};
	}


	typedef struct Viewport {
		float    TopLeftX;
		float    TopLeftY;
		float    Width;
		float    Height;
		float    MinDepth;
		float    MaxDepth;
	} Viewport;

	typedef struct Rect {
		int32_t left;
		int32_t top;
		uint32_t right;
		uint32_t bottom;
	} Rect;

	typedef uint32_t ShaderStageFlag;
	

	class IGraphicsEngine
	{
	public:
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version);
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, std::ofstream* ofstr);

		 INVISION_API virtual void Init(MSAAMode msaa = MSAAMODE_OFF) = 0;
		 INVISION_API virtual void Init(unsigned int index, MSAAMode msaa = MSAAMODE_OFF) = 0;

		 INVISION_API EngineType::Type type() const;

		 INVISION_API std::string const& name() const;

		 INVISION_API std::string const& version() const;

		 INVISION_API virtual std::shared_ptr<IGraphicsInstance> CreateInstance(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest = true) = 0;
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