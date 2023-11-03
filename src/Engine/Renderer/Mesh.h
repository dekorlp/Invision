#ifndef MESH_H
#define MESH_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix.h"

#include "Renderer/Common/IPipeline.h"
#include "Renderer/Common/IUniformBuffer.h"
#include "Renderer/Common/ITexture.h"
#include "Common/IIndexBuffer.h"

namespace Invision
{
	struct Vertex {
		Invision::Vector2 position;
		Invision::Vector3 color;
		Invision::Vector2 texCoord;
	};

	class Mesh
	{
	public:
		Mesh(std::shared_ptr <Invision::IGraphicsInstance> &graphicsInstance, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, std::vector<char> &vertexShader, std::vector<char> &fragmentShader);
		~Mesh();

	private:
		std::shared_ptr <Invision::IUniformBuffer> mUniformBuffer;
		std::shared_ptr <Invision::ITexture> mTexture;
		std::shared_ptr <Invision::IPipeline> mPipeline;
		std::shared_ptr <Invision::IVertexBuffer> mVertexBuffer;
		std::shared_ptr <Invision::IIndexBuffer> mIndexBuffer;
	};
}

#endif // MESH_H