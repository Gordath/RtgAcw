#ifndef GLACIER_DLL_EXPORT_H_
#define GLACIER_DLL_EXPORT_H_

#ifdef GLACIERENGINE_STATIC
#define GLACIERENGINE_API
#else
#if defined (__GNUC__) && defined(__unix__)
#define PRINT_API __attribute__ ((__visibility__("default")))
#elif defined (WIN32)
#ifdef GLACIERENGINE_EXPORTS
#define GLACIERENGINE_API __declspec(dllexport)
#else
#define GLACIERENGINE_API __declspec(dllimport)
#endif
#endif
#endif

#endif //GLACIER_DLL_EXPORT_H_