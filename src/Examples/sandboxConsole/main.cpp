#include "InCommon.h"
#include "InMath.h"

#include "input/WindowsKeyboard.h"
#include "input/IKeyboard.h"

//#include <iostream>
//#include <string>
//#include "math\Vector3.h"
//#include "math\Matrix.h"
//#include "mathnew/Vector3.h"


using namespace std;

void testCPUInfo()
{
	/*Invision::PlatformCPU platformCpu;
	Invision::PlatformCPUFrequency cpufreq;

	const char* name = platformCpu.GetCpuName();
	std::cout << "Name: " << name << std::endl;
	std::cout << "Count V-Cores: " << platformCpu.GetLogicalCpuCores() << std::endl;
	std::cout << "Frequency: " << cpufreq.EstimateCpuSpeed() << std::endl;*/
}

void testVector(){

	Invision::Vector4 vec4();
	
	Invision::Vector3 vektor1(1.0f, 4.0f, 2.0f);

	Invision::Vector3 vec1(1, 4, 2);
	//Vector3 vec2(4, 3, 1);
	Invision::Vector3 vec2(2, 4, 2);
	//Vector3 res = vec1 + vec2;
	//Vector3 res = vec2 - vec1;
	Invision::Vector3 res = vec2 + vec1;
	bool b = vec1 == vec2;
}

/*void testLog()
{
	Invision::Log log("../../../logs/log.txt");
	log.RawText(std::string("HalloWelt").append("RAW"));
	log.Info(std::string("HalloWelt").append("INFO"));
	log.Error(std::string("HalloWelt").append("ERROR"));
	log.Warning(std::string("HalloWelt").append("WARNING"));
	//Log::m_pThis = &log;
	Invision::Log::SetLogger(&log);
	INVISION_LOG_INFO(std::string("HalloWelt").append("INFO"));
}*/

/*void testAllocatorBlockStack()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Invision::Log log("../../../logs/AllocationLog.txt");
	Invision::Log::SetLogger(&log);
	
	void* top;
	
	int* test6 = (int*)Invision::MemoryBlock::CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;
	Invision::SHeaderStack *header = Invision::MemoryBlock::GetStackHeader(test6);
	Invision::SMemoryTracking* tracking = Invision::MemoryBlock::GetTrackingHeader(test6, INVISION_USE_STACKHEADER);
	bool isBoundaries = Invision::MemoryBlock::CheckBoundaries(test6, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	size_t size = Invision::MemoryBlock::CalculateSize(arena, sizeof(int), INVISION_USE_STACKHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	// Ausgabe Size
	stringstream ss1;
	ss1 << "calculateSize(): " << size << std::endl;
	INVISION_LOG_RAWTEXT(ss1.str());

	// Ausgabe Top
	stringstream ss2;
	ss2 << "Top: " << top << std::endl;
	INVISION_LOG_RAWTEXT(ss2.str());

}*/

/*void testAllocatorBlockPool()
{
	//1024 * 1024
	//void* arena = ::operator new (1);
	uint32 length = 1024;
	void* arena = malloc(length);
	Invision::Log log("../../../logs/AllocationLog.txt");
	Invision::Log::SetLogger(&log);

	void* top;

	int* test6 = (int*)Invision::MemoryBlock::CreateMemoryBlock(arena, &top, sizeof(int), __LINE__, __FILE__, INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);
	*test6 = 0x2BEC;

	INVISION_LOG_RAWTEXT("----------Before SetPoolHeader()--------");
	Invision::SHeaderPool *header1 = Invision::MemoryBlock::GetPoolHeader(test6);
	//memBlock.SetPoolHeader(test6, (void*)0xFFFFFFFF);
	INVISION_LOG_RAWTEXT("----------After SetPoolHeader()-------");
	Invision::SHeaderPool *header2 = Invision::MemoryBlock::GetPoolHeader(test6);
	Invision::SMemoryTracking* tracking = Invision::MemoryBlock::GetTrackingHeader(test6, INVISION_USE_POOLHEADER);
	bool isBoundaries = Invision::MemoryBlock::CheckBoundaries(test6, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING);
	size_t size = Invision::MemoryBlock::CalculateSize(arena, sizeof(int), INVISION_USE_POOLHEADER, INVISION_ADVANCED_MEMORY_TRACKING, INVISION_STANDARD_BOUNDS_CHECKING);

	// Ausgabe Size
	stringstream ss1;
	ss1 << "calculateSize(): " << size << std::endl;
	INVISION_LOG_RAWTEXT(ss1.str());

	// Ausgabe Top
	stringstream ss;
	ss << "Top: " << top << std::endl;
	INVISION_LOG_RAWTEXT(ss.str());
}*/

