#ifndef PAPERUI_CGLOBALRESOURCE_H
#define PAPERUI_CGLOBALRESOURCE_H

#include "platformdef.h"
#include "CWindowPainterContext.h"
#include <list>

class CGlobalResource
{
public:
	inline static CGlobalResource* Get()
	{
		return &__instance;
	}

	inline void CGlobalResource::Add(CWindowPainterContext* context)
	{
		m_windowPainterContext.push_back(context);
	}

	inline CWindowPainterContext* CGlobalResource::Pop()
	{
		return m_windowPainterContext.front();
	}

	inline int GetWindowPainterContextCount()const
	{
		return m_windowPainterContext.size();
	}
private:
	CGlobalResource();
	~CGlobalResource();


	std::list<CWindowPainterContext*> m_windowPainterContext;

	static CGlobalResource __instance;
};





#endif	//PAPERUI_CGLOBALRESOURCE_H
