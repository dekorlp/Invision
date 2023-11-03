#include "EngineBackend/EngineBackend.h"
#include "EngineBackend/IGame.h"

class Game : public IGame
{
	public:
		void init(WindowContext&windowContext, GameContext& gameContext) {
			// init game
			windowContext.setWindowName(L"Sandbox App");
			windowContext.setWindowSize(800, 600);
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