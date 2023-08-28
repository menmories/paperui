#ifndef __PAPER_DEFINE_H__
#define __PAPER_DEFINE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
#include <tchar.h>
#endif

#ifdef _WIN64
	typedef int32_t		int32;
	typedef int64_t		int64;
	typedef int16_t		int16;
	typedef int8_t		int8;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;
	typedef uint16_t	uint16;
	typedef uint8_t		uint8;
	typedef uint64		uint_ptr;
#else
	typedef int32_t		int32;
	typedef int64_t		int64;
	typedef int16_t		int16;
	typedef int8_t		int8;
	typedef uint32_t	uint32;
	typedef uint64_t	uint64;
	typedef uint16_t	uint16;
	typedef uint8_t		uint8;
	typedef uint32		uint_ptr;
#endif

#ifdef PACKAGE_DLL
#define PAPER_API __declspec(dllexport)
#else
#define PAPER_API
#endif



#ifdef __cplusplus
}
#endif

#endif	//__PAPER_DEFINE_H__