#include "Window/CMainWindow.h"
#include "CApplication.h"
CMainWindow::CMainWindow()
{
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::OnDestroy()
{
	CWindow::OnDestroy();
	CApplication::Quit(0);
}

