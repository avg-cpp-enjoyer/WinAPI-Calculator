#pragma once

#include "Base.h"
#include "Button.h"
#include "EditWindow.h"
#include "ExpressionSolver.h"

#include <vector>
#include <memory>
#include <ShellScalingApi.h>

#pragma comment(lib, "Shcore.lib")

constexpr int IDC_BUTTON_1 = 1;
constexpr int IDC_BUTTON_2 = 2;
constexpr int IDC_BUTTON_3 = 3;
constexpr int IDC_BUTTON_4 = 4;
constexpr int IDC_BUTTON_5 = 5;
constexpr int IDC_BUTTON_6 = 6;
constexpr int IDC_BUTTON_7 = 7;
constexpr int IDC_BUTTON_8 = 8;
constexpr int IDC_BUTTON_9 = 9;
constexpr int IDC_BUTTON_0 = 10;

constexpr int IDC_BUTTON_NEGATIVE = 11;
constexpr int IDC_BUTTON_POINT = 12;
constexpr int IDC_BUTTON_DEG = 13;
constexpr int IDC_BUTTON_C = 14;
constexpr int IDC_BUTTON_EQ = 15;
constexpr int IDC_BUTTON_ERASE = 16;
constexpr int IDC_BUTTON_SIN = 17;
constexpr int IDC_BUTTON_COS = 18;
constexpr int IDC_BUTTON_TAN = 21;
constexpr int IDC_BUTTON_ATAN = 22;
constexpr int IDC_BUTTON_SQRT = 23;
constexpr int IDC_BUTTON_POW = 25;
constexpr int IDC_BUTTON_PLUS = 26;
constexpr int IDC_BUTTON_MINUS = 27;
constexpr int IDC_BUTTON_MULTIPLY = 28;
constexpr int IDC_BUTTON_DIVISION = 29;
constexpr int IDC_BUTTON_L_PAR = 30;
constexpr int IDC_BUTTON_R_PAR = 31;
constexpr int IDC_BUTTON_LN = 32;
constexpr int IDC_BUTTON_LOG = 33;

class Calculator : public Base<Calculator> {
public:
	int width;
	int height;

	Calculator();
	~Calculator();

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	LRESULT HandleKeyboardInputs(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT PaintEditWindow(WPARAM wParam) const;
	void AddEditWindow();
	void AddButtons();
	void OnPaint() const;
	void AppendText(const std::wstring& text);
	float GetScreenDpi() const;

	std::unique_ptr<EditWindow> m_mainEditWindow;
	std::unique_ptr<EditWindow> m_infoEditWindow;
	std::unique_ptr<Button> m_degButton;
	std::vector<std::unique_ptr<Button>> m_buttons;

	HFONT m_hEditFont;
	HFONT m_hInfoFont;
	HFONT m_hBtnFont;

	RECT m_textRect;

	std::wstring m_expr;

	HBRUSH m_editWindowBrush;
	HBRUSH m_bgBrush;
	HPEN m_editWindowPen;

	float m_scaleFactor = 1.0f;
	bool m_isDegree = true;
	int m_uiElemSpacing;
	int m_uiElemCornerRad;
};

struct ButtonConfig {
	const wchar_t* label;
	int x;
	int y;
	int cornerRadius;
	int id;
	COLORREF textColor;
	COLORREF clrDefault;
	COLORREF clrClicked;
	COLORREF clrHovered;
};
