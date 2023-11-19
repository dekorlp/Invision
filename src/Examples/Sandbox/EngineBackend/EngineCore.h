#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <vector>

#include "OSWinWindow.h"

#include "IEngine.h"

class Mesh;

class EngineCore
{
private:
	IEngine* mEngine = nullptr;
	std::vector<Mesh*> mMeshes;
public:
	EngineCore(IEngine* engineInstance);
#if defined(_WIN32)
	void Create(HWND hwnd, unsigned int width, unsigned int height);
#elif defined(__linux__)

#endif
	void Render(unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Shutdown();
	void AddMesh(Mesh* mesh);

};

#endif // ENGINE_CORE_H