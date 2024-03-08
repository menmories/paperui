#ifndef __PAPER_STRING_H__
#define __PAPER_STRING_H__

#include "paper_define.h"


#define PTEXT(text) L##text

struct paper_string
{
	wchar_t* data;
	uint32 len;
};

#ifdef __cplusplus
extern "C"
{
#endif
	PAPER_API struct paper_string* paper_string_new();

	PAPER_API void paper_string_assign(struct paper_string* str, const wchar_t* src, uint32 size);

	PAPER_API void paper_string_from_utf8(struct paper_string* str, const wchar_t* src);

	PAPER_API void paper_string_to_utf8(struct paper_string* str);

	PAPER_API void paper_string_copy(struct paper_string* str, const wchar_t* src);

	PAPER_API void paper_string_append(struct paper_string* str, const wchar_t* src);

	PAPER_API void paper_string_substr(struct paper_string* str, uint32 beginPos, uint32 len);

	PAPER_API uint32 paper_string_find(struct paper_string* str, const wchar_t* src);

	PAPER_API uint32 paper_string_rfind(struct paper_string* str, const wchar_t* src);

	PAPER_API void paper_string_make_upper(struct paper_string* str);

	PAPER_API void paper_string_make_lower(struct paper_string* str);

	PAPER_API void paper_string_free(struct paper_string** str);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_STRING_H__
