#ifndef CPUID_H
#define CPUID_H
namespace Invision
{
#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
	typedef unsigned __int32  uint32_t;

#else
#include <stdint.h>
#endif

	class INVISION_API CPUID {
		uint32_t regs[4];

	public:
		explicit CPUID(unsigned i);

		const uint32_t &EAX() const;
		const uint32_t &EBX() const;
		const uint32_t &ECX() const;
		const uint32_t &EDX() const;
	};

}
#endif // CPUID_H