#include "CApplication.h"
#include <stdio.h>
#include "Render/CPainter.h"

CApplication::CApplication(int argc, char** argv)
{
	m_bInitOK = Initialize();
}

CApplication::~CApplication()
{
	Destroy();
}

void CApplication::SetDefaultFont(const CFont& font)
{
	m_defaultFont->Copy(font);
}

int CApplication::Run()
{
	MSG msg = { 0 };
	if (!m_bInitOK)
	{
		return -1;
	}
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void CApplication::Quit(int code)
{
	::PostQuitMessage(code);
}

void CApplication::Exit(int code)
{
	::exit(code);
}

bool CApplication::Initialize()
{
	if (FAILED(CoInitialize(NULL)))
	{
		fprintf(stderr, "CoInitialize failed!\n");
		return false;
	}
	if (!CPainter::InitEnvironment())
	{
		fprintf(stderr, "Init painter Environment failed!\n");
		CoUninitialize();
		return false;
	}
	m_defaultFont = new CFont("微软雅黑", 10);
	return true;
}

void CApplication::Destroy()
{
	if (m_bInitOK)
	{
		delete m_defaultFont;
		CPainter::DestroyEnvironment();
		CoUninitialize();
	}
}

