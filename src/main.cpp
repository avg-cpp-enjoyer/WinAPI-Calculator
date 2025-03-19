#include "Calculator.h"

int APIENTRY WinMain([[maybe_unused]] HINSTANCE hInstance, HINSTANCE, [[maybe_unused]] LPSTR pCmdLine, int nCmdShow) {
	Calculator window;

	if (!window.Create(L"Calculator", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 
		WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, window.width, window.height)) {
		return 0;
	}

	ShowWindow(window.Window(), nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}