#include "EditWindow.h"

EditWindow::EditWindow(HWND parent, const wchar_t* text, int x, int y, int width, int height, HFONT font)
  : m_hFont(font) {
	m_hEdit = CreateWindow(
		L"EDIT", text,
		WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
		x, y, width, height,
		parent,
		nullptr,
		GetModuleHandle(nullptr),
		this
	);

	m_editWindowBrush = CreateSolidBrush(RGB(50, 50, 50));

	SetWindowLongPtr(m_hEdit, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	m_originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_hEdit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(EditProc)));
	SendMessage(m_hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(m_hFont), true);
}

EditWindow::~EditWindow() {
	DeleteObject(m_editWindowBrush);
}

void EditWindow::SetText(const std::wstring& text) const {
	SendMessage(m_hEdit, WM_SETREDRAW, false, 0);
	SetWindowText(m_hEdit, text.c_str());
	SendMessage(m_hEdit, WM_SETREDRAW, true, 0);
	RedrawWindow(m_hEdit, nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

std::wstring EditWindow::GetText() const {
	int length = GetWindowTextLength(m_hEdit);
	std::wstring text(length, L'\0');
	SendMessage(m_hEdit, WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text.data()));
	return text;
}

void EditWindow::Clear() {
	SetWindowText(m_hEdit, L"");
}

HWND EditWindow::GetHandle() const {
	return m_hEdit;
}

LRESULT EditWindow::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto* edit = reinterpret_cast<EditWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!edit) return DefWindowProc(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_MOUSEACTIVATE:
		return MA_NOACTIVATE;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		return 0;
	case WM_SETFOCUS:
		SetFocus(GetParent(hWnd));
		return 0;
	}

	return CallWindowProc(edit->m_originalWndProc, hWnd, msg, wParam, lParam);
}
