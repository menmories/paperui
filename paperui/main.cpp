// paperui.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
* 目标：跨平台gui
*/

#include <iostream>
#include "paper_window.h"
#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>

#include "paper_memorypool.h"
#include "paper_vector.h"

#include "paper_elememt.h"
#include "paper_render.h"
#include "paper_builder.h"

#include "paper_event.h"
#include "paper_application.h"

//gdi+来绘制一张图片
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")



//窗口回调函数，在这里处理窗口消息
uint_ptr __stdcall handle_my_event(struct paper_event* event);
void initui(struct paper_window* window);
void init_control(struct paper_render* render);
struct paper_window* window = nullptr;
int32 window_width = 1200;
int32 window_height = 700;
int main(int argc, char** argv)
{
    paper_application_init();
    //paper_set_window_event_cb(handle_my_event);      //如果想要接手事件循环，那么接手此消息，必须要在窗口创建前调用
    window = paper_window_new(TEXT("纸片UI窗口"), 100, 100, window_width, window_height, nullptr);
    //初始化你的UI
    initui(window);
    struct paper_color color = { 0.0f, 0.0f, 0.0f, 1.0f };
    paper_window_set_clearcolor(window, &color);
    paper_window_center_screen(window);
    paper_window_show(window);
    return paper_application_run();
}


uint_ptr __stdcall handle_my_event(struct paper_event* event)
{
    struct paper_window* window = (struct paper_window*)event->source;
    static paper_render* render = nullptr;
    static paper_builder* builder = nullptr;
    if (event->type == WM_PAINT)   //WM_PAINT	//绘图消息，告诉我们窗口应该绘图了
    {
        PAINTSTRUCT ps = { 0 };
        //HDC hdc = ::BeginPaint(hWnd, &ps);
        //在这里加入绘图代码

        //::EndPaint(hWnd, &ps);
        struct paper_color color = {1.0f, 1.0f, 1.0f, 1.0f};
        paper_render_begin_draw(render, &color);    //开始绘制前必须clear
        struct paper_render* compatible_render = paper_render_create_compatible(render, 
            paper_rect_get_width(&builder->image_rect), 
            paper_rect_get_height(&builder->image_rect));
        paper_render_begin_draw2(compatible_render);
        //paper_brush_set_opacity(builder->image_brush, 0.5f);
        paper_render_fill_rectangle(render, &builder->image_rect, builder->image_brush);
        paper_render_draw_image(compatible_render, builder->image,
            builder->image_rect.left, 
            builder->image_rect.top, 
            builder->image_rect.right - builder->image_rect.left,
            builder->image_rect.bottom - builder->image_rect.top);

        paper_render_end_draw(compatible_render);
        struct paper_image* image = paper_image_get_from_render(compatible_render);
        paper_render_draw_image(render, image, 0, 0, window_width, window_height);
        paper_render_end_draw(render);
        paper_render_free(compatible_render);
        return FALSE;
    }
    if (event->type == WM_CLOSE)
    {
        paper_window_destroy(window);
        return FALSE;
    }
    if (event->type == WM_DESTROY)
    {
        paper_render_free(render);
        PostQuitMessage(0);
        return FALSE;
    }
    if (event->type == WM_SIZE)
    {
        window_width = LOWORD(event->param2);
        window_height = HIWORD(event->param2);
        paper_render_resize(render, window_width, window_height);
        paper_rect_set_size(&builder->image_rect, window_width, window_height);
        return FALSE;
    }
    if (event->type == WM_ERASEBKGND)
    {
        return FALSE;
    }
	if (event->type == WM_CREATE)
	{
        HWND hWnd = (HWND)paper_window_get_native_id(window);
		render = paper_render_create(hWnd, window_width, window_height);
        //builder = paper_builder_load(render, "E:\\GitHub\\paperui\\Output\\Win64\\window.xml", Load_XML);
        builder = paper_builder_load(render, "window.xml", Load_XML);
        //builder = paper_builder_load(render, "window.xml", Load_XML);
        if (!builder)
        {
            fprintf(stderr, "paper_builder_load 失败\n");
        }
        else
        {
			struct paper_window* win = paper_window_new_from_native_handle(hWnd);
			paper_window_set_pos(win, builder->x, builder->y);
			//paper_window_set_size(win, builder->width, builder->height);
            //paper_window_set_pos(win, 0, 0);
			uint32 width = builder->image_rect.right - builder->image_rect.left;
			uint32 height = builder->image_rect.bottom - builder->image_rect.top;

            paper_rect_set_pos(&builder->image_rect, 0, 0);

            paper_window_set_size(win, width, height);
			paper_window_free_form_native_handle(win);
			//backgroundImage = paper_image_load_from_file(render, "98146720_p0_master1200.jpg");
        }
        
		return FALSE;
	}
    return paper_window_default_handle(event);     //默认处理窗口回调函数
}


