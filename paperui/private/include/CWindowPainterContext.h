#ifndef PAPERUI_CWINDOWPAINTERCONTEXT_H
#define PAPERUI_CWINDOWPAINTERCONTEXT_H

#include "platformdef.h"
#include "CD2D1Context.h"
#include "Render/CPainter.h"
#include "Window/CWindow.h"
#include "Render/CBrush.h"
class CWindow;
class CWindowPainterContext
{
public:
    CWindowPainterContext(CWindow* window = nullptr);
    virtual ~CWindowPainterContext();

    void AttachWindow(CWindow* window);

    inline void Resize(int width, int height)
    {
        m_renderTarget->Resize(D2D1::SizeU(width, height));
    }

    CPainter* BeginPaint();

    void EndPaint();

    ID2D1HwndRenderTarget* RenderTarget() const
    {
        return m_renderTarget;
    }
private:
    CWindow* m_window;
    CColor m_clearColor;
    ID2D1HwndRenderTarget* m_renderTarget;
    CPainter* m_painter;
};

#endif  // PAPERUI_CWINDOWPAINTERCONTEXT_H