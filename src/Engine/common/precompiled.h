
#ifdef INVISION_EXPORTS
#define INVISION_API __declspec(dllexport) 
#else
#define INVISION_API __declspec(dllimport) 
#endif

// is needed for define WIN_32
#include "BaseTypes.h"
#include "exception.h"

/* standard library */
#include <iostream>
#include <time.h>
#include <map>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#define NOMINMAX
#include <algorithm>

// @TODO DEPRECATED -> REMOVE AND REWORK String classes
//// boost shared library
//#ifdef WIN_32
//#pragma warning(disable: 4800) // disable warning 4800: 'unsigned int':forcing value to bool 'true' or 'false' (performance warning)
//#endif
//#include <boost\crc.hpp>
//#ifdef WIN_32
//#pragma warning(default: 4800)
//#endif
//#include <boost\shared_array.hpp>



// windows platform libraries
#ifdef WIN_32
#include <Windows.h>
#include <VersionHelpers.h>
#include <direct.h>
#endif

#include "log.h"

