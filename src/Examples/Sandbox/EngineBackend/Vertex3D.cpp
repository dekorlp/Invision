#include "EVertexType.h"
#include "Vertex3D.h"

Vertex3D::Vertex3D()
{
	setVertexType(EVertexType::VERTEX3D);
	position = Invision::Vector3(0.0f, 0.0f, 0.0f);
	color = Invision::Vector3(0.0f, 0.0f, 0.0f);
}

Vertex3D::Vertex3D(Invision::Vector3 position, Invision::Vector3 color)
{
	setVertexType(EVertexType::VERTEX3D);
	this->position = position;
	this->color = color;
}

Vertex3D::~Vertex3D()
{
}

Invision::Vector3 Vertex3D::getPosition()
{
	return position;
}

Invision::Vector3 Vertex3D::getColor()
{
	return color;
}

EVertexType Vertex3D::getVertexType()
{
	return mVertexType;
}

void Vertex3D::setVertexType(EVertexType vertexType)
{
	mVertexType = vertexType;
}