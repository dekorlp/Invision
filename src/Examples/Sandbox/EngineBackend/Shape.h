#ifndef SHAPE_H
#define SHAPE_H
#include "EVertexType.h"
#include <vector>
#include <iostream>
#include "InCommon.h"
#include "renderer/GraphicsFactory.h"
#include <Math/Matrix.h>
#include "IMesh.h"

class Vertex2D;

struct UniformBufferObject2 {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

class Shape : public IMesh
{
public:
	Shape();
	Shape(Vertex2D vertex);
	Shape(Vertex2D vertex, uint32_t index);
	Shape(std::vector<Vertex2D> vertices);
	Shape(std::vector<Vertex2D> vertices, std::vector<uint32_t> indices);

	Vertex2D GetVertex(unsigned int index);
	uint32_t GetIndex(unsigned int index);

	std::vector<Vertex2D> GetVertices();
	std::vector<uint32_t> GetIndices();

	std::shared_ptr <Invision::IVertexBuffer> GetVertexBuffer();
	std::shared_ptr <Invision::IUniformBuffer> GetUniformBuffer();
	std::shared_ptr <Invision::IIndexBuffer> GetIndexBuffer();
	std::shared_ptr <Invision::IPipeline> GetPipeline();

	void Initialize(const std::shared_ptr <Invision::IGraphicsInstance>& graphicsInstance, std::shared_ptr<Invision::IRenderPass>& renderPass);

	bool HasIndexBuffer();
	EVertexType GetVertexType();
	size_t GetVertexCount();

	~Shape();
private:
	std::vector<char> readFile(const std::string& filename);
protected:
private:
	std::vector<Vertex2D> mVertices;
	std::vector<uint32_t> mIndices;
	EVertexType mVertexType;
	bool mHasIndixBuffer;

	std::shared_ptr <Invision::IGraphicsInstance> mGraphicsInstance;
	std::shared_ptr <Invision::IVertexBuffer> mVertexBuffer;
	std::shared_ptr <Invision::IUniformBuffer> mUniformBuffer;
	std::shared_ptr <Invision::IIndexBuffer> mIndexBuffer;
	std::shared_ptr <Invision::IPipeline> mPipeline;
};

#endif // SHAPE_H