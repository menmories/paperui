#include "paper_string.h"
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <Windows.h>
struct paper_string* paper_string_new()
{
    struct paper_string* str = malloc(sizeof(struct paper_string));
    if (str)
    {
        memset(str, 0, sizeof(struct paper_string));
        str->data = malloc(sizeof(wchar_t));
        if (str->data)
        {
            *str->data = PTEXT('\0');
        }
    }
    return str;
}

void paper_string_assign(struct paper_string* str, const wchar_t* src, uint32 size)
{
	assert(str);
	if (str->data)
	{
		free(str->data);
	}
	str->data = malloc(size + 1);
	memcpy(str->data, src, size);
	str->data[size] = 0;
	str->len = size;
}

void paper_string_from_utf8(struct paper_string* str, const wchar_t* src)
{
    int len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	char* data = malloc(len + 1);
	memset(data, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, src, (int)wcslen(src), data, len, NULL, NULL);


	int mlen = MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0);
	wchar_t* mData = malloc(sizeof(wchar_t) * mlen + 2);
	wmemset(mData, 0, mlen + 1);
	MultiByteToWideChar(CP_ACP, 0, data, len, mData, mlen);
	if (str->data)
	{
		free(str->data);
	}
	str->data = mData;
	str->len = mlen;
	free(data);
}

void paper_string_to_utf8(struct paper_string* str)
{

}

void paper_string_copy(struct paper_string* str, const wchar_t* src)
{
	
}

void paper_string_append(struct paper_string* str, const wchar_t* src)
{

}

void paper_string_substr(struct paper_string* str, uint32 beginPos, uint32 len)
{

}

uint32 paper_string_find(struct paper_string* str, const wchar_t* src)
{
	return 0;
}

uint32 paper_string_rfind(struct paper_string* str, const wchar_t* src)
{
	return 0;
}

void paper_string_make_upper(struct paper_string* str)
{
    wchar_t* ch = str->data;
	while (*ch != '\0')
	{
		if (*ch >= 65 && *ch <= 90)
		{
            *ch += 32;
		}
        ch++;
	}
}

void paper_string_make_lower(struct paper_string* str)
{
	wchar_t* ch = str->data;
	while (*ch != '\0')
	{
		if (*ch >= 97 && *ch <= 122)
		{
			*ch -= 32;
		}
		ch++;
	}
}

void paper_string_free(struct paper_string** str)
{
    assert(str);
    free((*str)->data);
    free(*str);
    *str = NULL;
}