void initui(struct paper_window* window)
{
    struct paper_render* render = paper_window_get_render(window);
    init_control(render);
    return;



    struct paper_widget* widget = paper_widget_new(nullptr);
    struct paper_color color = {0.1f, 1.0f, 1.0f, 1.0f};
    struct paper_color color2 = { 0.0f, 0.5f, 1.0f, 1.0f };
    /*widget->background1 = paper_brush_new_solid(render, &color);
    widget->background2 = paper_brush_new_solid(render, &color2);
    widget->background = widget->background1;*/
    /*
    paper_rect_set_pos(&widget->rect, 10, 10);
    paper_rect_set_size(&widget->rect, 120, 50);
    paper_widget_add_event(widget, PAPER_LISTEN_EVENT_LBUTTON | PAPER_LISTEN_EVENT_MOUSEENTER | PAPER_LISTEN_EVENT_MOUSELEAVE | PAPER_LISTEN_EVENT_RESIZE);
    paper_window_add_widget(window, widget);

	widget = paper_widget_new(nullptr);
	widget->render = render;
	widget->background1 = paper_brush_new_solid(render, &color);
	widget->background2 = paper_brush_new_solid(render, &color2);
	widget->background = widget->background1;
	paper_rect_set_pos(&widget->rect, 150, 10);
	paper_rect_set_size(&widget->rect, 120, 50);
	paper_widget_add_event(widget, PAPER_LISTEN_EVENT_LBUTTON | PAPER_LISTEN_EVENT_MOUSEENTER | PAPER_LISTEN_EVENT_MOUSELEAVE | PAPER_LISTEN_EVENT_RESIZE);
	paper_window_add_widget(window, widget);

	widget = paper_widget_new(nullptr);
	widget->render = render;
	widget->background1 = paper_brush_new_solid(render, &color);
	widget->background2 = paper_brush_new_solid(render, &color2);
	widget->background = widget->background1;
	paper_rect_set_pos(&widget->rect, 130, 180);
	paper_rect_set_size(&widget->rect, 120, 50);
	paper_widget_add_event(widget, PAPER_LISTEN_EVENT_LBUTTON | PAPER_LISTEN_EVENT_MOUSEENTER | PAPER_LISTEN_EVENT_MOUSELEAVE | PAPER_LISTEN_EVENT_RESIZE);
	paper_window_add_widget(window, widget);*/

    int8* szApplicationPath = nullptr;
    uint32 appPathLen = 0;
    paper_application_get_path(&szApplicationPath, &appPathLen);        //获取当前应用程序路径
    std::string strApplicationPath = szApplicationPath;
    size_t pos = strApplicationPath.rfind('\\');
    if (pos != std::string::npos)
    {
        strApplicationPath = strApplicationPath.substr(0, pos);
    }
    std::string imagePath = strApplicationPath + "\\111158151_p0_master1200.jpg";
    struct paper_image* image = paper_image_load_from_file(render, imagePath.c_str());
    struct paper_widget_image* image_widget = paper_widget_image_new(NULL, image);
	paper_rect_set_pos(&((struct paper_widget*)(image_widget))->rect, 100, 100);
	paper_rect_set_size(&((struct paper_widget*)(image_widget))->rect, 380, 560);
    //paper_window_add_widget(window, (struct paper_widget*)image_widget);

    struct paper_overlay* overlay = paper_overlay_new(nullptr);
    paper_window_set_root_widget(window, (struct paper_widget*)overlay);

    struct paper_overlay_slot* slot_img = paper_overlay_slot_new();
    slot_img->halign = paper_halign_center;
    slot_img->valign = paper_valign_center;
    slot_img->widget = (struct paper_widget*)image_widget;
    paper_overlay_add_slot(overlay, slot_img);

	std::wstring button_str = L"我是居中按钮";
    struct paper_color text_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	struct paper_font* font = paper_font_create(L"微软雅黑", 24.0f, L"zh-cn");
	struct paper_widget_text* text_widget = paper_widget_text_new(nullptr, &text_color, font, button_str.c_str(), (uint32)button_str.length());
    paper_widget_set_size((struct paper_widget*)text_widget, 100, 40);
    paper_widget_set_pos((struct paper_widget*)text_widget, 0, 0);
	struct paper_overlay_slot* slot_text = paper_overlay_slot_new();
	slot_text->halign = paper_halign_center;
	slot_text->valign = paper_valign_center;
	slot_text->widget = (struct paper_widget*)text_widget;
	paper_overlay_add_slot(overlay, slot_text);


    //struct paper_widget_button* button_left = paper_widget_button_new(nullptr)

    /*for (int32 i = 0; i < 20; i++)
    {
        for (int32 j = 0; j < 40; j++)
        {
			widget = paper_widget_new(nullptr);
			widget->render = render;
			widget->background1 = paper_brush_new_solid(render, &color);
			widget->background2 = paper_brush_new_solid(render, &color2);
			widget->background = widget->background1;
			paper_rect_set_pos(&widget->rect, i * 100, j * 40);
			paper_rect_set_size(&widget->rect, 90, 30);
			paper_widget_add_event(widget, PAPER_LISTEN_EVENT_LBUTTON | PAPER_LISTEN_EVENT_MOUSEENTER | PAPER_LISTEN_EVENT_MOUSELEAVE | PAPER_LISTEN_EVENT_RESIZE);
			paper_window_add_widget(window, widget);
        }
    }*/
}

