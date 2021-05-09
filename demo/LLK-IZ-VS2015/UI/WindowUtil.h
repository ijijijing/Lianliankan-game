#ifndef _WINDOWUTIL_H_
#define _WINDOWUTIL_H_

#include <atlimage.h>
#include <CommCtrl.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <gdiplus.h>
#include <memory>
#include <ShellScalingAPI.h>
#include <Shlobj.h>
#include <Windows.h>

#include "..\model\LLKModel.h"

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

#define WND_NUM 3

using namespace Gdiplus;

typedef struct WindowGeometricAttributes {
	int X;
	int Y;
	int nWidth;
	int nHeight;
} WindowGeometricAttributes, WndGeoAttr;

extern HWND splashUIhWnd;
extern HWND mainUIhWnd;
extern HWND gameUIhWnd;
extern HWND rankingUIhWnd;

extern HINSTANCE splashUIhInstance;
extern HINSTANCE mainUIhInstance;
extern HINSTANCE gameUIhInstance;
extern HINSTANCE rankingUIhInstance;

extern LPCSTR splashUIRegClassName;
extern LPCSTR mainUIRegClassName;
extern LPCSTR gameUIRegClassName;
extern LPCSTR rankingUIRegClassName;

extern LPCSTR splashUITitle;
extern LPCSTR mainUITitle;
extern LPCSTR gameUITitle;
extern LPCSTR rankingUITitle;

extern WindowGeometricAttributes splashUIWGA;
extern WindowGeometricAttributes mainUIWGA;
extern WindowGeometricAttributes gameUIWGA;
extern WindowGeometricAttributes rankingUIWGA;

extern HINSTANCE initInstance;
extern HINSTANCE inithPrevInstance;
extern LPSTR initlpCmdLine;
extern int initnCmdShow;
extern int FullScreenWidth;
extern int FullScreenHeight;

const wchar_t SplashImagePath[] = L"res\\splash.bmp";
const int SplashWaitTime = 0;

extern bool isTimeMode;

void setWindowSize(HWND hWnd, int width, int height);

#endif // !_WINDOWUTIL_H_
