#include "SplashUI.h"
#include "MainUI.h"
#include "..\WindowUtil.h"

//bool volatile bThreadOn = true;
void _cdecl wndSwitch(PVOID param);
void _cdecl wndSwitch(PVOID param) {
	//while (time(0) < cTime + INIT_DELAY_TIME);
	Sleep(SplashWaitTime * 1000);
	PostMessage(splashUIhWnd, WM_CLOSE, 0, 0);
}

// �����ڻ�ͼ����
void onDraw() {

}

// ���ڴ�����Ϣ����
LRESULT CALLBACK WindowProc_SplashUI(HWND phwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CREATE:
	{
		splashUIhWnd = phwnd;
		Image image(SplashImagePath);
		//���ش���ͼ��
		int iWidth = image.GetWidth();
		int iHeight = image.GetHeight();
		HDC hdcScreen = GetDC(splashUIhWnd);
		HDC hdcMem = CreateCompatibleDC(hdcScreen);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
		HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
		Graphics gp(hdcMem);
		//�����ｨ�����ڵ�ͼ��,�����ô���������
		//GraphicsPath graphicspath;
		//SolidBrush brush(Color(255, 175, 0, 0));
		//CreateRoundRectangle(Rect(0, 0, iWidth, iHeight), 20, &graphicspath);//����Բ�Ǿ���·��
		//gp.FillPath(&brush, &graphicspath);//���·��----ȥ��ע�Ͳ鿴Ч��
		gp.DrawImage(&image, 0, 0, 1000, 625);//��pngͼ����Ƶ���̨DC��
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;//��ͨ�����
		POINT	pPos = { 460, 188 };
		POINT	pSrc = { 0, 0 };
		SIZE	sizeWnd = { iWidth, iHeight };
		//ʹ��UpdateLayeredWindow()�������·ֲ㴰��
		UpdateLayeredWindow(splashUIhWnd, hdcScreen, &pPos, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);
		//��β������
		SelectObject(hdcMem, hBitmapOld);
		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		ReleaseDC(splashUIhWnd, hdcScreen);

		/*������һ���߳�,������ʱ�ȴ�һ��ʱ��ر�����ҳ��,�Զ���ת����һ������*/
		_beginthread(wndSwitch, 0, NULL);
		/*CreateThread�����̲߳�֪��Ϊʲô��ʧ��,����_beginthread����*/
		//hTimer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wndSwitch(NULL), NULL, 0, NULL);
		//CloseHandle(hTimer);

		/*
		HMENU menubar = CreateMenu();
		HMENU menuPop = CreatePopupMenu();
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_SOLID, L"ʵ��");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DASH, L"����");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DOT, L"����");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DASHDOT, L"������");
		AppendMenu(menubar, MF_STRING | MF_POPUP, (UINT_PTR)menuPop, L"ѡ������");
		SetMenu(hwnd, menubar);
		*/

	}
	// ���ڻ�����Ϣ
	/*ʹ��WS_EX_LAYERED��񴰿ڲ���� WM_PAINT ��Ϣ������Ӧ,��ͼ������WM_CREATE״̬�µ�UpdateLayeredWindow()��������*/
	case WM_PAINT:
		/*
		* ����ֻ��Ҫ������������ǵ� GDI ���ƺ����Ϳ��ԣ������ط�����������
		*/
		onDraw();
		break;
		// ���ڹر���Ϣ
	case WM_CLOSE:
		DestroyWindow(splashUIhWnd);
		break;
		// ����������Ϣ
	case WM_DESTROY:
		//CloseHandle(hTimer);
		CreateWindow_MainUI(initInstance, initnCmdShow);
		break;
	case WM_SIZE:
	{
		break;
	}
	//set_window_size(hwnd, 1000, 625);
	// ������Ϣ
	default:
		// pass ��ϵͳ���۲���
		return DefWindowProc(phwnd, msg, wParam, lParam);
	}
	return 0;
}

// ע�ᴰ����
void RegisterWindow_SplashUI(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	// 1)���ô�������
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc_SplashUI; // ���õ��Ĳ��Ĵ��ڹ��̻ص�����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = splashUIRegClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// 2)ע��
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "����ע��ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0); // �����˳�
	}
}

/*��������*/
HWND CreateWindow_SplashUI(HINSTANCE hInstance, int nCmdShow)
{
	splashUIhWnd = CreateWindowEx(
		WS_EX_LAYERED,
		splashUIRegClassName,
		splashUITitle,
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		splashUIWGA.X, splashUIWGA.Y, splashUIWGA.nWidth, splashUIWGA.nHeight, // �������� x,y,��� �߶� 
		NULL, NULL, hInstance, NULL);

	if (splashUIhWnd == NULL)
	{
		MessageBox(NULL, "���ڴ���ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0); // �����˳�
	}
	// ��ʾ����
	ShowWindow(splashUIhWnd, SW_SHOW);
	UpdateWindow(splashUIhWnd);
	return splashUIhWnd;
}
