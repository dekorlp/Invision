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
	class VulkanTexture;

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

	enum BlendOp
	{
		BLEND_OP_ADD = 0,
		BLEND_OP_SUBTRACT = 1,
		BLEND_OP_REVERSE_SUBTRACT = 2,
		BLEND_OP_MIN = 3,
		BLEND_OP_MAX = 4
	};

	enum BlendFactor
	{
		BLEND_FACTOR_ZERO = 0,
		BLEND_FACTOR_ONE = 1,
		BLEND_FACTOR_SRC_COLOR = 2,
		BLEND_FACTOR_ONE_MINUS_SRC_COLOR = 3,
		BLEND_FACTOR_DST_COLOR = 4,
		BLEND_FACTOR_ONE_MINUS_DST_COLOR = 5,
		BLEND_FACTOR_SRC_ALPHA = 6,
		BLEND_FACTOR_ONE_MINUS_SRC_ALPHA = 7,
		BLEND_FACTOR_DST_ALPHA = 8,
		BLEND_FACTOR_ONE_MINUS_DST_ALPHA = 9,
		BLEND_FACTOR_SRC_ALPHA_SATURATE = 14,
		BLEND_FACTOR_SRC1_COLOR = 15,
		BLEND_FACTOR_ONE_MINUS_SRC1_COLOR = 16,
		BLEND_FACTOR_SRC1_ALPHA = 17,
		BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA = 18
	};

	enum CompareOp
	{
		COMPARE_OP_NEVER = 0,
		COMPARE_OP_LESS = 1,
		COMPARE_OP_EQUAL = 2,
		COMPARE_OP_LESS_OR_EQUAL = 3,
		COMPARE_OP_GREATER = 4,
		COMPARE_OP_NOT_EQUAL = 5,
		COMPARE_OP_GREATER_OR_EQUAL = 6,
		COMPARE_OP_ALWAYS = 7
	};

	enum GfxFormat
	{
		FORMAT_UNEDFINED,
		FORMAT_R32G32B32A32_SFLOAT,
		FORMAT_R32G32B32A32_UINT,
		FORMAT_R32G32B32A32_SINT,
		FORMAT_R32G32B32_SFLOAT,
		FORMAT_R32G32B32_UINT,
		FORMAT_R32G32B32_SINT,
		FORMAT_R16G16B16A16_SFLOAT,
		FORMAT_R16G16B16A16_UNORM,
		FORMAT_R16G16B16A16_UINT,
		FORMAT_R16G16B16A16_SNORM,
		FORMAT_R16G16B16A16_SINT,
		FORMAT_R32G32_SFLOAT,
		FORMAT_R32G32_UINT,
		FORMAT_R32G32_SINT,
		FORMAT_R8G8B8A8_UNORM,
		FORMAT_R8G8B8A8_SRGB,
		FORMAT_R8G8B8A8_UINT,
		FORMAT_R8G8B8A8_SNORM,
		FORMAT_R8G8B8A8_SINT,
		FORMAT_R16G16_SFLOAT,
		FORMAT_R16G16_UNORM,
		FORMAT_R16G16_UINT,
		FORMAT_R16G16_SNORM,
		FORMAT_R16G16_SINT,
		FORMAT_D32_SFLOAT,
		FORMAT_R32_SFLOAT,
		FORMAT_R32_UINT,
		FORMAT_R32_SINT,
		FORMAT_D24_UNORM_S8_UINT,
		FORMAT_R8G8_UNORM,
		FORMAT_R8G8_UINT,
		FORMAT_R8G8_SNORM,
		FORMAT_R8G8_SINT,
		FORMAT_R16_SFLOAT,
		FORMAT_D16_UNORM,
		FORMAT_R16_UNORM,
		FORMAT_R16_UINT,
		FORMAT_R16_SNORM,
		FORMAT_R16_SINT,
		FORMAT_R8_UNORM,
		FORMAT_R8_UINT,
		FORMAT_R8_SNORM,
		FORMAT_R8_SINT,
		FORMAT_BC6H_UFLOAT,
		FORMAT_BC6H_SFLOAT,
		FORMAT_BC7_UNORM,
		FORMAT_BC7_SRGB
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

	enum SamplerAddressMode
	{
		SAMPLER_ADDRESS_MODE_REPEAT = 0x00,
		SAMPLER_ADDRESS_MODE_CLAMP = 0x01,
		SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT = 0x02,
		VK_SAMPLER_ADDRESS_MODE_MIRRORED_ONCE = 0x03
	};

	enum SamplerFilterMode
	{
		SAMPLER_FILTER_MODE_LINEAR = 0x00,
		SAMPLER_FILTER_MODE_NEAREST = 0x01
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


	enum AttachmentType
	{
		ATTACHMENT_TYPE_COLOR,
		ATTACHMENT_TYPE_DEPTH,
		ATTACHMENT_TYPE_DEPTH_STENCIL
	};

	typedef uint32_t ShaderStageFlag;
	

	class IGraphicsEngine
	{
	public:
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version);
		 INVISION_API IGraphicsEngine(EngineType::Type type, std::string const& name, std::string const& version, std::ofstream* ofstr);

		 INVISION_API virtual void Init() = 0;
		 INVISION_API virtual void Init(unsigned int index) = 0;

		 INVISION_API EngineType::Type GetType() const;

		 INVISION_API std::string const& GetName() const;

		 INVISION_API std::string const& GetVersion() const;

		 INVISION_API virtual std::shared_ptr<IGraphicsInstance> CreateInstance(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest = true, MSAAMode msaaMode = MSAAMode::MSAAMODE_SAMPLE_COUNT_BEST) = 0;
		 INVISION_API virtual std::vector< PhysicalDeviceProperties> GetPhysicalDevices() = 0;


		 INVISION_API ~IGraphicsEngine();
	protected:
		EngineType::Type mType = EngineType::Unknown;
		PhysicalDeviceProperties mDeviceProperties;
		std::string const mName;
		std::string const mVersion;
	};

}

#endif // IGRAPHICS_ENGINE_H