#ifndef PAPERUI_CD2D1CONTEXT_H
#define PAPERUI_CD2D1CONTEXT_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

class CD2D1Context
{
public:
	inline static CD2D1Context* Get()
	{
		return &__instance;
	}
	inline bool Status() const
	{
		return m_bInitOK;
	}

	inline ID2D1Factory* Factory()
	{
		return m_d2d1Factory;
	}

	inline IDWriteFactory* DWriteFactory()
	{
		return m_dwriteFactory;
	}

	void test()
	{
		/*ID2D1RenderTarget* renderTarget;
		m_d2d1Factory->GetDesktopDpi()*/
	}
private:
	CD2D1Context();
	~CD2D1Context();
private:
	ID2D1Factory* m_d2d1Factory;
	IDWriteFactory* m_dwriteFactory;
	//IDWriteRenderingParams* RenderParams;
	bool m_bInitOK;
	static CD2D1Context __instance;
};

#endif  //PAPERUI_CD2D1CONTEXT_H
