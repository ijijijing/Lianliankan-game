#include "GameUI.h"
#include "..\MouseUtil.h"
#include "..\WindowUtil.h"
#include "..\comctl\text\Text.h"
#include "..\comctl\border\Border.h"
#include "..\comctl\button\Button.h"
#include "..\..\controller\GameController.h"
#include "..\..\controller\GameInfoController.h"
#include <iostream>

static HBITMAP gameUIBufferBmp = NULL, gameUIMainBufferBmp = NULL, gameUIInfoBufferBmp = NULL;
static HDC gameUIBufferDC = NULL, gameUIMainBufferDC = NULL, gameUIInfoBufferDC = NULL;
static int gameUICxClientPix, gameUICyClientPix;
static MouseLRBtnGeoStatus gameUIMLRBGS;
static Gdiplus::Graphics* gameUIInfoGraphics = NULL;
static Gdiplus::Graphics* gameUIMainGraphics = NULL;
static Border* border;
static GameController* mainController = NULL;
static GameInfoController* infoController = NULL;
UINT Timer0;

time_t tc;
static unsigned long i;
static RECT rect;




// 窗口处理消息过程(回调函数)
LRESULT CALLBACK WindowProc_GameUI(HWND phwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

		/*
		WM_CREATE 在创建窗口(调用CreateWindow函数)时触发，只触发一次。
		WM_PAINT在需要重画(调用UpdateWindow函数)时调用，并多次触发。
		WM_CREATE先于WM_PAINT
		*/
	case WM_CREATE:
	{
		GetClientRect(phwnd, &rect);
		setWindowSize(phwnd, gameUIWGA.nWidth, gameUIWGA.nHeight);
		memset(&gameUIMLRBGS, 0, sizeof(gameUIMLRBGS));
		gameUICxClientPix = gameUIWGA.nWidth;
		gameUICyClientPix = gameUIWGA.nHeight;
		/*获取物理设备上下文(PhysicalDC)*/
		HDC hdc = GetDC(phwnd);

		/*创建缓冲内存DC*/
		gameUIBufferDC = CreateCompatibleDC(hdc);
		/*创建缓冲内存DC(用于绘制主Map)*/
		gameUIMainBufferDC = CreateCompatibleDC(hdc);
		/*创建缓冲内存DC(用于绘制按钮,进度条等控件以及消除路径)*/
		gameUIInfoBufferDC = CreateCompatibleDC(hdc);

		/*创建三个gameUICxClientPix, gameUICyClientPix大小并且适应DC设备环境的缓冲位图,注意这里的源DC应选择物理DC,不能选择内存DC*/
		gameUIBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);
		gameUIMainBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);
		gameUIInfoBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);

		/*释放物理设备上下文(PhysicalDC)*/
		ReleaseDC(phwnd, hdc);

		/*选择相应的缓冲位图为相应DC的画布*/
		SelectObject(gameUIBufferDC, gameUIBufferBmp);
		SelectObject(gameUIMainBufferDC, gameUIMainBufferBmp);
		SelectObject(gameUIInfoBufferDC, gameUIInfoBufferBmp);

		gameUIMainGraphics = new Gdiplus::Graphics(gameUIMainBufferDC);
		gameUIMainGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);

		mainController = new GameController();
		if (!mainController->init(gameUIMainGraphics, 20, 15, 16, 80, 80)) {
			AppGlobalLogE("LRESULT CALLBACK WindowProc_GameUI(): mainController init failed!\n");
			LLKApplicationExit(-1, NULL);
		}
		/*载入成功后先画一次*/
		mainController->draw();


		gameUIInfoGraphics = new Gdiplus::Graphics(gameUIInfoBufferDC);
		gameUIInfoGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);

		infoController = new GameInfoController();
		infoController->init(gameUIInfoGraphics, mainController);
		if (isTimeMode) {
			infoController->setTimingMode(true);
			infoController->setRemainingTime(300);
		}
		/*载入成功后先画一次*/
		infoController->draw();

		GetClientRect(phwnd, &rect);
		SetTimer(phwnd, Timer0, 100, NULL);
		i = 0;
		gameStatus = GAMESTATUS::G_PLAYING;
		break;
	}
	case WM_TIMER: {
		infoController->distributeActionMsg(gameUIMLRBGS);
		if (isTimeMode) {
			i += 70;
			if (i > 1000) {
				infoController->trigger();
				i %= 1000;
			}
		}
		InvalidateRect(phwnd, &rect, true);
		break;
	}
	// 窗口绘制消息
	/*使用WS_EX_LAYERED风格窗口不会对 WM_PAINT 消息进行响应,绘图过程由WM_CREATE状态下的UpdateLayeredWindow()函数进行*/
	case WM_PAINT: {
		PAINTSTRUCT ps;
		GetClientRect(phwnd, &rect);
		/*响应WM_PAINT时必须在内容中添加BeginPaint与EndPaint,否则WM_PAINT会导致无效区域的背景被擦除;本函数返回HDC绘图上下文*/
		HDC hdc = BeginPaint(phwnd, &ps);


		/*这里就不画Map了,不然太影响效率*/
		/*AIMode下还是需要重画一下的,如何判断AI模式呢,手动发送消息时候两个参数分别约定一下即可*/
		if (wParam == 666 && lParam == 999) {
			mainController->draw();
		}
		
		
		/*改进后的新版画法,增加一个MemDC,确保物理DC在一次WM_PAINT中只被绘制一次,实测绘制两次以及两次会发生闪屏*/
		infoController->draw();
		BitBlt(gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIMainBufferDC, 0, 0, SRCCOPY);
		TransparentBlt(gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIInfoBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, RGB(255, 255, 255));
		BitBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIBufferDC, 0, 0, SRCCOPY);

		/*旧版画法,有闪屏*/
		//BitBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIMainBufferDC, 0, 0, SRCCOPY);
		//TransparentBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, RGB(255, 255, 255));

		/*结束绘图,释放绘图区*/
		EndPaint(phwnd, &ps);
		break;
	}
	case WM_MOUSEMOVE: {
		gameUIMLRBGS.mpx = (int)LOWORD(lParam);
		gameUIMLRBGS.mpy = (int)HIWORD(lParam);
		break;
	}
	case WM_LBUTTONDOWN: {
		gameUIMLRBGS.lbs = 1;
		gameUIMLRBGS.ldpx = gameUIMLRBGS.mpx;
		gameUIMLRBGS.ldpy = gameUIMLRBGS.mpy;
		
		break;
	}
	case WM_RBUTTONDOWN: {
		gameUIMLRBGS.rbs = 1;
		gameUIMLRBGS.rdpx = gameUIMLRBGS.mpx;
		gameUIMLRBGS.rdpy = gameUIMLRBGS.mpy;
		
		break;
	}
	case WM_LBUTTONUP: {
		gameUIMLRBGS.lbs = 0;
		gameUIMLRBGS.lupx = gameUIMLRBGS.mpx;
		gameUIMLRBGS.lupy = gameUIMLRBGS.mpy;
		if (AIModeflag) {
			mainController->actionProc(GC_MOUSELBUP, &AIModeClickX, &AIModeClickY);
		}
		else {
			mainController->actionProc(GC_MOUSELBUP, &gameUIMLRBGS.lupx, &gameUIMLRBGS.lupy);
		}
		
		break;
	}
	case WM_RBUTTONUP: {
		gameUIMLRBGS.rbs = 0;
		gameUIMLRBGS.rupx = gameUIMLRBGS.mpx;
		gameUIMLRBGS.rupy = gameUIMLRBGS.mpy;
		break;
	}
	case WM_CLOSE:
		DestroyWindow(phwnd);
		break;
	case WM_DESTROY:
		DeleteObject(gameUIMainBufferBmp);
		DeleteObject(gameUIBufferBmp);
		ReleaseDC(phwnd, gameUIMainBufferDC);
		ReleaseDC(phwnd, gameUIBufferDC);
		KillTimer(phwnd, Timer0);
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(phwnd, msg, wParam, lParam);
	}
	return 0;
}

void RegisterWindow_GameUI(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc_GameUI; // 设置第四步的窗口过程回调函数
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = gameUIRegClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "窗口注册失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
}
HWND CreateWindow_GameUI(HINSTANCE hInstance, int nCmdShow) {
	gameUIhWnd = CreateWindow(
		gameUIRegClassName,
		gameUITitle,
		WS_OVERLAPPEDWINDOW,
		gameUIWGA.X, gameUIWGA.Y, gameUIWGA.nWidth, gameUIWGA.nHeight, // 出现坐标 x,y,宽度 高度 
		NULL, NULL, hInstance, NULL);
	if (gameUIhWnd == NULL)
	{
		MessageBox(NULL, "窗口创建失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
	ShowWindow(gameUIhWnd, SW_SHOW);
	UpdateWindow(gameUIhWnd);
	return gameUIhWnd;
}