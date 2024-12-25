#ifndef PAPERUI_CSTRING_H
#define PAPERUI_CSTRING_H

#include "platformdef.h"

class PAPERUI_API CString
{
public:
	CString();
	CString(const char* data, int len = -1);
	CString(const wchar_t* data, int len = -1);
	~CString();

	CString& operator=(const CString& other);

	void Assign(const wchar_t* data, int len);

	inline int Length() const {
		return m_len;
	}

	inline wchar_t* Data() const {
		return m_data;
	}

	inline bool IsEmpty()const {
		return m_len;
	}

private:
	wchar_t* m_data;
	int m_len;
};




#endif
