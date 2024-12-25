#include "private/include/CGlobalResource.h"

CGlobalResource CGlobalResource::__instance;

CGlobalResource::CGlobalResource()
{
}

CGlobalResource::~CGlobalResource()
{
	m_windowPainterContext.clear();
}

