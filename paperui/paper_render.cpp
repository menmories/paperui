#include "paper_render.h"




#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>


//#define STBI_NO_STDIO
//#define STBI_NO_WRITE
//#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
//#include "stb_image_resize.h"
//#include "stb_image_write.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
//#pragma comment(lib, "windowscodecs.lib")

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

struct paper_render
{
	ID2D1RenderTarget* renderTarget;
};

struct paper_image
{
	ID2D1Bitmap* bitmap;
};

typedef void* (*AllocMemoryCb)(size_t size);
typedef void (*FreeMemoryCb)(void* addr);


struct paper_allocator
{
	AllocMemoryCb alloc;
	FreeMemoryCb free;
};

static AllocMemoryCb AllocMemory = malloc;
static FreeMemoryCb  FreeMemory = free;

static ID2D1Factory* Direct2DFactory = nullptr;
static IDWriteFactory* DWriteFactory = nullptr;

int paper_render_initenv(void)
{
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (FAILED(CoInitialize(NULL)))
	{
		return -1;
	}
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &Direct2DFactory);
	if (FAILED(hr))
	{
		return -1;
	}
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(DWriteFactory),
		reinterpret_cast<IUnknown**>(&DWriteFactory)
	);
	if (FAILED(hr))
	{
		SafeRelease(&Direct2DFactory);
		return -1;
	}
	return 0;
}

void paper_render_destroyenv(void)
{
	SafeRelease(&DWriteFactory);
	SafeRelease(&Direct2DFactory);
}

struct paper_render* paper_render_create(void* wnd, uint32 width, uint32 height)
{
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	HRESULT hr = Direct2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties((HWND)wnd, D2D1::SizeU(width, height)),
		&renderTarget
	);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_render* render = (struct paper_render*)AllocMemory(sizeof(struct paper_render));
	if (!render)		//menmory alloc exception.
	{
		SafeRelease(&renderTarget);
		return nullptr;
	}
	render->renderTarget = renderTarget;
	return render;
}

void paper_render_resize(struct paper_render* render, uint32 width, uint32 height)
{
	ID2D1HwndRenderTarget* renderTarget = (ID2D1HwndRenderTarget*)render->renderTarget;
	renderTarget->Resize(D2D1::SizeU(width, height));
}

void paper_render_begin_draw(struct paper_render* render)
{
	render->renderTarget->BeginDraw();
}

void paper_render_end_draw(struct paper_render* render)
{
	render->renderTarget->EndDraw();
}

void paper_render_clear(struct paper_render* render, struct paper_color* color)
{
	render->renderTarget->Clear(D2D1::ColorF(color->r, color->g, color->b, color->a));
}

void paper_render_draw_image(struct paper_render* render, struct paper_image* image, int32 x, int32 y, uint32 width, uint32 height)
{
	// 指定要绘制的图片的目标区域大小
	D2D1_RECT_F destRect = D2D1::RectF((float)x, (float)y, (float)(x + width), (float)(y + height));
	render->renderTarget->DrawBitmap(image->bitmap, destRect);
}

struct paper_render* paper_render_create_compatible(struct paper_render* render, uint32 width, uint32 height)
{
	ID2D1BitmapRenderTarget* bitmapRenderTarget = nullptr;
	HRESULT hr = render->renderTarget->CreateCompatibleRenderTarget(&bitmapRenderTarget);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_render* compatible_render = (struct paper_render*)AllocMemory(sizeof(struct paper_render));
	if (!compatible_render)
	{
		SafeRelease(&bitmapRenderTarget);
		return nullptr;
	}
	compatible_render->renderTarget = bitmapRenderTarget;
	return render;
}

struct paper_image* paper_image_load_from_file(struct paper_render* render, const char* filename)
{
	assert(render);
	assert(filename);
	FILE* pFile = fopen(filename, "rb+");
	if (!pFile)
	{
		return nullptr;
	}
	unsigned char* data = nullptr;
	int width = 0;
	int height = 0;
	int comp = 0;
	stbi_uc* stbi_image = stbi_load_from_file(pFile, &width, &height, &comp, STBI_rgb_alpha);
	if (!stbi_image)
	{
		return nullptr;
	}
	struct paper_image* image = nullptr;
	image = (struct paper_image*)AllocMemory(sizeof(struct paper_image));
	if (!image)
	{
		free(stbi_image);
		return nullptr;
	}
	D2D1_SIZE_U size;
	size.height = height;
	size.width = width;
	// 创建Direct2D位图
	D2D1_BITMAP_PROPERTIES properties = D2D1::BitmapProperties(
		D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = render->renderTarget->CreateBitmap(size, stbi_image, width * 4, properties, &image->bitmap);
	if (FAILED(hr))
	{
		paper_image_free(image);
		return nullptr;
	}
	return image;
}

void paper_image_free(struct paper_image* image)
{
	SafeRelease(&image->bitmap);
	FreeMemory(image);
}
