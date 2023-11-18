#ifndef MESH_H
#define MESH_H

#include <vector>

class IVertex;
class Vertex2D;
class Vertex3D;

class Mesh
{
public:
	Mesh();
	Mesh(IVertex* vertex);
	Mesh(std::vector<IVertex*> vertizes);


	Mesh(Vertex2D vertex);
	Mesh(std::vector<Vertex2D> vertizes);


	Mesh(Vertex3D vertex);
	Mesh(std::vector<Vertex3D> vertizes);

	IVertex* GetVertex(unsigned int index);
protected:
private:
	std::vector<IVertex*> mVertizes;
};

#endif // MESH_H