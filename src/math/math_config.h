#pragma once



#if _MSC_VER >= 1910 && _MSC_VER <= 1916
#define SSE_INSTR_SET 7
#else
#define SSE_INSTR_SET 5
#endif

#ifdef _WIN64
#define USE_SSE2
#endif //_WIN64

#define PI 3.14159265358979323846264
#define PI_F 3.14159265358979f
