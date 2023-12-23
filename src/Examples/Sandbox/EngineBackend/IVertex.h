#ifndef I_VERTEX_H
#define I_VERTEX_H


#include "InCommon.h"
#include "math\Vector2.h"
#include "math\Vector3.h"

enum EVertexType;

class IVertex
{
public:
	virtual EVertexType getVertexType();
	virtual void setVertexType(EVertexType vertexType);

private:
	EVertexType mVertexType;
};

#endif // I_VERTEX_H