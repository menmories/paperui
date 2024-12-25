#ifndef PAPERUI_CWINDOWBASE_H
#define PAPERUI_CWINDOWBASE_H

#include "platformdef.h"
#include "Render/CRect.h"

class PAPERUI_API CWindowBase
{
public:
	CWindowBase();
	virtual ~CWindowBase();

	void Resize(int width, int height);
	void Move(int x, int y);
	void Show();
	void ShowNormal();
	void Hide();
	void ShowMaximized();
	void ShowMinimized();
	bool IsMaximized();
	bool IsMinimized();
	void SetMinimizedWidth(int width);
	void SetMinimizedHeight(int height);
	void SetMinimizedSize(int width, int height);
	void SetMaximizedSize(int width, int height);
	void SetMaximizedWidth(int width);
	void SetMaximizedHeight(int height);
	void CenterWindow();
	const CRect GetRect() const;
	const CRect GetFramelessRect() const;

	HWND CreateNativeWindow(const char* title, int x, int y, int width, int height, HWND hParent);

	inline HWND GetWindowId()const
	{
		return m_hWnd;
	}
protected:
	virtual LRESULT NativeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;
	int m_nMaxWidth;
	int m_nMaxHeight;
	int m_nMinWidth;
	int m_nMinHeight;
};



#endif	//PAPERUI_CWINDOWBASE_H

