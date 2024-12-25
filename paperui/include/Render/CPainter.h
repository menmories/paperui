#ifndef PAPERUI_CPAINTER_H
#define PAPERUI_CPAINTER_H

#include "platformdef.h"
#include <d2d1.h>
#include "CRect.h"
#include "CPoint.h"
#include "CColor.h"
#include "CBitmap.h"
#include "CBrush.h"
#include "CFont.h"
#include "Utils/CString.h"

#ifdef DrawText
#undef DrawText
#endif
class PAPERUI_API CPainter
{
public:
    CPainter();
    ~CPainter();

    inline void CPainter::BeginPaint()
    {
        m_renderTarget->BeginDraw();
    }

    inline void CPainter::EndPaint()
    {
        m_renderTarget->EndDraw();
    }
    void Clear(const CColor& color)
    {
        m_renderTarget->Clear(D2D1::ColorF(color.RF(),
            color.GF(),
            color.BF(),
            color.AF())
        );
    }

    CPainter CreateCompatiblePainter(int width, int height);

    CBitmap GetBitmap();

    void FillRect(const CRect& rect, const CColor& color);
    
    void DrawBitmap(const CBitmap& bitmap, const CRect& rect);

    void DrawRect(const CRect& rect, const CBrush& brush);

    void DrawText(const CRect& rect, const CString& text, const CFont& font, const CBrush& brush);

    inline void Destroy()
    {
        m_renderTarget->Release();
    }

    static bool InitEnvironment();
    static void DestroyEnvironment();
private:    
        
private:
    ID2D1BitmapRenderTarget* m_renderTarget;
    friend class CWindowPainterContext;
};




#endif  // PAPERUI_CPAINTER_H