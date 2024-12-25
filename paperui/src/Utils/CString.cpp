#include "Utils/CString.h"
#include <wchar.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_BLOCK 8

CString::CString()
{
	m_data = new wchar_t[DEFAULT_BLOCK];
	wmemset(m_data, 0, DEFAULT_BLOCK);
	m_len = 0;
}

CString::CString(const char* data, int len)
{
	m_len = MultiByteToWideChar(CP_UTF8, 0, data, len, nullptr, 0);
	m_data = new wchar_t[m_len + 1];
	m_data[m_len] = 0;
	int ret = MultiByteToWideChar(CP_UTF8, 0, data, len, m_data, m_len);
}

CString::CString(const wchar_t* data, int len)
{
	if (len == -1) {
		len = wcslen(data);
	}
	Assign(data, len);
}

CString::~CString()
{
	delete[] m_data;
}

CString& CString::operator=(const CString& other)
{
	if (m_data != other.m_data)
	{
		delete[] m_data;
		m_len = other.m_len;
		m_data = new wchar_t[m_len + 1];
		m_data[m_len] = 0;
		wmemcpy(m_data, other.m_data, m_len);
	}
	return *this;
}

void CString::Assign(const wchar_t* data, int len)
{
	m_len = len;
	m_data = new wchar_t[m_len + 1];
	m_data[m_len] = 0;
	wmemcpy(m_data, data, m_len);
}


