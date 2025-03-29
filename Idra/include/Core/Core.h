#ifdef _WIN32
	#ifdef IDRA_CORE_EXPORT
		#define IDRA_API __declspec(dllexport)
	#else
		#define IDRA_API __declspec(dllimport)
	#endif
#else
	#error Only Windows is supported!
#endif

#ifdef IDRA_ENABLE_ASSERTS
	#define IDRA_ASSERT(x, ...) { if(!(x)) { IDRA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define IDRA_CORE_ASSERT(x, ...) { if(!(x)) { IDRA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define IDRA_ASSERT(x, ...)
	#define IDRA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
