#ifndef PAPERUI_PLATFORMDEF_H
#define PAPERUI_PLATFORMDEF_H

#include <Windows.h>
#include <windowsx.h>

//去除Windows平台一些不需要的定义
#ifdef IsMaximized
#undef IsMaximized
#endif

#ifdef IsMinimized
#undef IsMinimized
#endif



#ifdef PAPERUI_EXPORTS
#define PAPERUI_API __declspec(dllexport)
#else
#define PAPERUI_API __declspec(dllimport)
#endif


#endif  // PAPERUI_PLATFORMDEF_H