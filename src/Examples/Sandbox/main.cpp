#include "EngineBackend/Engine.h"

class Game : public Engine
{
	public:
		void init() {
			// init game
			getWindow()->isWindowActive();
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