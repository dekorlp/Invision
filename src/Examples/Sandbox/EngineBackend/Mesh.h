#ifndef MESH_H
#define MESH_H
#include "EVertexType.h"
#include <vector>
#include <iostream>
#include "InCommon.h"
#include "renderer/GraphicsFactory.h"
#include <Math/Matrix.h>

class IVertex;
class Vertex2D;
class Vertex3D;

struct UniformBufferObject {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

class Mesh
{
public:
	Mesh();
	Mesh(IVertex* vertex);
	Mesh(IVertex* vertex, uint32_t index);
	Mesh(std::vector<IVertex*> vertices);
	Mesh(std::vector<IVertex*> vertices, std::vector<uint32_t> indices);


	Mesh(Vertex2D vertex);
	Mesh(Vertex2D vertex, uint32_t index);
	Mesh(std::vector<Vertex2D> vertices);
	Mesh(std::vector<Vertex2D> vertices, std::vector<uint32_t> indices);


	Mesh(Vertex3D vertex);
	Mesh(Vertex3D vertex, uint32_t index);
	Mesh(std::vector<Vertex3D> vertices);
	Mesh(std::vector<Vertex3D> vertices, std::vector<uint32_t> indices);

	IVertex* GetVertex(unsigned int index);
	uint32_t GetIndex(unsigned int index);

	std::vector<IVertex*> GetVertices();
	std::vector<uint32_t> GetIndices();

	std::shared_ptr <Invision::IVertexBuffer> GetVertexBuffer();
	std::shared_ptr <Invision::IUniformBuffer> GetUniformBuffer();
	std::shared_ptr <Invision::IIndexBuffer> GetIndexBuffer();
	std::shared_ptr <Invision::IPipeline> GetPipeline();

	void Initialize(const std::shared_ptr <Invision::IGraphicsInstance> &graphicsInstance, std::shared_ptr<Invision::IRenderPass>& renderPass);

	bool HasIndexBuffer();
	EVertexType GetVertexType();

	~Mesh();
private:
	std::vector<char> readFile(const std::string& filename);
protected:
private:
	std::vector<IVertex*> mVertices;
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