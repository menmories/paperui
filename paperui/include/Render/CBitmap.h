#ifndef PAPERUI_CBITMAP_H
#define PAPERUI_CBITMAP_H

#include "platformdef.h"

#include <d2d1.h>

class PAPERUI_API CBitmap
{
public:
    CBitmap();
    ~CBitmap();

    bool Load(const char* fileName);

    inline ID2D1Bitmap* Data() const
    { 
        return m_data; 
    }

    inline int Width() const
    {
        return m_width;
    }
    inline int Height() const
    {
        return m_height;
    }

    inline int Comp() const
    {
        return m_comp;
    }

    inline void Destroy()
    {
        m_data->Release();
    }
private:
	int m_width;
	int m_height;
	int m_comp;
    ID2D1Bitmap* m_data;
    friend class CPainter;
};





#endif  // PAPERUI_CBITMAP_H