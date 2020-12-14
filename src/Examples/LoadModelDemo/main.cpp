
#include "MainWindow2.h"

#include "Engine\InCommon.h"

int main(int argc, char *argv[]) {
	

	//Invision::Vector4 test(2, 3, 4, 5);
	//Invision::Matrix mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	//Invision::Vector4 ergebnis =  mat * test;
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



