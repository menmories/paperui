#ifndef __PAPER_DEFINE_H__
#define __PAPER_DEFINE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
#include <tchar.h>
#endif

#include <stdint.h>
#include <assert.h>

#ifdef _WIN64
	typedef int32_t		int32;
	typedef int64_t		int64;
	typedef int16_t		int16;
	typedef char		int8;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;
	typedef uint16_t	uint16;
	typedef uint8_t		uint8;
	typedef uint64		uint_ptr;
	typedef int64		int_ptr;
#else
	typedef int32_t		int32;
	typedef int64_t		int64;
	typedef int16_t		int16;
	typedef char		int8;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;
	typedef uint16_t	uint16;
	typedef uint8_t		uint8;
	typedef uint32		uint_ptr;
	typedef int32		int_ptr;
#endif

#ifdef PACKAGE_DLL
#define PAPER_API __declspec(dllexport)
#else
#define PAPER_API
#endif
/*
#ifdef PAPER_PLATFORM_WINDOWS
#define PAPER_CALL __stdcall

#elif defined PAPER_PLATFORM_LINUX
#define PAPER_CALL __attribute__((__stdcall__))

#else
#define PAPER_CALL
#endif*/

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_DEFINE_H__