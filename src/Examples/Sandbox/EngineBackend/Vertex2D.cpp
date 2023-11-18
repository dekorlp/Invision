#include "IVertex.h"
#include "Vertex2D.h"

Vertex2D::Vertex2D()
{
}

Vertex2D::Vertex2D(Invision::Vector2 position, Invision::Vector3 color)
{
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