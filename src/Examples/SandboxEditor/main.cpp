#include "Invision.h"

class SimpleGame : public Invision
{
	// Inherited via IInvision
	void Initialize() override
	{
	}
	void Update() override
	{
	}
	void Render() override
	{
	}
	void Destroy() override
	{
	}
};



INVISION_MAIN(SimpleGame)