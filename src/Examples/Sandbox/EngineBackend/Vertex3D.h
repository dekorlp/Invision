#ifndef VERTEX3D_H
#define I_VERTEX3D_H

#include "InCommon.h"
#include "math\Vector3.h"

class IVertex;

class Vertex3D : IVertex
{
protected:
public:
	Vertex3D();
	Vertex3D(Invision::Vector3 position, Invision::Vector3 color);
	~Vertex3D();

	Invision::Vector3 getPosition();
	Invision::Vector3 getColor();
private:
	Invision::Vector3 position;
	Invision::Vector3 color;
};

#endif // I_VERTEX3D_H