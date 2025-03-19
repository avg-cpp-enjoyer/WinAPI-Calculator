#pragma once
#include <Windows.h>
#include <string>

class EditWindow {
public:
	EditWindow(HWND parent, const wchar_t* text, int x, int y, int width, int height, HFONT font);
	~EditWindow();
	void SetText(const std::wstring& text) const;
	std::wstring GetText() const;
	void Clear();
	HWND GetHandle() const;

	static LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hEdit;
	HBRUSH m_editWindowBrush;
	HFONT m_hFont;
	WNDPROC m_originalWndProc = nullptr;
	std::wstring m_text;
};