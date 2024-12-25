#ifndef PAPERUI_CBRUSH_H
#define PAPERUI_CBRUSH_H

#include "platformdef.h"
#include <d2d1.h>
#include "CColor.h"

enum EBrushType
{
    Brush_Solid,
    Brush_Linear,
};

class PAPERUI_API CBrush
{
public:
    CBrush();
    CBrush(const CColor& color);
    ~CBrush();

    inline EBrushType GetType() const
    {
        return m_type;
    }

    inline CColor GetColor()const
    {
        return m_color;
    }

    inline ID2D1SolidColorBrush* GetBrush() const
    {
        return m_brush;
    }
private:
    EBrushType m_type;
    CColor m_color;
    ID2D1SolidColorBrush* m_brush;
};


#endif  // PAPERUI_CBRUSH_H

