#include "Render/CBitmap.h"
#include "private/include/CGlobalResource.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "private/include/stb_image.h"

CBitmap::CBitmap()
    : m_width(0)
    , m_height(0)
    , m_comp(0)
    , m_data(nullptr)
{
}

CBitmap::~CBitmap()
{
}

bool CBitmap::Load(const char* fileName)
{
    assert(fileName);
    FILE* pFile = fopen(fileName, "rb+");
	if (!pFile)
	{
		return false;
	}
	int width = 0;
	int height = 0;
	int comp = 0;
	stbi_uc* stbi_image = stbi_load_from_file(pFile, &width, &height, &comp, STBI_rgb_alpha);
	fclose(pFile);
    if (!stbi_image)
    {
        return false;
    }
    ID2D1HwndRenderTarget* renderTarget = CGlobalResource::Get()->Pop()->RenderTarget();
	D2D1_BITMAP_PROPERTIES properties = D2D1::BitmapProperties(
		D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = renderTarget->CreateBitmap(D2D1::SizeU(width, height),
        stbi_image, width * 4, properties, &m_data);
    if (FAILED(hr))
    {
        return false;
    }
    m_width = width;
    m_height = height;
    m_comp = comp;
    return true;
}

