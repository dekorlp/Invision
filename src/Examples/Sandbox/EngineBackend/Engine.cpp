#include "IMesh.h"
#include "Mesh.h"
#include "IEngine.h"
#include "Engine.h"

Engine::Engine()
{
	try
	{
		log.Open(std::string(INVISION_BASE_DIR).append("/logs/logQTDemo.txt"));
		Invision::Log::SetLogger(&log);

		mWindow = new Window();

#ifdef NDEBUG
		mGraphicsEngine = Invision::create_engine(Invision::EngineType::Vulkan);
#else
		mGraphicsEngine = Invision::Create_engine(Invision::EngineType::Vulkan, log.GetOutputStream());
#endif
		mGraphicsEngine->Init();

		mEngineCore = new EngineCore(this, &mGraphicsEngine);

	}
	catch (std::runtime_error& err) {
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << err.what();
		INVISION_LOG_ERROR(ss.str());
	}
	catch (Invision::InvisionException& iEx)
	{
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << iEx.what();
		INVISION_LOG_ERROR(ss.str());
	}
	catch (Invision::InvisionBaseRendererException& iEx)
	{
		std::stringstream ss;
		ss << "Vulkan Error encountered trying to create the Vulkan canvas:\n";
		ss << iEx.what();
		INVISION_LOG_ERROR(ss.str());
	}
}

#if defined(_WIN32)
WPARAM Engine::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    return dynamic_cast<OSWinWindow*>(mWindow)->createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mEngineCore, 1920, 1080, L"Demo App");
}
#elif defined(__linux__)

#endif

Window* Engine::getWindow() {
    return dynamic_cast<Window*>(mWindow);
}

void Engine::AddMesh(IMesh* mesh)
{
    mEngineCore->AddMesh(mesh);
}

Engine::~Engine()
{
    delete mWindow;
	delete mEngineCore;
}