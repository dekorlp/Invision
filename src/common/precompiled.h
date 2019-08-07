
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

#ifdef WIN_32
#pragma warning(disable: 4800) // disable warning 4800: 'unsigned int':forcing value to bool 'true' or 'false' (performance warning)
#endif
#include <boost\crc.hpp>
#ifdef WIN_32
#pragma warning(default: 4800)
#endif

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
