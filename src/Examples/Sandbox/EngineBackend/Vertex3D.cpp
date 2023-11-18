#include "IVertex.h"
#include "Vertex3D.h"

Vertex3D::Vertex3D()
{
}

Vertex3D::Vertex3D(Invision::Vector3 position, Invision::Vector3 color)
{
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