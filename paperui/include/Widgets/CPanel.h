#ifndef PAPERUI_CPANEL_H
#define PAPERUI_CPANEL_H

#include "platformdef.h"
#include "CWidget.h"

class PAPERUI_API CPanel : public CWidget
{
public:
	CPanel(CWidget* parent = nullptr);
    virtual ~CPanel();

protected:
    virtual void OnPaint(CPainter& painter);
    virtual void NcPaint(CPainter& painter);
};


#endif  //PAPERUI_CPANEL_H

