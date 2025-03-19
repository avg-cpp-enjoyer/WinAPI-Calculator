#include "Calculator.h"

Calculator::Calculator() : Base() {
	m_bgBrush = CreateSolidBrush(RGB(30, 30, 30));
	m_editWindowBrush = CreateSolidBrush(RGB(50, 50, 50));
	m_editWindowPen = CreatePen(PS_NULL, 0, 0);

	m_scaleFactor = GetScreenDpi() / 96.0f;

	m_uiElemSpacing   = static_cast<int>(2.0f  * m_scaleFactor);
	m_uiElemCornerRad = static_cast<int>(7.0f * m_scaleFactor);

	m_hBtnFont  = CreateFont(static_cast<int>(22 * m_scaleFactor), 0, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

	m_hEditFont = CreateFont(static_cast<int>(55 * m_scaleFactor), 0, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

	m_hInfoFont = CreateFont(static_cast<int>(20 * m_scaleFactor), 0, 0, 0, FW_DONTCARE, false, false, false, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

	m_textRect = {
		m_uiElemSpacing,
		m_uiElemSpacing + static_cast<int>(1.0f * m_scaleFactor),
		static_cast<int>(422.0f * m_scaleFactor),
		static_cast<int>(107.0f * m_scaleFactor),
	};

	width  = static_cast<int>(440.0f * m_scaleFactor);
	height = static_cast<int>(349.0f * m_scaleFactor);
}

Calculator::~Calculator() {
	DeleteObject(m_hBtnFont);
	DeleteObject(m_hEditFont);
	DeleteObject(m_hInfoFont);
	DeleteObject(m_bgBrush);
	DeleteObject(m_editWindowBrush);
	DeleteObject(m_editWindowPen);
}

PCWSTR Calculator::ClassName() const {
	return L"Calculator Class";
}

LRESULT Calculator::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		HandleKeyboardInputs(uMsg, wParam, lParam);
		break;
	case WM_CREATE:
		AddEditWindow();
		AddButtons();
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_CTLCOLOREDIT:
		return PaintEditWindow(wParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void Calculator::AddEditWindow() {
	const int mainWindowX = static_cast<int>(2.0f   * m_scaleFactor);
	const int mainWindowY = static_cast<int>(50.0f  * m_scaleFactor);
	const int infoWindowX = static_cast<int>(10.0f  * m_scaleFactor);
	const int infoWindowY = static_cast<int>(20.0f  * m_scaleFactor);
	const int mainWindowH = static_cast<int>(52.0f  * m_scaleFactor);
	const int infoWindowH = static_cast<int>(20.0f  * m_scaleFactor);
	const int windowWidth = static_cast<int>(400.0f * m_scaleFactor);
	
	m_mainEditWindow = std::make_unique<EditWindow>(m_hWnd, L"0", mainWindowX, mainWindowY, windowWidth, mainWindowH, m_hEditFont);
	m_infoEditWindow = std::make_unique<EditWindow>(m_hWnd, L"", infoWindowX, infoWindowY, windowWidth, infoWindowH, m_hInfoFont);
}

LRESULT Calculator::HandleKeyboardInputs(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case '0':
		AppendText(L"0");
		break;
	case '1':
		AppendText(L"1");
		break;
	case '2':
		AppendText(L"2");
		break;
	case '3':
		AppendText(L"3");
		break;
	case '4':
		AppendText(L"4");
		break;
	case '5':
		AppendText(L"5");
		break;
	case '6':
		AppendText(L"6");
		break;
	case '7':
		AppendText(L"7");
		break;
	case '8':
		AppendText(L"8");
		break;
	case '9':
		AppendText(L"9");
		break;
	case VK_OEM_MINUS:
		AppendText(L"-");
		break;
	case VK_OEM_PLUS:
		AppendText(L"+");
		break;
	case VK_MULTIPLY:
		AppendText(L"×");
		break;
	case VK_DIVIDE:
		AppendText(L"÷");
		break;
	case VK_OEM_4:
		AppendText(L"(");
		break;
	case VK_OEM_6:
		AppendText(L")");
		break;
	case VK_BACK:
		if (!m_expr.empty()) {
			 m_expr.pop_back();
			 m_mainEditWindow->SetText(m_expr);
		}
		break;
	case VK_RETURN: {
		try {
			ExpressionSolver::UseDegrees(m_isDegree);
			std::wstring buffer = ExpressionSolver::EvaluateExpression(m_expr);
			m_expr.append(L" =");
			m_mainEditWindow->SetText(buffer);
			m_infoEditWindow->SetText(m_expr);
			m_expr.clear();
		} catch (const CalcException& ex) {
			m_infoEditWindow->SetText(ex.wwhat());
		}
		break;
	}
	default: return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

LRESULT Calculator::PaintEditWindow(WPARAM wParam) const {
	HDC hdc = reinterpret_cast<HDC>(wParam);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	return reinterpret_cast<LRESULT>(m_editWindowBrush);
}

void Calculator::AddButtons() {
	constexpr COLORREF DEFAULT_CLR      = RGB(59, 59, 59);
	constexpr COLORREF ALT_CLR          = RGB(51, 50, 49);
	constexpr COLORREF EQUALS_CLR       = RGB(188, 100, 0);
	constexpr COLORREF EQUALS_HOVER_CLR = RGB(160, 70, 0);
	constexpr COLORREF TEXT_COLOR       = RGB(255, 255, 255);

	const int buttonWidth  = static_cast<int>(70.0f * m_scaleFactor);
	const int buttonHeight = static_cast<int>(40.0f * m_scaleFactor);

	const int x = m_uiElemSpacing;
	const int y = m_textRect.bottom + m_uiElemSpacing - static_cast<int>(1.0f * m_scaleFactor);

	const std::vector<ButtonConfig> buttons = {
		{L"DEG", x,                   y, m_uiElemCornerRad, IDC_BUTTON_DEG,   TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"C",   x + buttonWidth,     y, m_uiElemCornerRad, IDC_BUTTON_C,     TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"⌫",  x + buttonWidth * 2, y, m_uiElemCornerRad, IDC_BUTTON_ERASE, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"(",   x + buttonWidth * 3, y, m_uiElemCornerRad, IDC_BUTTON_L_PAR, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L")",   x + buttonWidth * 4, y, m_uiElemCornerRad, IDC_BUTTON_R_PAR, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"sin", x + buttonWidth * 5, y, m_uiElemCornerRad, IDC_BUTTON_SIN,   TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},

		{L"1",   x,                   y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_1,     TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"2",   x + buttonWidth,     y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_2,     TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"3",   x + buttonWidth * 2, y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_3,     TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"+",   x + buttonWidth * 3, y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_PLUS,  TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"-",   x + buttonWidth * 4, y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_MINUS, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"cos", x + buttonWidth * 5, y + buttonHeight, m_uiElemCornerRad, IDC_BUTTON_COS,   TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},

		{L"4",   x,                   y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_4,        TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"5",   x + buttonWidth,     y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_5,        TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"6",   x + buttonWidth * 2, y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_6,        TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"×",   x + buttonWidth * 3, y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_MULTIPLY, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"÷",   x + buttonWidth * 4, y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_DIVISION, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"tan", x + buttonWidth * 5, y + buttonHeight * 2, m_uiElemCornerRad, IDC_BUTTON_TAN,      TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},

		{L"7",    x,                   y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_7,    TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"8",    x + buttonWidth,     y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_8,    TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"9",    x + buttonWidth * 2, y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_9,    TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"^",    x + buttonWidth * 3, y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_POW,  TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"√",    x + buttonWidth * 4, y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_SQRT, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"atan", x + buttonWidth * 5, y + buttonHeight * 3, m_uiElemCornerRad, IDC_BUTTON_ATAN, TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},

		{L"+/-", x,                   y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_NEGATIVE, TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"0",   x + buttonWidth,     y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_0,        TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L".",   x + buttonWidth * 2, y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_POINT,    TEXT_COLOR, DEFAULT_CLR, DEFAULT_CLR, ALT_CLR},
		{L"=",   x + buttonWidth * 3, y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_EQ,       TEXT_COLOR, EQUALS_CLR, EQUALS_CLR, EQUALS_HOVER_CLR},
		{L"ln",  x + buttonWidth * 4, y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_LN,       TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR},
		{L"log", x + buttonWidth * 5, y + buttonHeight * 4, m_uiElemCornerRad, IDC_BUTTON_LOG,      TEXT_COLOR, ALT_CLR, ALT_CLR, DEFAULT_CLR}
	};

	for (const auto& config : buttons) {
		if (!std::wcscmp(config.label, L"DEG")) {
			m_degButton = std::make_unique<Button>(m_hWnd, config.label, config.x, config.y, buttonWidth, buttonHeight, config.cornerRadius,
				config.id, m_hBtnFont, config.textColor, config.clrDefault, config.clrClicked, config.clrHovered
			);

			m_degButton->SetOnClick([this] {
				SetFocus(m_hWnd);
				m_isDegree = !m_isDegree;
				if (m_isDegree) {
					m_degButton->SetText(L"DEG");
				} else {
					m_degButton->SetText(L"RAD");
				}
			});

			continue;
		}

		auto button = std::make_unique<Button>(m_hWnd, config.label, config.x, config.y, buttonWidth, buttonHeight, config.cornerRadius, 
			config.id, m_hBtnFont, config.textColor, config.clrDefault, config.clrClicked, config.clrHovered
		);
		
		if (!std::wcscmp(config.label, L"C")) {
			button->SetOnClick([this] {
				SetFocus(m_hWnd);
				m_mainEditWindow->Clear();
				m_mainEditWindow->SetText(L"0");
				m_infoEditWindow->Clear();
				m_expr.clear();
			});
		}
		
		else if (!std::wcscmp(config.label, L"⌫")) {
			button->SetOnClick([this] {
				SetFocus(m_hWnd);
				if (!m_expr.empty()) {
					m_expr.pop_back();
					m_mainEditWindow->SetText(m_expr);
				}
			});
		}

		else if (!std::wcscmp(config.label, L"=")) {
			button->SetOnClick([this] {
				SetFocus(m_hWnd);
				try {
					ExpressionSolver::UseDegrees(m_isDegree);
					std::wstring result = ExpressionSolver::EvaluateExpression(m_expr);
					m_expr.append(L" =");
					m_mainEditWindow->SetText(result);
					m_infoEditWindow->SetText(m_expr);
				} catch (const CalcException& ex) {
					m_infoEditWindow->SetText(ex.wwhat());
				}
			});
		}

		else {
			button->SetOnClick([this, label = config.label] {
				SetFocus(m_hWnd);
				if (!m_expr.empty()) {
					m_infoEditWindow->Clear();
				}

				AppendText(label);
			});
		}

		m_buttons.push_back(std::move(button));
	}
}

void Calculator::OnPaint() const {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hWnd, &ps);

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	FillRect(hdc, &rc, m_bgBrush);

	SelectObject(hdc, m_editWindowBrush);
	SelectObject(hdc, m_editWindowPen);

	RoundRect(hdc, m_textRect.left, m_textRect.top, m_textRect.right, m_textRect.bottom, m_uiElemCornerRad, m_uiElemCornerRad);

	EndPaint(m_hWnd, &ps);
}

void Calculator::AppendText(const std::wstring& text) {
	m_expr = m_mainEditWindow->GetText();

	if (m_expr == L"0") {
		m_expr.clear();
	}

	m_expr.append(text);
	m_mainEditWindow->SetText(m_expr);

	int textLength = static_cast<int>(m_expr.length());
	SendMessage(m_mainEditWindow->GetHandle(), EM_SETSEL, textLength, textLength);
	SendMessage(m_mainEditWindow->GetHandle(), EM_SCROLLCARET, 0, 0);
}

float Calculator::GetScreenDpi() const {
	UINT dpiX = 0;
	UINT dpiY = 0;

	HMONITOR hMonitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	HRESULT hRes = GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

	return SUCCEEDED(hRes) ? static_cast<float>(dpiX) : 96.0f;
}
