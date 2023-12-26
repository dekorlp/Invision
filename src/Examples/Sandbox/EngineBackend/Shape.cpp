#include "Vertex2D.h"
#include "Vertex3D.h"
#include "Shape.h"

Shape::Shape()
{
	mHasIndixBuffer = false;
	mVertexType = UNDEFINED;
}

Shape::Shape(Vertex2D vertex)
{
	mVertices.push_back(vertex);
	mVertexType = vertex.getVertexType();
	mHasIndixBuffer = false;
}

Shape::Shape(Vertex2D vertex, uint32_t index)
{
	mVertices.push_back(vertex);
	mIndices.push_back(index);
	mVertexType = vertex.getVertexType();
	mHasIndixBuffer = true;
}

Shape::Shape(std::vector<Vertex2D> vertices)
{
	mVertices = vertices;
	if (vertices.size() > 0)
	{
		mVertexType = vertices[0].getVertexType();
	}
	mHasIndixBuffer = false;
}

Shape::Shape(std::vector<Vertex2D> vertices, std::vector<uint32_t> indices)
{
	mVertices = vertices;
	mIndices = indices;

	if (vertices.size() > 0)
	{
		mVertexType = vertices[0].getVertexType();
	}
	mHasIndixBuffer = true;
}

Vertex2D Shape::GetVertex(unsigned int index) {
	return mVertices.at(index);
}

uint32_t Shape::GetIndex(unsigned int index)
{
	return mIndices.at(index);
}

bool Shape::HasIndexBuffer()
{
	return mHasIndixBuffer;
}

EVertexType Shape::GetVertexType()
{
	return mVertexType;
}

size_t Shape::GetVertexCount()
{
	return mVertices.size();
}

std::vector<Vertex2D> Shape::GetVertices()
{
	return mVertices;
}

std::vector<uint32_t> Shape::GetIndices()
{
	return mIndices;
}

std::shared_ptr <Invision::IVertexBuffer> Shape::GetVertexBuffer()
{
	return mVertexBuffer;
}

std::shared_ptr <Invision::IUniformBuffer> Shape::GetUniformBuffer()
{
	return mUniformBuffer;
}

std::shared_ptr <Invision::IIndexBuffer> Shape::GetIndexBuffer()
{
	return mIndexBuffer;
}

std::shared_ptr <Invision::IPipeline> Shape::GetPipeline()
{
	return mPipeline;
}

void Shape::Initialize(const std::shared_ptr <Invision::IGraphicsInstance>& graphicsInstance, std::shared_ptr<Invision::IRenderPass>& renderPass)
{
	mGraphicsInstance = graphicsInstance;
	mVertexBuffer = graphicsInstance->CreateVertexBuffer();
	mUniformBuffer = graphicsInstance->CreateUniformBuffer();
	if (mHasIndixBuffer)
	{
		mIndexBuffer = graphicsInstance->CreateIndexBuffer();
	}
	mPipeline = graphicsInstance->CreatePipeline();


	std::shared_ptr<Invision::IVertexBindingDescription> verBindingDescr;
	if (mVertexType == VERTEX2D)
	{
		verBindingDescr = graphicsInstance->CreateVertexBindingDescription();
		verBindingDescr->CreateVertexBinding(0, sizeof(Vertex2D), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, (uint32_t)Vertex2D::getPositionOffset())
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, (uint32_t)Vertex3D::getColorOffset());
	}
	else //(mVertexType == VERTEX3D)
	{
		verBindingDescr = graphicsInstance->CreateVertexBindingDescription();
		verBindingDescr->CreateVertexBinding(0, sizeof(Vertex3D), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32B32_SFLOAT, (uint32_t)Vertex3D::getPositionOffset())
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, (uint32_t)Vertex3D::getColorOffset());
	}


	mVertexBuffer->CreateBuffer(mVertices.data(), sizeof(mVertices[0]) * mVertices.size(), 0, verBindingDescr);

	if (mHasIndixBuffer)
	{
		mIndexBuffer->CreateIndexBuffer(sizeof(mIndices[0]) * mIndices.size(), mIndices.data());
	}

	mUniformBuffer->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject2)).CreateUniformBuffer();

	auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/QTDemoApp/Shader/DrawUniformBuffer/vert.spv"));
	auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/QTDemoApp/Shader/DrawUniformBuffer/frag.spv"));
	mPipeline->AddUniformBuffer(mUniformBuffer);
	mPipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
	mPipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
	mPipeline->AddVertexDescription(verBindingDescr);
	mPipeline->CreatePipeline(renderPass);

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject2 ubo = {};
	ubo.model = Invision::Matrix(1.0f) * Invision::Matrix::RotateZ(time * 90.0);
	ubo.view = Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Invision::Matrix::Perspective(45.0, 1900 / 1037, 0.1f, 10.0f);
	mUniformBuffer->UpdateUniform(&ubo, sizeof(ubo), 0, 0);
}

std::vector<char> Shape::readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

Shape::~Shape()
{

}