#include "WindowUtil.h"

HWND splashUIhWnd;
HWND mainUIhWnd;
HWND gameUIhWnd;
HWND rankingUIhWnd;

HINSTANCE splashUIhInstance;
HINSTANCE mainUIhInstance;
HINSTANCE gameUIhInstance;
HINSTANCE rankingUIhInstance;

LPCSTR splashUIRegClassName = "{RSAR-L6O1-XCWMQ1-2OU1}";
LPCSTR mainUIRegClassName = "{WSA1-L6O1-XCWMQF-OOU2}";
LPCSTR gameUIRegClassName = "{ADDD-L7O1-TCWMQ1-5OU1}";
LPCSTR rankingUIRegClassName = "{WTA1-LFO1-XCWMQ1-O1U1}";

LPCSTR splashUITitle = "LLK-IZ Splash";
LPCSTR mainUITitle = "LLK-IZ Main";
LPCSTR gameUITitle = "LLK-IZ Playing...";
LPCSTR rankingUITitle = "RangingList";

WindowGeometricAttributes splashUIWGA = { 300,300,500,500 };
WindowGeometricAttributes mainUIWGA = { 300,300,965, 687 };
WindowGeometricAttributes gameUIWGA = { 238,130,1200,880 };
WindowGeometricAttributes rankingUIWGA = { 300,300,500,500 };

HINSTANCE initInstance;
HINSTANCE inithPrevInstance;
LPSTR initlpCmdLine;
int initnCmdShow;
int FullScreenWidth;
int FullScreenHeight;
bool isTimeMode = true;

void setWindowSize(HWND hWnd, int width, int height)
{
	if (width > FullScreenWidth || height > FullScreenHeight || width < 1 || height < 1) {
		assert(0);
	}
	MoveWindow(hWnd, (FullScreenWidth - width) / 2, (FullScreenHeight - height) / 2, width, height, false);
}