/*void testLinearAllocator()
{
	Invision::Log log("../../../logs/LinearAllocationLog.txt");
	Invision::Log::SetLogger(&log);


	Invision::LinearAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA1 = 44;
	uint32* LinA2 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA2 = 125;
	uint32* LinA3 = (uint32*) alloc.Allocate(sizeof(int));
	*LinA3 = 128;
	alloc.Clear();
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA4 = 1028;

	alloc.Destroy();
}

void testStackAllocator()
{
	Invision::Log log("../../../logs/StackAllocationLog.txt");
	Invision::Log::SetLogger(&log);

	Invision::StackAllocator alloc;
	alloc.Init(1024);
	uint32* LinA1 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA1 = 44;
	uint32* LinA2 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA2 = 125;
	uint32* LinA3 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA3 = 128;
	alloc.Deallocate(LinA2);
	uint32* LinA4 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA4 = 1028;
	uint32* LinA5 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA5 = 2048;
	alloc.Clear();
	uint32* LinA6 = (uint32*)alloc.Allocate(sizeof(int));
	*LinA6 = 4096;

	alloc.Destroy();
}

void testPoolAllocator()
{
	Invision::Log log("../../../logs/PoolAllocationLog.txt");
	Invision::Log::SetLogger(&log);

	int test1 = 1024;

	Invision::PoolAllocator alloc;
	alloc.Init(192, sizeof(int));
	uint32* Pol1 = (uint32*) alloc.Allocate();
	*Pol1 = test1;
	uint32* Pol2 = (uint32*)alloc.Allocate();
	*Pol2 = 2048;
	uint32* Pol3 = (uint32*)alloc.Allocate();
	*Pol3 = 3000;
	uint32* Pol4 = (uint32*)alloc.Allocate();
	*Pol4 = 4000;
	uint32* Pol5 = (uint32*)alloc.Allocate();
	*Pol5 = 5000;
	uint32* Pol6 = (uint32*)alloc.Allocate();
	*Pol6 = 7000;
	alloc.Deallocate(Pol2);
	alloc.Deallocate(Pol3);
	uint32* Pol7 = (uint32*)alloc.Allocate();
	*Pol7 = 8000;
	uint32* Pol8 = (uint32*)alloc.Allocate();
	*Pol8 = 9000;

	alloc.Destroy();
}

struct Person
{
	float Gewicht;
	int Alter;
	std::string name;
	char* address;
	Person* ptr;
};

void testPoolAllocatorStruct()
{
	Invision::Log log("../../../logs/PoolAllocationLog.txt");
	Invision::Log::SetLogger(&log);

	

	std::string Addresse = "Main Road";
	std::string Addresse1 = "Second Road";
	std::string Addresse2 = "Third Road";
	std::string Addresse3 = "Fourth Road";
	std::string Addresse4 = "Fifth Road";
	std::string Addresse5 = "Kettelerstraße";

	Person PersSave ;
	PersSave.address = (char*)Addresse2.c_str();
	PersSave.Alter = 23;
	PersSave.Gewicht = 68;
	PersSave.name = "Marie";
	PersSave.ptr = nullptr;

	Person Pers1;
	Pers1.address = (char*)Addresse.c_str();
	Pers1.Alter = 22;
	Pers1.Gewicht = 180;
	Pers1.name = "Hans";
	Pers1.ptr = &PersSave;

	PersSave.ptr = &Pers1;

	Person Pers2;
	Pers2.address = (char*)Addresse1.c_str();
	Pers2.Alter = 45;
	Pers2.Gewicht = 185;
	Pers2.name = "Dieter";
	Pers2.ptr = nullptr;

	Person Pers3;
	Pers3.address = (char*)Addresse2.c_str();
	Pers3.Alter = 58;
	Pers3.Gewicht = 200;
	Pers3.name = "Werner";
	Pers3.ptr = nullptr;

	Person Pers4;
	Pers4.address = (char*)Addresse2.c_str();
	Pers4.Alter = 16;
	Pers4.Gewicht = 48;
	Pers4.name = "Sven";
	Pers4.ptr = nullptr;

	Person Pers5;
	Pers5.address = (char*)Addresse3.c_str();
	Pers5.Alter = 13;
	Pers5.Gewicht = 35;
	Pers5.name = "Peter";
	Pers5.ptr = nullptr;

	Person Pers6;
	Pers6.address = (char*)Addresse4.c_str();
	Pers6.Alter = 68;
	Pers6.Gewicht = 94;
	Pers6.name = "Gundula";
	Pers6.ptr = nullptr;

	Person Pers7;
	Pers7.address = (char*)Addresse5.c_str();
	Pers7.Alter = 27;
	Pers7.Gewicht = 52;
	Pers7.name = "Dennis";
	Pers7.ptr = nullptr;

	Person Pers8;
	Pers8.address = (char*)Addresse.c_str();
	Pers8.Alter = 25;
	Pers8.Gewicht = 92;
	Pers8.name = "Thorsten";
	Pers8.ptr = nullptr;
	


	Invision::PoolAllocator alloc;
	alloc.Init(6 * (sizeof(Person) + alloc.GetLayoutSize()), sizeof(Person));
	//alloc.InitBlocks(6 , sizeof(Person));
	
	Person** Pol1 = (Person**)alloc.Allocate();
	*Pol1 = &Pers1;
	Person** Pol2 = (Person**)alloc.Allocate();
	*Pol2 = &Pers2;
	Person** Pol3 = (Person**)alloc.Allocate();
	*Pol3 = &Pers3;
	Person** Pol4 = (Person**)alloc.Allocate();
	*Pol4 = &Pers4;
	Person** Pol5 = (Person**)alloc.Allocate();
	*Pol5 = &Pers5;
	Person** Pol6 = (Person**)alloc.Allocate();
	*Pol6 = &Pers6;
	alloc.Deallocate(Pol2);
	alloc.Deallocate(Pol3);
	Person** Pol7 = (Person**)alloc.Allocate();
	*Pol7 = &Pers7;
	Person** Pol8 = (Person**)alloc.Allocate();
	*Pol8 = &Pers8;

	(*Pol1)->ptr = &Pers6;

	alloc.Destroy();
}*/

