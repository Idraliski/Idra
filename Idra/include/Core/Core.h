#ifdef _WIN32
	#ifdef IDRA_CORE_EXPORT
		#define IDRA_API __declspec(dllexport)
	#else
		#define IDRA_API __declspec(dllimport)
	#endif
#else
	#error Only Windows is supported!
#endif

