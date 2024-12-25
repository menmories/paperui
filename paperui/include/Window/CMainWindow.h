#ifndef PAPERUI_CMAINWINDOW_H
#define PAPERUI_CMAINWINDOW_H

#include "CWindow.h"

class PAPERUI_API CMainWindow : public CWindow
{
public:
	CMainWindow();
	virtual ~CMainWindow();

protected:
	virtual void OnDestroy();
};


#endif	//PAPERUI_CMAINWINDOW_H
