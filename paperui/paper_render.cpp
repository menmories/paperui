#include "paper_render.h"

#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "paper_memorypool.h"

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
	void* winid;
};

struct paper_image
{
	ID2D1Bitmap* bitmap;
};

struct paper_font
{
	IDWriteTextFormat* textFormat;
};

struct paper_brush
{
	ID2D1Brush* brush;
};

typedef void* (*AllocMemoryCb)(size_t size);
typedef void (*FreeMemoryCb)(void* addr);

//
//struct paper_allocator
//{
//	AllocMemoryCb alloc;
//	FreeMemoryCb free;
//};
//
//static AllocMemoryCb AllocMemory = malloc;
//static FreeMemoryCb  FreeMemory = free;

static ID2D1Factory* Direct2DFactory = nullptr;
static IDWriteFactory* DWriteFactory = nullptr;
static paper_memorypool* render_pool = nullptr;
static paper_memorypool* image_pool = nullptr;
static paper_memorypool* font_pool = nullptr;
static paper_memorypool* brush_pool = nullptr;

int paper_render_initenv(void)
{
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (FAILED(CoInitialize(NULL)))
	{
		printf("CoInitialize failed!error:\n");
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
	render_pool = paper_memorypool_create(sizeof(struct paper_render));
	image_pool = paper_memorypool_create(sizeof(struct paper_image));
	font_pool = paper_memorypool_create(sizeof(struct paper_font));
	brush_pool = paper_memorypool_create(sizeof(struct paper_brush));
	return 0;
}

void paper_render_destroyenv(void)
{
	SafeRelease(&DWriteFactory);
	SafeRelease(&Direct2DFactory);
	paper_memorypool_free(render_pool);
	paper_memorypool_free(image_pool);
	paper_memorypool_free(font_pool);
	paper_memorypool_free(brush_pool);
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
	struct paper_render* render = (struct paper_render*)paper_memorypool_alloc(render_pool);
	if (!render)		//menmory alloc exception.
	{
		SafeRelease(&renderTarget);
		return nullptr;
	}
	render->renderTarget = renderTarget;
	render->winid = wnd;
	return render;
}

void paper_render_free(struct paper_render* render)
{
	render->renderTarget->Release();
	render->renderTarget = nullptr;
	paper_memorypool_dealloc(render_pool, render);
}

void paper_render_resize(struct paper_render* render, uint32 width, uint32 height)
{
	ID2D1HwndRenderTarget* renderTarget = (ID2D1HwndRenderTarget*)render->renderTarget;
	renderTarget->Resize(D2D1::SizeU(width, height));
}

void paper_render_invalid(struct paper_render* render)
{
	InvalidateRect((HWND)render->winid, nullptr, FALSE);
}

void paper_render_valid(struct paper_render* render)
{
	ValidateRect((HWND)render->winid, nullptr);
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

void paper_render_draw_image2(struct paper_render* render, struct paper_image* image, uint32 width, uint32 height)
{
	// 指定要绘制的图片的目标区域大小
	D2D1_RECT_F destRect = D2D1::RectF(0.0f, 0.0f, (float)(width), (float)(height));
	render->renderTarget->DrawBitmap(image->bitmap, destRect);
}

void paper_render_draw_image3(struct paper_render* render, struct paper_image* image)
{
	// 指定要绘制的图片的目标区域大小
	D2D1_SIZE_F size = render->renderTarget->GetSize();
	D2D1_RECT_F destRect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
	render->renderTarget->DrawBitmap(image->bitmap, destRect);
}

void paper_render_draw_text(struct paper_render* render, const TCHAR* szText, uint32 len, const struct paper_rect* rect, struct paper_font* font, struct paper_brush* brush)
{
	assert(render && szText && rect && font && brush);
	render->renderTarget->DrawText(szText,
		(UINT32)len,
		font->textFormat,
		D2D1::RectF((float)rect->left, (float)rect->top, (float)rect->right, (float)rect->bottom),
		brush->brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);
}

void paper_render_draw_line(struct paper_render* render, struct paper_point* start, struct paper_point* end, struct paper_brush* brush, float stroke, enum paper_line_type type)
{
	ID2D1StrokeStyle* pStrokeStyle = nullptr;
	if (type == paper_line_type::paper_line_solid)
	{
	}
	else if (type == paper_line_type::paper_line_dashed)
	{
		// Dash array for dashStyle D2D1_DASH_STYLE_CUSTOM
		float dashes[] = { 1.0f, 2.0f, 2.0f, 3.0f, 2.0f, 2.0f };

		// Stroke Style with Dash Style -- Custom
		HRESULT hr = Direct2DFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&pStrokeStyle
		);
		
	}
	render->renderTarget->DrawLine(D2D1::Point2F((float)start->x, (float)start->y), D2D1::Point2F((float)end->x, (float)end->y), brush->brush, stroke, pStrokeStyle);
}

void paper_render_draw_rectangle(struct paper_render* render, const struct paper_rect* rect, struct paper_brush* brush)
{
	D2D1_RECT_F rc = { (float)rect->left, (float)rect->top, (float)rect->right, (float)rect->bottom };
	render->renderTarget->DrawRectangle(rc, brush->brush);
}

void paper_render_fill_rectangle(struct paper_render* render, const struct paper_rect* rect, struct paper_brush* brush)
{
	//D2D1_RECT_F rc = { (float)rect->left, (float)rect->top, (float)(rect->right - rect->left), (float)(rect->bottom - rect->top) };
	assert(render);
	assert(brush);
	assert(rect);
	D2D1_RECT_F rc = { (float)rect->left, (float)rect->top, (float)rect->right, (float)rect->bottom };
	render->renderTarget->FillRectangle(rc, brush->brush);
}

void paper_render_draw_roundrectangle(struct paper_render* render, struct paper_rect* rect, struct paper_point* radius, struct paper_brush* brush, float stroke)
{
	D2D1_ROUNDED_RECT rc;
	rc.rect = { (float)rect->left, (float)rect->top, (float)(rect->right), (float)(rect->bottom) };
	rc.radiusX = (float)radius->x;
	rc.radiusY = (float)radius->y;
	render->renderTarget->DrawRoundedRectangle(rc, brush->brush);
}

void paper_render_fill_roundrectangle(struct paper_render* render, struct paper_rect* rect, struct paper_point* radius, struct paper_brush* brush)
{
	D2D1_ROUNDED_RECT rc;
	rc.rect = { (float)rect->left, (float)rect->top, (float)(rect->right), (float)(rect->bottom) };
	rc.radiusX = (float)radius->x;
	rc.radiusY = (float)radius->y;
	render->renderTarget->FillRoundedRectangle(rc, brush->brush);
}

void paper_render_draw_ellipse(struct paper_render* render, struct paper_point* center, float radius_x, float radius_y, struct paper_brush* brush, float stroke)
{
	render->renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F((float)center->x, (float)center->y), radius_x, radius_y), brush->brush, stroke);
}

