#include "IVertex.h"
#include "Vertex2D.h"
#include "Vertex3D.h"
#include "Mesh.h"

Mesh::Mesh()
{
	
}

Mesh::Mesh(IVertex* vertex)
{
	mVertizes.push_back(vertex);
}

Mesh::Mesh(IVertex* vertex, uint32_t index)
{
	mVertizes.push_back(vertex);
	mIndices.push_back(index);
}

Mesh::Mesh(std::vector<IVertex*> vertizes)
{
	mVertizes = vertizes;
}

Mesh::Mesh(std::vector<IVertex*> vertizes, std::vector<uint32_t> indices)
{
	mVertizes = vertizes;
	mIndices = indices;
}

Mesh::Mesh(Vertex2D vertex)
{
	mVertizes.push_back(new Vertex2D(vertex));
}

Mesh::Mesh(Vertex2D vertex, uint32_t index)
{
	mVertizes.push_back(new Vertex2D(vertex));
	mIndices.push_back(index);
}

Mesh::Mesh(std::vector<Vertex2D> vertizes)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(new Vertex2D(vertizes[i]));
	}
}

Mesh::Mesh(std::vector<Vertex2D> vertizes, std::vector<uint32_t> indices)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(new Vertex2D(vertizes[i]));
	}
	mIndices = indices;
}

Mesh::Mesh(Vertex3D vertex)
{
	mVertizes.push_back(new Vertex3D(vertex));
}

Mesh::Mesh(Vertex3D vertex, uint32_t index)
{
	mVertizes.push_back(new Vertex3D(vertex));
	mIndices.push_back(index);
}

Mesh::Mesh(std::vector<Vertex3D> vertizes)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(new Vertex3D(vertizes[i]));
	}
}

Mesh::Mesh(std::vector<Vertex3D> vertizes, std::vector<uint32_t> indices)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(new Vertex3D(vertizes[i]));
	}
	mIndices = indices;
}

IVertex* Mesh::GetVertex(unsigned int index) {
	return mVertizes.at(index);
}

uint32_t Mesh::GetIndex(unsigned int index)
{
	return mIndices.at(index);
}

std::vector<IVertex*> Mesh::GetVertizes()
{
	return mVertizes;
}

std::vector<uint32_t> Mesh::GetIndices()
{
	return mIndices;
}

Mesh::~Mesh()
{
	for (int i = 0; i < mVertizes.size(); i++)
	{
		delete mVertizes[i];
	}
}