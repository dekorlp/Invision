#ifndef VERTEX2D_H
#define I_VERTEX2D_H

#include "InCommon.h"
#include "math\Vector2.h"
#include "math\Vector3.h"

enum EVertexType;

class Vertex2D
{
private:
	Invision::Vector2 position;
	Invision::Vector3 color;
	EVertexType mVertexType;

public:
	Vertex2D();
	Vertex2D(Invision::Vector2 position, Invision::Vector3 color);
	~Vertex2D();

	Invision::Vector2 getPosition();
	Invision::Vector3 getColor();
	static size_t getPositionOffset() {
		return offsetOf(&Vertex2D::position); 
	}
	static size_t getColorOffset() {
		return offsetOf(&Vertex2D::color);
	}

	EVertexType getVertexType();
	void setVertexType(EVertexType vertexType);
protected:


};

#endif // I_VERTEX2D_H