/*typedef void* HANDLE;

class VulkanAPI
{
	private:
		std::string name = "Vulklan API";
	public:
		std::string getName()
		{
			return name;
		}

};

class DirectXAPI
{
private:
	std::string name = "DirectX API";
public:
	std::string getName()
	{
		return name;
	}

};*/

/*enum GRAPHIC_API
{
	VULKAN = 0,
	DIRECTX = 1
};

class APIConfig
{
public:
	VulkanAPI* apiVulkan = nullptr;
	DirectXAPI* apiDirectx = nullptr;
	GRAPHIC_API api;

	std::string GetApi()
	{
		if (api == VULKAN)
		{
			return apiVulkan->getName();
		}
		else if (api == DIRECTX)
		{
			return apiDirectx->getName();
		}
		else
		{
			return "";
		}
	}
};*/

/*class InvisionRenderer
{
	private:
		APIConfig* config;
		
public:
	InvisionRenderer(GRAPHIC_API api, HANDLE& handle)
	{
		config = new APIConfig();
		if (api == VULKAN)
		{
			config->apiVulkan = new VulkanAPI();
			config->api = VULKAN;
			handle = (HANDLE)(config);
		}
		else if (api == DIRECTX)
		{
			config->apiDirectx = new DirectXAPI();
			config->api = DIRECTX;
			DirectXAPI* ap = config->apiDirectx;
			handle = (HANDLE)(config);
		}
		else
		{
			throw "undefined Handle";
		}
	}
	~InvisionRenderer()
	{
		if (config->api == VULKAN)
		{
			delete config->apiVulkan;
		}
		else if (config->api == DIRECTX)
		{
			delete config->apiDirectx;
		}

		delete config;
	 }

};

class InvisionCommandBuffer
{
	public:
		void printSelectedAPI(void* handle)
		{
			std::cout << ((APIConfig*)handle)->GetApi() << std::endl;
		}
};

void testCast()
{

	
	HANDLE config;

	InvisionRenderer *dx = new InvisionRenderer(DIRECTX, config);
	InvisionCommandBuffer commandBuffer;
	commandBuffer.printSelectedAPI(config);
	dx->~InvisionRenderer();
	int test = 0;
}*/

