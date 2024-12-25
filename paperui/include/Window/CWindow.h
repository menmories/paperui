#ifndef PAPERUI_CWINDOW_H
#define PAPERUI_CWINDOW_H

#include "CWindowBase.h"
#include "Render/CPainter.h"
#include "Widgets/CWidget.h"

class PAPERUI_API CWindow : public CWindowBase
{
public:
    CWindow(CWindow* parent = nullptr);
    virtual ~CWindow();

    inline void SetRoot(CWidget* widget)
    {
        m_root = widget;
    }

    inline CWidget* GetRoot()
    {
        return m_root;
    }
protected:
    virtual LRESULT NativeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnPaint(CPainter& painter);
    virtual void OnMouseMove(int x, int y);
    virtual void OnResize(int width, int height);
    virtual void OnCreate();
    virtual void OnDestroy();
private:
    void __NcCreate();
    void __NcPaint(CPainter& painter);
private:
    class CWindowPainterContext* m_windwPainterContext;
    class CWidget* m_root;
    class CWindow* m_parent;
};




#endif  // PAPERUI_CWINDOW_H