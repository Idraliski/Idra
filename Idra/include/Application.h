#pragma once

#ifdef _WIN32
	#ifdef IDRA_BUILD_DLL
		#define IDRA_API __declspec(dllexport)
	#else
		#define IDRA_API __declspec(dllimport)
	#endif
#else
	#error Idra only supports Windows!
#endif //_WIN32

class Application
{
	public:
	Application();
	~Application();
	void Run();
};
