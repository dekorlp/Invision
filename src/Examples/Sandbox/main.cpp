#include <vector>
#include "EngineBackend/Engine.h"
#include "EngineBackend/Vertex3D.h"
#include "EngineBackend/Vertex2D.h"
#include "EngineBackend/Shape.h"
#include "EngineBackend/IRenderable.h"

class Game : public Engine
{
	public:
		void init() {
			// init game
			EWindowStatus status = getWindow()->getWindowStatus();
			getWindow()->setWindowTitle("Hallo Welt");


			std::vector<Vertex2D> vertices { {{ -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
			{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{ 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{ -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}} };


			const std::vector<uint32_t> indices = {
				0, 1, 2, 0
			};

			mesh = new Shape(vertices, indices);

			Vertex2D vert =  dynamic_cast<Shape*>(mesh)->GetVertex(3);
			uint32_t index = dynamic_cast<Shape*>(mesh)->GetIndex(3);

			AddMesh(mesh);
			
		}
		void update() {
			// update game
			EWindowStatus status = getWindow()->getWindowStatus();
		}
		void render() {
			// render game
			EWindowStatus status = getWindow()->getWindowStatus();
		}
		void destroy() {
			// destroy game
			EWindowStatus status = getWindow()->getWindowStatus();
		}


private:
	IRenderable* mesh;
};



INVISION_MAIN(Game)