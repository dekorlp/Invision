#include "EngineBackend/EngineBackend.h"
#include "EngineBackend/IGame.h"

class Game : public IGame
{
	public:
		void init(GameContext& context) {
			// init game
			context.setWindowName(L"Sandbox App");
			context.setWindowSize(800, 600);
		}
		void update() {
			// update game
		}
		void render() {
			// render game
		}
		void destroy() {
			// destroy game
		}
};



INVISION_MAIN(Game)