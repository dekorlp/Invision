#include "EVertexType.h"
#include "IVertex.h"

EVertexType IVertex::getVertexType()
{
	return mVertexType;
}

void IVertex::setVertexType(EVertexType vertexType)
{
	mVertexType = vertexType;
}