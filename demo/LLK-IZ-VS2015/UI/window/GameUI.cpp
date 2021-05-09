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




// ���ڴ�����Ϣ����(�ص�����)
LRESULT CALLBACK WindowProc_GameUI(HWND phwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

		/*
		WM_CREATE �ڴ�������(����CreateWindow����)ʱ������ֻ����һ�Ρ�
		WM_PAINT����Ҫ�ػ�(����UpdateWindow����)ʱ���ã�����δ�����
		WM_CREATE����WM_PAINT
		*/
	case WM_CREATE:
	{
		GetClientRect(phwnd, &rect);
		setWindowSize(phwnd, gameUIWGA.nWidth, gameUIWGA.nHeight);
		memset(&gameUIMLRBGS, 0, sizeof(gameUIMLRBGS));
		gameUICxClientPix = gameUIWGA.nWidth;
		gameUICyClientPix = gameUIWGA.nHeight;
		/*��ȡ�����豸������(PhysicalDC)*/
		HDC hdc = GetDC(phwnd);

		/*���������ڴ�DC*/
		gameUIBufferDC = CreateCompatibleDC(hdc);
		/*���������ڴ�DC(���ڻ�����Map)*/
		gameUIMainBufferDC = CreateCompatibleDC(hdc);
		/*���������ڴ�DC(���ڻ��ư�ť,�������ȿؼ��Լ�����·��)*/
		gameUIInfoBufferDC = CreateCompatibleDC(hdc);

		/*��������gameUICxClientPix, gameUICyClientPix��С������ӦDC�豸�����Ļ���λͼ,ע�������ԴDCӦѡ������DC,����ѡ���ڴ�DC*/
		gameUIBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);
		gameUIMainBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);
		gameUIInfoBufferBmp = CreateCompatibleBitmap(hdc, gameUICxClientPix, gameUICyClientPix);

		/*�ͷ������豸������(PhysicalDC)*/
		ReleaseDC(phwnd, hdc);

		/*ѡ����Ӧ�Ļ���λͼΪ��ӦDC�Ļ���*/
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
		/*����ɹ����Ȼ�һ��*/
		mainController->draw();


		gameUIInfoGraphics = new Gdiplus::Graphics(gameUIInfoBufferDC);
		gameUIInfoGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);

		infoController = new GameInfoController();
		infoController->init(gameUIInfoGraphics, mainController);
		if (isTimeMode) {
			infoController->setTimingMode(true);
			infoController->setRemainingTime(300);
		}
		/*����ɹ����Ȼ�һ��*/
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
	// ���ڻ�����Ϣ
	/*ʹ��WS_EX_LAYERED��񴰿ڲ���� WM_PAINT ��Ϣ������Ӧ,��ͼ������WM_CREATE״̬�µ�UpdateLayeredWindow()��������*/
	case WM_PAINT: {
		PAINTSTRUCT ps;
		GetClientRect(phwnd, &rect);
		/*��ӦWM_PAINTʱ���������������BeginPaint��EndPaint,����WM_PAINT�ᵼ����Ч����ı���������;����������HDC��ͼ������*/
		HDC hdc = BeginPaint(phwnd, &ps);


		/*����Ͳ���Map��,��Ȼ̫Ӱ��Ч��*/
		/*AIMode�»�����Ҫ�ػ�һ�µ�,����ж�AIģʽ��,�ֶ�������Ϣʱ�����������ֱ�Լ��һ�¼���*/
		if (wParam == 666 && lParam == 999) {
			mainController->draw();
		}
		
		
		/*�Ľ�����°滭��,����һ��MemDC,ȷ������DC��һ��WM_PAINT��ֻ������һ��,ʵ����������Լ����λᷢ������*/
		infoController->draw();
		BitBlt(gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIMainBufferDC, 0, 0, SRCCOPY);
		TransparentBlt(gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIInfoBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, RGB(255, 255, 255));
		BitBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIBufferDC, 0, 0, SRCCOPY);

		/*�ɰ滭��,������*/
		//BitBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIMainBufferDC, 0, 0, SRCCOPY);
		//TransparentBlt(hdc, 0, 0, gameUICxClientPix, gameUICyClientPix, gameUIBufferDC, 0, 0, gameUICxClientPix, gameUICyClientPix, RGB(255, 255, 255));

		/*������ͼ,�ͷŻ�ͼ��*/
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
	wc.lpfnWndProc = WindowProc_GameUI; // ���õ��Ĳ��Ĵ��ڹ��̻ص�����
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
		MessageBox(NULL, "����ע��ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
}
HWND CreateWindow_GameUI(HINSTANCE hInstance, int nCmdShow) {
	gameUIhWnd = CreateWindow(
		gameUIRegClassName,
		gameUITitle,
		WS_OVERLAPPEDWINDOW,
		gameUIWGA.X, gameUIWGA.Y, gameUIWGA.nWidth, gameUIWGA.nHeight, // �������� x,y,��� �߶� 
		NULL, NULL, hInstance, NULL);
	if (gameUIhWnd == NULL)
	{
		MessageBox(NULL, "���ڴ���ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
	ShowWindow(gameUIhWnd, SW_SHOW);
	UpdateWindow(gameUIhWnd);
	return gameUIhWnd;
}