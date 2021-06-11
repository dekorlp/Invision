#ifndef VERTEX_H
#define VERTEX_H

#include "Engine\InCommon.h"

#include "Engine\math\Vector2.h"
#include "Engine\math\Vector3.h"
#include "Engine\math\Matrix.h"

struct Vertex {
	Invision::Vector3 position;
	Invision::Vector3 color;
	Invision::Vector2 texCoord;

	bool operator==(const Vertex& other) const {

		return position == other.position && color == other.color && texCoord == other.texCoord;
	}
};

/*// custom hash can be a standalone function object:
template <class T> struct CustomHashVektor3;
template<> struct CustomHashVektor3<Invision::Vector3>
{
	std::size_t operator()(Invision::Vector3 const& s) const
	{
		std::size_t h1 = std::hash<float>()(s.getX());
		std::size_t h2 = std::hash<float>()(s.getY());
		std::size_t h3 = std::hash<float>()(s.getZ());
		std::size_t seed = 0; 
		seed = h1 ^ (h2 << 1);
		seed = seed ^ (h3 << 1);
		return seed; // or use boost::hash_combine
	}
};

template <class T> struct CustomHashVektor2;
template<> struct CustomHashVektor2<Invision::Vector2>
{
	std::size_t operator()(Invision::Vector2 const& s) const
	{
		std::size_t h1 = std::hash<float>()(s.getX());
		std::size_t h2 = std::hash<float>()(s.getY());
		std::size_t seed = 0;
		seed = h1 ^ (h2 << 1);
		return seed; // or use boost::hash_combine
	}
};*/



namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {

			//CustomHashVektor3<Invision::Vector3>()(vertex.position);

			return ((vertex.position.GetHash() ^
				(vertex.color.GetHash() << 1)) >> 1) ^
				(vertex.texCoord.GetHash() << 1);
		}
	};
}

#endif
