#ifndef PAPERUI_CCOLOR_H
#define PAPERUI_CCOLOR_H

#include "platformdef.h"

class PAPERUI_API CColor
{
public:
    CColor();
    CColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    ~CColor();

    inline void SetColor(const CColor& color)
    {
        m_r = color.m_r;
        m_g = color.m_g;
        m_b = color.m_b;
        m_a = color.m_a;
    }
    inline void SetColor(int r, int g, int b, int a)
    {
        m_r = r;
        m_g = g;
        m_b = b;
        m_a = a;
    }

    inline void SetColorF(float r, float g, float b, float a)
    {
        m_r = r * 255;
        m_g = g * 255;
        m_b = b * 255;
        m_a = a * 255;
    }

    inline void SetR(unsigned char r)
    {
        m_r = r;
    }

    inline void SetG(unsigned char g)
    {
        m_g = g;
    }

    inline void SetB(unsigned char b)
    {
        m_b = b;
    }

    inline void SetA(unsigned char a)
    {
        m_a = a;
    }

    inline void SetColor(unsigned int color)
    {
        m_r = (color & 0xff000000) >> 24;
        m_g = (color & 0x00ff0000) >> 16;
        m_b = (color & 0x0000ff00) >> 8;
        m_a = (color & 0x000000ff);
    }

    inline unsigned char R()const
    {
        return m_r;
    }

    inline unsigned char G()const
    {
        return m_g;
    }

    inline unsigned char B()const
    {
        return m_b;
    }

    inline unsigned char A()const
    {
        return m_a;
    }

    inline unsigned int Color()const
    {
        return (m_r << 24) | (m_g << 16) | (m_b << 8) | m_a;
    }

    inline float RF()const
    {
        return m_r / 255.0f;
    }

    inline float GF()const
    {
        return m_g / 255.0f;
    }

    inline float BF()const
    {
        return m_b / 255.0f;
    }

    inline float AF()const
    {
        return m_a / 255.0f;
    }

    inline bool operator==(const CColor& color)const
    {
        return m_r == color.m_r && m_g == color.m_g && m_b == color.m_b && m_a == color.m_a;
    }

    inline bool operator!=(const CColor& color)const
    {
        return m_r != color.m_r || m_g != color.m_g || m_b != color.m_b || m_a != color.m_a;
    }

    inline CColor operator+(const CColor& color)const
    {
        return CColor(m_r + color.m_r, m_g + color.m_g, m_b + color.m_b, m_a + color.m_a);
    }

    inline CColor operator-(const CColor& color)const
    {
        return CColor(m_r - color.m_r, m_g - color.m_g, m_b - color.m_b, m_a - color.m_a);
    }

    inline CColor operator*(const CColor& color)const
    {
        return CColor(m_r * color.m_r, m_g * color.m_g, m_b * color.m_b, m_a * color.m_a);
    }

    inline CColor operator/(const CColor& color)const
    {
        return CColor(m_r / color.m_r, m_g / color.m_g, m_b / color.m_b, m_a / color.m_a);
    }

    inline CColor operator+(int value)const
    {
        return CColor(m_r + value, m_g + value, m_b + value, m_a + value);
    }

    inline CColor operator-(int value)const
    {
        return CColor(m_r - value, m_g - value, m_b - value, m_a - value);
    }

    inline CColor operator*(int value)const
    {
        return CColor(m_r * value, m_g * value, m_b * value, m_a * value);
    }

    inline CColor operator/(int value)const
    {
        return CColor(m_r / value, m_g / value, m_b / value, m_a / value);
    }

    inline CColor operator+(float value)const
    {
        return CColor(m_r + value, m_g + value, m_b + value, m_a + value);
    }

    inline CColor operator-(float value)const
    {
        return CColor(m_r - value, m_g - value, m_b - value, m_a - value);
    }

    inline CColor operator*(float value)const
    {
        return CColor(m_r * value, m_g * value, m_b * value, m_a * value);
    }

    inline CColor operator/(float value)const
    {
        return CColor(m_r / value, m_g / value, m_b / value, m_a / value);
    }

    inline CColor& operator+=(const CColor& color)
    {
        m_r += color.m_r;
        m_g += color.m_g;
        m_b += color.m_b;
        m_a += color.m_a;
        return *this;
    }

    inline CColor& operator-=(const CColor& color)
    {
        m_r -= color.m_r;
        m_g -= color.m_g;
        m_b -= color.m_b;
        m_a -= color.m_a;
        return *this;
    }

    inline CColor& operator*=(const CColor& color)
    {
        m_r *= color.m_r;
        m_g *= color.m_g;
        m_b *= color.m_b;
        m_a *= color.m_a;
        return *this;
    }

    inline CColor& operator/=(const CColor& color)
    {
        m_r /= color.m_r;
        m_g /= color.m_g;
        m_b /= color.m_b;
        m_a /= color.m_a;
        return *this;
    }

    inline CColor& operator+=(int value)
    {
        m_r += value;
        m_g += value;
        m_b += value;
        m_a += value;
        return *this;
    }

    inline CColor& operator-=(int value)
    {
        m_r -= value;
        m_g -= value;
        m_b -= value;
        m_a -= value;
        return *this;
    }

    inline CColor& operator*=(int value)
    {
        m_r *= value;
        m_g *= value;
        m_b *= value;
        m_a *= value;
        return *this;
    }

    inline CColor& operator/=(int value)
    {
        m_r /= value;
        m_g /= value;
        m_b /= value;
        m_a /= value;
        return *this;
    }

    inline CColor& operator+=(float value)
    {
        m_r += value;
        m_g += value;
        m_b += value;
        m_a += value;
        return *this;
    }

    inline CColor& operator-=(float value)
    {
        m_r -= value;
        m_g -= value;
        m_b -= value;
        m_a -= value;
        return *this;
    }

    inline CColor& operator*=(float value)
    {
        m_r *= value;
        m_g *= value;
        m_b *= value;
        m_a *= value;
        return *this;
    }

    inline CColor& operator/=(float value)
    {
        m_r /= value;
        m_g /= value;
        m_b /= value;
        m_a /= value;
        return *this;
    }

    static const unsigned int Black = 0x000000ff;
    static const unsigned int White = 0xffffffff;
private:
    unsigned char m_r;
    unsigned char m_g;
    unsigned char m_b;
    unsigned char m_a;
};



#endif  //PAPERUI_CCOLOR_H
