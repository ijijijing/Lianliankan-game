#include "XFonts.h"

// 设备上下文
HDC hfHDC;
// 自定义字体句柄
HFONT hfFont;
// 自定义字体
LOGFONT fLogFont;
// 字体名称
int fWordSize;
TCHAR FontName[32] = {"微软雅黑"};
int onLoadFontFromFile(std::wstring& fontPath) {
	return 0;
}

int onFontRender(HDC hdc, int wordSize) {
	hfHDC = hdc;
	fWordSize = wordSize;
	setFont();
	SelectObject(hdc, hfFont);
	return 0;
}

int onFontRender(HWND hwnd, int wordSize) {
	hfHDC = GetDC(hwnd);
	fWordSize = wordSize;
	setFont();
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hfFont, 1);
	return 0;
}

int setFont() {
	memset(&fLogFont, 0, sizeof(fLogFont));
	fLogFont.lfHeight = GetDeviceCaps(hfHDC, LOGPIXELSY) * fWordSize / 72;
	strcpy_s(fLogFont.lfFaceName, FontName);
	hfFont = CreateFontIndirect(&fLogFont);
	return 0;
}