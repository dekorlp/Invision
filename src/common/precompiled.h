
#ifdef INVISION_EXPORTS
#define INVISION_API __declspec(dllexport) 
#else
#define INVISION_API __declspec(dllimport) 
#endif

// is needed for define WIN_32
#include "BaseTypes.h"
#include "exception.h"

/* third party libs */
#include <time.h>
#include <thread>
#include <sstream>
#include <vector>
#include <boost\crc.hpp>
#include <boost\shared_array.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>

// windows platform
#ifdef WIN_32
#include <Windows.h>
#include <VersionHelpers.h>
#include <direct.h>
#endif

#include "log.h"
