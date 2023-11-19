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
	Mesh(IVertex* vertex, uint32_t index);
	Mesh(std::vector<IVertex*> vertizes);
	Mesh(std::vector<IVertex*> vertizes, std::vector<uint32_t> indices);


	Mesh(Vertex2D vertex);
	Mesh(Vertex2D vertex, uint32_t index);
	Mesh(std::vector<Vertex2D> vertizes);
	Mesh(std::vector<Vertex2D> vertizes, std::vector<uint32_t> indices);


	Mesh(Vertex3D vertex);
	Mesh(Vertex3D vertex, uint32_t index);
	Mesh(std::vector<Vertex3D> vertizes);
	Mesh(std::vector<Vertex3D> vertizes, std::vector<uint32_t> indices);

	IVertex* GetVertex(unsigned int index);
	uint32_t GetIndex(unsigned int index);

	std::vector<IVertex*> GetVertizes();
	std::vector<uint32_t> GetIndices();

	~Mesh();
protected:
private:
	std::vector<IVertex*> mVertizes;
	std::vector<uint32_t> mIndices;
};

#endif // MESH_H