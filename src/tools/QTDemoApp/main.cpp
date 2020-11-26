
#include "MainWindow2.h"

#include "InCommon.h"


int main(int argc, char *argv[]) {

	QApplication app(argc, argv);
	Invision::Log log;
	log.Open(std::string(INVISION_BASE_DIR).append("/logs/logQTDemo.txt"));
	Invision::Log::SetLogger(&log);

	MainWindow2 window(log.GetOutputStream());

	try
	{
		window.show();
	}
	catch (std::runtime_error& err) {
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << err.what();
		// TODO OutputDebug Replace with own MessageBox
		//wxMessageBox( ss.str(), "Info");
		INVISION_LOG_ERROR(ss.str());
		return false;
	}
	catch (Invision::InvisionException& iEx)
	{
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << iEx.what();
		// TODO OutputDebug Replace with own MessageBox
		//wxMessageBox( ss.str(), "Info");
		INVISION_LOG_ERROR(ss.str());
		return false;
	}
	catch (Invision::InvisionBaseRendererException& iEx)
	{
		std::stringstream ss;
		ss << "Vulkan Error encountered trying to create the Vulkan canvas:\n";
		ss << iEx.what();
		// TODO OutputDebug Replace with own MessageBox
		//wxMessageBox( ss.str(), "Info");
		INVISION_LOG_ERROR(ss.str());
		return false;
	}

	return app.exec();
}

