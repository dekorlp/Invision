#ifndef VERTEX3D_H
#define I_VERTEX3D_H

#include "InCommon.h"
#include "math\Vector3.h"
#include "IVertex.h"


enum EVertexType;

class Vertex3D
{
public:
	Vertex3D();
	Vertex3D(Invision::Vector3 position, Invision::Vector3 color);
	~Vertex3D();

	Invision::Vector3 getPosition();
	Invision::Vector3 getColor();
	static size_t getPositionOffset() {
		return offsetOf(&Vertex3D::position);
	}
	static size_t getColorOffset() {
		return offsetOf(&Vertex3D::color);
	}

	EVertexType getVertexType();
	void setVertexType(EVertexType vertexType);
protected:

private:
	Invision::Vector3 position;
	Invision::Vector3 color;
	EVertexType mVertexType;
};

#endif // I_VERTEX3D_H