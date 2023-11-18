#ifndef I_VERTEX_H
#define I_VERTEX_H


#include "InCommon.h"
#include "math\Vector2.h"
#include "math\Vector3.h"

class IVertex
{
protected:
	virtual Invision::Vector3 getColor() = 0;
};

#endif // I_VERTEX_H