void testKeyboardHandling()
{
	std::shared_ptr<Invision::IKeyboard> winKeyboard = std::make_shared<Invision::WindowsKeyboard>();

	while (true)
	{
		if (winKeyboard->GetStateOfKey(Invision::INVISION_KEY_CTRL, Invision::INVISION_KEY_PRESSED) && 
			winKeyboard->GetStateOfKey(Invision::INVISION_KEY_SHIFT, Invision::INVISION_KEY_PRESSED)&& 
			winKeyboard->GetStateOfKey(Invision::INVISION_KEY_F, Invision::INVISION_KEY_PRESSED))
		{
			std::cout << "F Key is pressed!" << std::endl;
		}

	}
}

struct container
{
	Invision::Matrix view;
	Invision::Matrix proj;

} container;

void testMatrix()
{
	Invision::Matrix testMat1(1, 2, 3, 4,
		                  5, 6, 7, 8,
		                  9, 10, 11, 12,
	                      13, 14, 15, 16);

	Invision::Matrix testMat2(17, 18, 19, 20,
		21, 22, 23, 24,
		25, 26, 27, 28,
		29, 30, 31, 32);

	Invision::Vector4 testVec1(8.0, 4.0, 2.0, 1.0);

	Invision::Matrix resMat =   testMat1 * testMat2;

	Invision::Vector4 resVec = testMat1 * testVec1;


	float det = Invision::Matrix(5.0f, 3, -1, 1, 0, 0, 2, 0, 3, 0, 4, 7, -1, 4, -2, 5).GetDeterminant();

	Invision::Matrix trianglulated = Invision::Matrix(5.0f, 3, -1, 1, 0, 0, 2, 0, 3, 0, 4, 7, -1, 4, -2, 5).Triangulate();

	Invision::Matrix transp = testMat2.GetTranspose();

	container.view = Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));

	container.proj = Invision::Matrix::Perspective(45.0, 1920 / 1080, 0.1f, 10.0f);

	Invision::Matrix coVectors = Invision::Matrix(5.0f, 3, -1, 1, 0, 0, 2, 0, 3, 0, 4, 7, -1, 4, -2, 5).GetCoFactor();
	//Invision::Matrix coVectors = Invision::Matrix(5.0f, 0, 3, -1, 3, 0, 0, 4, -1, 2, 4, -2, 1, 0, 7, 5).GetCoFactor();

	Invision::Matrix invertexMatrix = Invision::Matrix(5.0f, 3, -1, 1, 0, 0, 2, 0, 3, 0, 4, 7, -1, 4, -2, 5).Invert();

	Invision::Matrix trianglulated2 = Invision::Matrix(1.0f, 1, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 1, 2, 0, 0).Triangulate();

	int test100 = 0;
}

int main()
{
	//CPUID cpuid(0x80000004);

	//std::string vendor;
	//vendor += std::string((const char *)&cpuid.EBX(), 4);
	//vendor += std::string((const char *)&cpuid.EDX(), 4);
	//vendor += std::string((const char *)&cpuid.ECX(), 4);

	//cout << "CPU vendor = " << vendor << endl;


	//testVector();
	//testAllocators();
	//testLog();

	//testAllocatorBlockStack();
	//testAllocatorBlockPool();
	//testCPUInfo();
	//testLinearAllocator();
	//testStackAllocator();
	//testPoolAllocator();
	//testPoolAllocatorStruct();
	//testVector();
	//testMatrix();
	//testCast();
	//testKeyboardHandling();
	testMatrix();
	
	return 0;
}