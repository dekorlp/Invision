#ifndef VERTEX3D_H
#define I_VERTEX3D_H

#include "InCommon.h"
#include "math\Vector3.h"

class IVertex;

class Vertex3D : IVertex
{
protected:

private:
	Invision::Vector3 position;
	Invision::Vector3 color;
};

#endif // I_VERTEX3D_H