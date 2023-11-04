#include "EngineBackend/Engine.h"

class Game : public Engine
{
	public:
		void init() {
			// init game
			EWindowStatus status = getWindow()->getWindowStatus();
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
			int test = 0;
		}
};



INVISION_MAIN(Game)