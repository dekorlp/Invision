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

Mesh::Mesh(std::vector<IVertex*> vertizes)
{
	mVertizes = vertizes;
}

Mesh::Mesh(Vertex2D vertex)
{
	mVertizes.push_back(&vertex);
}

Mesh::Mesh(std::vector<Vertex2D> vertizes)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(&vertizes[i]);
	}
}

Mesh::Mesh(Vertex3D vertex)
{
	mVertizes.push_back(&vertex);
}

Mesh::Mesh(std::vector<Vertex3D> vertizes)
{
	for (int i = 0; i < vertizes.size(); i++)
	{
		mVertizes.push_back(&vertizes[i]);
	}
}

IVertex* Mesh::GetVertex(unsigned int index) {
	return mVertizes.at(index);
}