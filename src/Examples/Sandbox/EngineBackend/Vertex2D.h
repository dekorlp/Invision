#ifndef VERTEX2D_H
#define I_VERTEX2D_H

#include "InCommon.h"
#include "math\Vector2.h"
#include "math\Vector3.h"

class IVertex;

class Vertex2D : IVertex
{
protected:

private:
	Invision::Vector2 position;
	Invision::Vector3 color;

};

#endif // I_VERTEX2D_H