#ifndef __PAPER_EVENT_H__
#define __PAPER_EVENT_H__


#include "paper_define.h"


#ifdef __cplusplus
extern "C"
{
#endif
	enum paper_event_type
	{
		paper_event_quit = 0,
		paper_event_mouse,
		paper_event_key,
		paper_event_mouse_wheel,
	};


	struct paper_event
	{
		enum paper_event_type type;
		int32 code;
		uint_ptr param;
	};

	extern uint8 paper_key[255];

	PAPER_API int32 paper_event_run(struct paper_event* event);

	PAPER_API uint_ptr paper_event_handle(struct paper_event* event);

	

	//PAPER_API LRESULT CALLBACK handle_windows_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#ifdef __cplusplus
}
#endif
#endif	//__PAPER_EVENT_H__

