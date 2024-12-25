#ifndef PAPERUI_CFONT_H
#define PAPERUI_CFONT_H

#include "platformdef.h"
#include <d2d1.h>
#include "Utils/CString.h"

class PAPERUI_API CFont
{
public:
    CFont();
    CFont(const CString& family, float size = 10);
    ~CFont();

    CString GetFontFamily()const;

    float GetFontSize()const;

    void Copy(const CFont& src);
private:
    void Init(const CString& family, float size);
private:
    CString m_fontFamily;
    IDWriteTextFormat* m_pTextFormat;
    friend class CPainter;
};





#endif  //PAPERUI_CFONT_H
