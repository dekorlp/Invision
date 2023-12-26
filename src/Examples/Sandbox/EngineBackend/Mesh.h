#ifndef MESH_H
#define MESH_H
#include "EVertexType.h"
#include <vector>
#include <iostream>
#include "InCommon.h"
#include "renderer/GraphicsFactory.h"
#include <Math/Matrix.h>
#include "IRenderable.h"

class Vertex2D;
class Vertex3D;

struct UniformBufferObject {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

class Mesh : public IRenderable
{
public:
	Mesh();

	Mesh(Vertex3D vertex);
	Mesh(Vertex3D vertex, uint32_t index);
	Mesh(std::vector<Vertex3D> vertices);
	Mesh(std::vector<Vertex3D> vertices, std::vector<uint32_t> indices);

	Vertex3D GetVertex(unsigned int index);
	uint32_t GetIndex(unsigned int index);

	std::vector<Vertex3D> GetVertices();
	std::vector<uint32_t> GetIndices();

	std::shared_ptr <Invision::IVertexBuffer> GetVertexBuffer();
	std::shared_ptr <Invision::IUniformBuffer> GetUniformBuffer();
	std::shared_ptr <Invision::IIndexBuffer> GetIndexBuffer();
	std::shared_ptr <Invision::IPipeline> GetPipeline();

	void Initialize(const std::shared_ptr <Invision::IGraphicsInstance> &graphicsInstance, std::shared_ptr<Invision::IRenderPass>& renderPass);

	bool HasIndexBuffer();
	EVertexType GetVertexType();
	size_t GetVertexCount();

	~Mesh();
private:
	std::vector<char> readFile(const std::string& filename);
protected:
private:
	std::vector<Vertex3D> mVertices;
	std::vector<uint32_t> mIndices;
	EVertexType mVertexType;
	bool mHasIndixBuffer;

	std::shared_ptr <Invision::IGraphicsInstance> mGraphicsInstance;
	std::shared_ptr <Invision::IVertexBuffer> mVertexBuffer;
	std::shared_ptr <Invision::IUniformBuffer> mUniformBuffer;
	std::shared_ptr <Invision::IIndexBuffer> mIndexBuffer;
	std::shared_ptr <Invision::IPipeline> mPipeline;
};

#endif // MESH_H