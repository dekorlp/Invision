#include <vector>
#include "EngineBackend/Engine.h"
#include "EngineBackend/Vertex3D.h"
#include "EngineBackend/Mesh.h"

class Game : public Engine
{
	public:
		void init() {
			// init game
			EWindowStatus status = getWindow()->getWindowStatus();
			getWindow()->setWindowTitle("Hallo Welt");
			
			std::vector<Vertex3D> vertizes{ {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},

			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}} };

			const std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
					};

			Mesh *mesh = new Mesh(vertizes, indices);

			IVertex *vert =  mesh->GetVertex(3);
			uint32_t index = mesh->GetIndex(3);

			int test = 0;
			delete mesh;
			
		}
		void update() {
			// update game
			EWindowStatus status = getWindow()->getWindowStatus();
			int test = 0;
		}
		void render() {
			// render game
			EWindowStatus status = getWindow()->getWindowStatus();
			int test = 0;
		}
		void destroy() {
			// destroy game
			EWindowStatus status = getWindow()->getWindowStatus();
			int test = 0;
		}
};



INVISION_MAIN(Game)