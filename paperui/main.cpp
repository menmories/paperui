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

//gdi+来绘制一张图片
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")



//窗口回调函数，在这里处理窗口消息
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
ULONG_PTR token;
Gdiplus::GdiplusStartupInput input;
struct paper_window* window = nullptr;
int32 window_width = 1200;
int32 window_height = 700;
int main(int argc, char** argv)
{
    struct student
    {
        int a;
        int b;
        char szTitle[32];
    };

 //   struct paper_vector* vec = paper_vector_create(sizeof(uint32));
 //   int subway = 1;
 //   for (int i = 0; i < 8; i++)
 //   {
 //       uint32 temp = i + 1;
 //       paper_vector_add_front(vec, &temp);
 //   }

	//for (int i = 0; i < 8; i++)
	//{
 //       uint32 temp = 0;
	//	paper_vector_pop_front(vec, &temp);
 //       printf("number=%d\n", temp);
	//}

 //   paper_vector_free(vec);
 //   return 0;

	struct paper_memorypool* mp = paper_memorypool_create(sizeof(struct student));
	struct student* stu[10];
	for (int i = 0; i < 10; i++)
	{
		stu[i] = (struct student*)paper_memorypool_alloc(mp);
		stu[i]->a = i;
		stu[i]->b = i + 1;
	}

	for (int i = 0; i < 10; i++)
	{
		stu[i]->a = i;
		stu[i]->b = i + 1;
		strcpy(stu[i]->szTitle, "hello world");
		printf("a:%d,b:%d,title:%s\n", stu[i]->a, stu[i]->b, stu[i]->szTitle);
	}

	for (int i = 0; i < 10; i++)
	{
		paper_memorypool_dealloc(mp, stu[i]);
	}



    Gdiplus::GdiplusStartup(&token, &input, nullptr);
    paper_render_initenv();         //初始化paper渲染器环境
    window = paper_window_create(TEXT("你好呀"), WndProc, 100, 100, window_width, window_height);
    paper_window_show(window);

    struct paper_event e;
    while (paper_event_run(&e))
    {
        //do something
    }
    paper_render_destroyenv();
    return e.code;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static paper_render* render = nullptr;
    //static paper_image* backgroundImage = nullptr;
    static paper_builder* builder = nullptr;
    if (uMsg == WM_PAINT)   //WM_PAINT	//绘图消息，告诉我们窗口应该绘图了
    {
        PAINTSTRUCT ps = { 0 };
        //HDC hdc = ::BeginPaint(hWnd, &ps);
        //在这里加入绘图代码

        //::EndPaint(hWnd, &ps);
        struct paper_color color = {0.1f, 1.0f, 1.0f, 1.0f};
        paper_render_begin_draw(render);
        paper_render_clear(render, &color);
        struct paper_render* compatible_render = paper_render_create_compatible(render);

        paper_render_begin_draw(compatible_render);
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
    if (uMsg == WM_CLOSE)
    {
        DestroyWindow(hWnd);
        return FALSE;
    }
    if (uMsg == WM_DESTROY)
    {
        paper_render_free(render);
        PostQuitMessage(0);
        return FALSE;
    }
    if (uMsg == WM_SIZE)
    {
        window_width = LOWORD(lParam);
        window_height = HIWORD(lParam);
        paper_render_resize(render, window_width, window_height);
        paper_rect_set_size(&builder->image_rect, window_width, window_height);
        return FALSE;
    }
    if (uMsg == WM_ERASEBKGND)
    {
        return FALSE;
    }
	if (uMsg == WM_CREATE)
	{
        
		render = paper_render_create(hWnd, window_width, window_height);
        //builder = paper_builder_load(render, "E:\\GitHub\\paperui\\Output\\Win64\\window.xml", Load_XML);
        builder = paper_builder_load(render, "window.xml", Load_XML);
        if (!builder)
        {
            fprintf(stderr, "paper_builder_load 失败\n");
        }
        else
        {
			struct paper_window* win = paper_window_native_fromhandle((HWND)hWnd);
			paper_window_set_pos(win, builder->x, builder->y);
			//paper_window_set_size(win, builder->width, builder->height);
            //paper_window_set_pos(win, 0, 0);
			uint32 width = builder->image_rect.right - builder->image_rect.left;
			uint32 height = builder->image_rect.bottom - builder->image_rect.top;

            paper_rect_set_pos(&builder->image_rect, 0, 0);

            paper_window_set_size(win, width, height);
			paper_window_free(win);
			//backgroundImage = paper_image_load_from_file(render, "98146720_p0_master1200.jpg");
        }
        
		return FALSE;
	}
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);     //默认处理窗口回调函数
}