PAPER_API void paper_render_fill_ellipse(struct paper_render* render, struct paper_point* center, float radius_x, float radius_y, struct paper_brush* brush)
{
	assert(render);
	render->renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F((float)center->x, (float)center->y), radius_x, radius_y), brush->brush);
}

struct paper_render* paper_render_create_compatible(struct paper_render* render, int32 width, int32 height)
{
	ID2D1BitmapRenderTarget* bitmapRenderTarget = nullptr;
	HRESULT hr = render->renderTarget->CreateCompatibleRenderTarget(D2D1::SizeF((float)width, (float)height), &bitmapRenderTarget);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_render* compatible_render = (struct paper_render*)paper_memorypool_alloc(render_pool);
	if (!compatible_render)
	{
		SafeRelease(&bitmapRenderTarget);
		return nullptr;
	}
	compatible_render->winid = render->winid;
	compatible_render->renderTarget = bitmapRenderTarget;
	return compatible_render;
}

PAPER_API struct paper_render* paper_render_create_compatible_extendsize(struct paper_render* render)
{
	ID2D1BitmapRenderTarget* bitmapRenderTarget = nullptr;
	HRESULT hr = render->renderTarget->CreateCompatibleRenderTarget(&bitmapRenderTarget);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_render* compatible_render = (struct paper_render*)paper_memorypool_alloc(render_pool);
	if (!compatible_render)
	{
		SafeRelease(&bitmapRenderTarget);
		return nullptr;
	}
	compatible_render->winid = render->winid;
	compatible_render->renderTarget = bitmapRenderTarget;
	return compatible_render;
}

