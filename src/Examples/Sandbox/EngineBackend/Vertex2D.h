#ifndef VERTEX2D_H
#define I_VERTEX2D_H

#include "InCommon.h"
#include "math\Vector2.h"
#include "math\Vector3.h"
#include "IVertex.h"

enum EVertexType;

class Vertex2D : public IVertex
{
private:
	Invision::Vector2 position;
	Invision::Vector3 color;

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
protected:


};

#endif // I_VERTEX2D_H