void init_control(struct paper_render* render)
{
    paper_widget* widget = paper_widget_new(nullptr);
	struct paper_overlay* overlay = paper_overlay_new(nullptr);
	paper_window_set_root_widget(window, (struct paper_widget*)overlay);


	int8* szApplicationPath = nullptr;
	uint32 appPathLen = 0;
	paper_application_get_path(&szApplicationPath, &appPathLen);        //获取当前应用程序路径
	std::string strApplicationPath = szApplicationPath;
	size_t pos = strApplicationPath.rfind('\\');
	if (pos != std::string::npos)
	{
		strApplicationPath = strApplicationPath.substr(0, pos);     //去除掉最后一个\符号
	}
	//std::string imagePath = strApplicationPath + "\\111158151_p0_master1200.jpg";
    std::string imagePath1 = strApplicationPath + "/92424854_p0.png";
    std::string imagePath2 = strApplicationPath + "/98146720_p0_master1200.jpg";
	struct paper_image* image1 = paper_image_load_from_file(render, imagePath1.c_str());
    struct paper_image* image2 = paper_image_load_from_file(render, imagePath2.c_str());
	struct paper_widget_image* image_widget1 = paper_widget_image_new(NULL, image1);
    struct paper_widget_image* image_widget2 = paper_widget_image_new(NULL, image2);
	//paper_rect_set_pos(&((struct paper_widget*)(image_widget))->rect, 1001, 100);
    struct paper_size imagesize;
    paper_image_get_size(image1, &imagesize);
    imagesize.width = (int32)(((float)imagesize.width) * 0.35f);
    imagesize.height = (int32)(((float)imagesize.height) * 0.35f);
	paper_rect_set_size(&((struct paper_widget*)(image_widget1))->rect, imagesize.width, imagesize.height);

	paper_image_get_size(image2, &imagesize);
	imagesize.width = (int32)(((float)imagesize.width) * 0.5f);
	imagesize.height = (int32)(((float)imagesize.height) * 0.5f);
	paper_rect_set_size(&((struct paper_widget*)(image_widget2))->rect, imagesize.width, imagesize.height);


    //创建overlay插槽
	struct paper_overlay_slot* slot_img1 = paper_overlay_slot_new();
	slot_img1->halign = paper_halign_right;
	slot_img1->valign = paper_valign_center;
	slot_img1->widget = (struct paper_widget*)image_widget1;
	paper_overlay_add_slot(overlay, slot_img1);

	struct paper_overlay_slot* slot_img2 = paper_overlay_slot_new();
	slot_img2->halign = paper_halign_left;
	slot_img2->valign = paper_valign_center;
	slot_img2->widget = (struct paper_widget*)image_widget2;
	paper_overlay_add_slot(overlay, slot_img2);

	std::wstring button_str = L"Consolas";
	struct paper_color text_color = { 1.0f, 0.1f, 0.0f, 1.0f };
    struct paper_font* font = paper_font_create(L"微软雅黑", 48.0f, L"zh-cn");
	struct paper_widget_text* text_widget = paper_widget_text_new(nullptr, &text_color, font, button_str.c_str(), (uint32)button_str.length());
    struct paper_font_metrics metrics;
    paper_font_get_metrics(text_widget->text_font, button_str.c_str(), (uint32)button_str.length(), &metrics);
	//paper_widget_set_size((struct paper_widget*)text_widget, metrics.width, metrics.height);
    paper_widget_set_size((struct paper_widget*)text_widget, 500, 320);
	paper_widget_set_pos((struct paper_widget*)text_widget, 0, 0);
	struct paper_overlay_slot* slot_text = paper_overlay_slot_new();
	slot_text->halign = paper_halign_left;
	slot_text->valign = paper_valign_center;
	slot_text->widget = (struct paper_widget*)text_widget;
	paper_overlay_add_slot(overlay, slot_text);
}