struct paper_image* paper_image_get_from_render(struct paper_render* render)
{
	ID2D1BitmapRenderTarget* renderTarget = (ID2D1BitmapRenderTarget*)render->renderTarget;
	assert(renderTarget);
	struct paper_image* image = (struct paper_image*)paper_memorypool_alloc(image_pool);
	if (image)
	{
		ID2D1Bitmap* bitmap = nullptr;
		HRESULT hr = renderTarget->GetBitmap(&bitmap);
		if (!bitmap)
		{
			paper_memorypool_dealloc(image_pool, image);
			image = nullptr;
		}
		else
		{
			image->bitmap = bitmap;
		}
	}
	/*ID2D1Bitmap* bitmap;
	ID2D1RenderTarget* pRen;
	bitmap->GetRender

	ID2D1HwndRenderTarget* renderTarget;

	renderTarget->CreateCompatibleRenderTarget();

	ID2D1BitmapRenderTarget* renderTarget2;
	renderTarget2->CreateBitmapRenderTarget()
	IWICBitmap* pWicBitmap;
	Direct2DFactory->CreateWicBitmapRenderTarget(pWicBitmap, D2D1::RenderTargetProperties(), &render->renderTarget);
	GUID_WICPixelFormat32bppPBGRA;
	GUID_WICPixelFormat32bppPRGBA;*/
	return image;
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
	int width = 0;
	int height = 0;
	int comp = 0;
	stbi_uc* stbi_image = stbi_load_from_file(pFile, &width, &height, &comp, STBI_rgb_alpha);
	fclose(pFile);
	if (!stbi_image)
	{
		return nullptr;
	}
	struct paper_image* image = nullptr;
	image = (struct paper_image*)paper_memorypool_alloc(image_pool);
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

struct paper_image* paper_image_load_from_memory(struct paper_render* render, void* data, uint32 len)
{
	assert(render);
	assert(data);
	int width = 0;
	int height = 0;
	int comp = 0;
	stbi_uc* stbi_image = stbi_load_from_memory((const stbi_uc*)data, len, &width, &height, &comp, STBI_rgb_alpha);
	if (!stbi_image)
	{
		return nullptr;
	}
	struct paper_image* image = nullptr;
	image = (struct paper_image*)paper_memorypool_alloc(image_pool);
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
	paper_memorypool_dealloc(image_pool, image);
}

PAPER_API struct paper_brush* paper_brush_create_solid(struct paper_render* render, struct paper_color* color)
{
	D2D1_COLOR_F c;
	c.r = color->r;
	c.g = color->g;
	c.b = color->b;
	c.a = color->a;
	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT hr = render->renderTarget->CreateSolidColorBrush(c, &brush);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_brush* b = (struct paper_brush*)paper_memorypool_alloc(brush_pool);
	if (!b)
	{
		brush->Release();
		return nullptr;
	}
	b->brush = brush;
	return b;
}

PAPER_API struct paper_brush* paper_brush_create_lineargradient(struct paper_render* render, struct paper_gradient_stop* attributes, uint32 count, struct paper_gradient_pos* pos)
{
	ID2D1LinearGradientBrush* pLinearGradientBrush = NULL;
	ID2D1GradientStopCollection* pGradientStopCollection = NULL;
	/*D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Red);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Blue);
	gradientStops[1].position = 1.0f;*/
	render->renderTarget->CreateGradientStopCollection((D2D1_GRADIENT_STOP*)attributes, count, &pGradientStopCollection);
	if (!pGradientStopCollection)
	{
		return nullptr;
	}
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearGradientBrushProperties;
	linearGradientBrushProperties.startPoint = D2D1::Point2F((float)pos->start_point.x, (float)pos->start_point.y);
	linearGradientBrushProperties.endPoint = D2D1::Point2F((float)pos->end_point.x, (float)pos->end_point.y);
	//render->renderTarget->CreateLinearGradientBrush();
	HRESULT hr = render->renderTarget->CreateLinearGradientBrush(linearGradientBrushProperties, pGradientStopCollection, &pLinearGradientBrush);
	SafeRelease(&pGradientStopCollection);
	if (FAILED(hr))
	{
		return nullptr;
	}
	struct paper_brush* b = (struct paper_brush*)paper_memorypool_alloc(brush_pool);
	if (!b)
	{
		SafeRelease(&pLinearGradientBrush);
		return nullptr;
	}
	b->brush = pLinearGradientBrush;
	return b;
}

struct paper_brush* paper_brush_create_from_image(struct paper_render* render, struct paper_image* image)
{
	ID2D1BitmapBrush* bitmapBrush = nullptr;
	assert(render && render->renderTarget && image && image->bitmap);
	render->renderTarget->CreateBitmapBrush(image->bitmap, &bitmapBrush);
	if (!bitmapBrush)
	{
		return nullptr;
	}
	struct paper_brush* b = (struct paper_brush*)paper_memorypool_alloc(brush_pool);
	if (b)
	{
		b->brush = bitmapBrush;
	}
	else
	{
		bitmapBrush->Release();
	}
	return b;
}

void paper_brush_set_opacity(struct paper_brush* brush, float opacity)
{
	brush->brush->SetOpacity(opacity);
}

void paper_brush_solid_setcolor(struct paper_brush* solidbrush, struct paper_color* color)
{
	ID2D1SolidColorBrush* brush = (ID2D1SolidColorBrush*)solidbrush->brush;
	brush->SetColor(D2D1::ColorF(color->r, color->g, color->b, color->a));
}

void paper_brush_free(struct paper_brush* brush)
{
	assert(brush && brush->brush);
	brush->brush->Release();
	paper_memorypool_dealloc(brush_pool, brush);
}

struct paper_font* paper_font_create(const wchar_t* family, float size, const wchar_t* localname /*= L"zh-cn"*/)
{
	struct paper_font* font = nullptr;
	IDWriteTextFormat* textFormat = nullptr;
	DWriteFactory->CreateTextFormat(
		family,
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		localname,
		&textFormat
	);
	if (!textFormat)
	{
		return nullptr;
	}
	font = (struct paper_font*)paper_memorypool_alloc(font_pool);
	font->textFormat = textFormat;
	return font;
}

void paper_font_free(struct paper_font* font)
{
	paper_memorypool_dealloc(font_pool, font);
}
