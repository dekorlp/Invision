#include <vector>
#include "EngineBackend/Engine.h"
#include "EngineBackend/Vertex3D.h"
#include "EngineBackend/Vertex2D.h"
#include "EngineBackend/Mesh.h"

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

			 mesh = new Mesh(vertices, indices);

			IVertex *vert =  mesh->GetVertex(3);
			uint32_t index = mesh->GetIndex(3);

			AddMesh(mesh);
			int test = 0;
			
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
			delete mesh;
			int test = 0;
		}


private:
	Mesh* mesh;
};



INVISION_MAIN(Game)