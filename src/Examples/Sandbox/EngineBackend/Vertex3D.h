#ifndef VERTEX3D_H
#define I_VERTEX3D_H

#include "InCommon.h"
#include "math\Vector3.h"
#include "IVertex.h"

enum EVertexType;

class Vertex3D : public IVertex
{
public:
	Vertex3D();
	Vertex3D(Invision::Vector3 position, Invision::Vector3 color);
	~Vertex3D();

	Invision::Vector3 getPosition();
	Invision::Vector3 getColor();
protected:

private:
	Invision::Vector3 position;
	Invision::Vector3 color;
};

#endif // I_VERTEX3D_H