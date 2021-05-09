#ifndef XFONTS_H
#define XFONTS_H

#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int onLoadFontFromFile(std::wstring& fontPath);

int onFontRender(HDC hdc, int wordSize);

int onFontRender(HWND hwnd, int wordSize);

int setFont();

#endif
