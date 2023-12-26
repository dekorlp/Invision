#include "EVertexType.h"
#include "Vertex2D.h"

Vertex2D::Vertex2D()
{
	setVertexType(EVertexType::VERTEX2D);
	position = Invision::Vector2(0.0f, 0.0f);
	color = Invision::Vector3(0.0f, 0.0f, 0.0f);
}

Vertex2D::Vertex2D(Invision::Vector2 position, Invision::Vector3 color)
{
	setVertexType(EVertexType::VERTEX2D);
	this->position = position;
	this->color = color;
}

Vertex2D::~Vertex2D()
{
}

Invision::Vector2 Vertex2D::getPosition()
{
	return position;
}

Invision::Vector3 Vertex2D::getColor()
{
	return color;
}

EVertexType Vertex2D::getVertexType()
{
	return mVertexType;
}

void Vertex2D::setVertexType(EVertexType vertexType)
{
	mVertexType = vertexType;
}