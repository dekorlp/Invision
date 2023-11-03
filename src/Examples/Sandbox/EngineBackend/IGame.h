#ifndef IGAME_H
#define IGAME_H

#include "GameContext.h"

class IGame
{
	public:
		virtual void init(GameContext& gameContext) = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void destroy() = 0;
};

#endif // IGAME_H