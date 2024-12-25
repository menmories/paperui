#include "private/include/CD2D1Context.h"

CD2D1Context CD2D1Context::__instance;

CD2D1Context::CD2D1Context()
{
	m_bInitOK = false;
	m_d2d1Factory = nullptr;
	m_dwriteFactory = nullptr;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_d2d1Factory);
	if (FAILED(hr))
	{
		return;
	}
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dwriteFactory),
		reinterpret_cast<IUnknown**>(&m_dwriteFactory)
	);
	if (FAILED(hr))
	{
		SafeRelease(&m_d2d1Factory);
		return;
	}
	m_bInitOK = true;
	/*float x = 0.0f;
	float y = 0.0f;
	m_d2d1Factory->GetDesktopDpi(&x, &y);*/
}

CD2D1Context::~CD2D1Context()
{
	SafeRelease(&m_dwriteFactory);
	SafeRelease(&m_d2d1Factory);
}

