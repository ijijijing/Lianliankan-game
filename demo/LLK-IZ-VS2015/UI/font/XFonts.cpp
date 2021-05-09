#include "XFonts.h"

// �豸������
HDC hfHDC;
// �Զ���������
HFONT hfFont;
// �Զ�������
LOGFONT fLogFont;
// ��������
int fWordSize;
TCHAR FontName[32] = {"΢���ź�"};
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