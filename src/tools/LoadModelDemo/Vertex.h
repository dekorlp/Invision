#ifndef VERTEX_H
#define VERTEX_H

#include "InCommon.h"

#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Matrix.h"

struct Vertex {
	Invision::Vector3 position;
	Invision::Vector3 color;
	Invision::Vector2 texCoord;

	bool operator==(const Vertex& other) const {
		return position == other.position && color == other.color && texCoord == other.texCoord;
	}
};

/*namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {

			hash<Invision::Vector3>()(vertex.position);

			return ((hash<Invision::Vector3>()(vertex.position) ^
				(hash<Invision::Vector3>()(vertex.color) << 1)) >> 1) ^
				(hash<Invision::Vector3>()(vertex.texCoord) << 1);
		}
	};
}*/

#endif
