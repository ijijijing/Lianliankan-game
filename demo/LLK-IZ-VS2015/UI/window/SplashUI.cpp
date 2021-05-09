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

// 本窗口绘图方法
void onDraw() {

}

// 窗口处理消息过程
LRESULT CALLBACK WindowProc_SplashUI(HWND phwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CREATE:
	{
		splashUIhWnd = phwnd;
		Image image(SplashImagePath);
		//加载窗口图形
		int iWidth = image.GetWidth();
		int iHeight = image.GetHeight();
		HDC hdcScreen = GetDC(splashUIhWnd);
		HDC hdcMem = CreateCompatibleDC(hdcScreen);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
		HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
		Graphics gp(hdcMem);
		//在这里建立窗口的图形,可以用代码来生成
		//GraphicsPath graphicspath;
		//SolidBrush brush(Color(255, 175, 0, 0));
		//CreateRoundRectangle(Rect(0, 0, iWidth, iHeight), 20, &graphicspath);//创建圆角矩形路径
		//gp.FillPath(&brush, &graphicspath);//填充路径----去掉注释查看效果
		gp.DrawImage(&image, 0, 0, 1000, 625);//将png图像绘制到后台DC中
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;//按通道混合
		POINT	pPos = { 460, 188 };
		POINT	pSrc = { 0, 0 };
		SIZE	sizeWnd = { iWidth, iHeight };
		//使用UpdateLayeredWindow()函数更新分层窗口
		UpdateLayeredWindow(splashUIhWnd, hdcScreen, &pPos, &sizeWnd, hdcMem, &pSrc, NULL, &blend, ULW_ALPHA);
		//收尾清理工作
		SelectObject(hdcMem, hBitmapOld);
		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		ReleaseDC(splashUIhWnd, hdcScreen);

		/*开启另一个线程,用来计时等待一段时间关闭启动页面,自动跳转到下一个窗口*/
		_beginthread(wndSwitch, 0, NULL);
		/*CreateThread创建线程不知道为什么老失败,换用_beginthread代替*/
		//hTimer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wndSwitch(NULL), NULL, 0, NULL);
		//CloseHandle(hTimer);

		/*
		HMENU menubar = CreateMenu();
		HMENU menuPop = CreatePopupMenu();
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_SOLID, L"实线");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DASH, L"虚线");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DOT, L"点线");
		AppendMenu(menuPop, MF_STRING, (UINT_PTR)PS_DASHDOT, L"点虚线");
		AppendMenu(menubar, MF_STRING | MF_POPUP, (UINT_PTR)menuPop, L"选择线型");
		SetMenu(hwnd, menubar);
		*/

	}
	// 窗口绘制消息
	/*使用WS_EX_LAYERED风格窗口不会对 WM_PAINT 消息进行响应,绘图过程由WM_CREATE状态下的UpdateLayeredWindow()函数进行*/
	case WM_PAINT:
		/*
		* 我们只需要在这里调用我们的 GDI 绘制函数就可以，其他地方可以先无视
		*/
		onDraw();
		break;
		// 窗口关闭消息
	case WM_CLOSE:
		DestroyWindow(splashUIhWnd);
		break;
		// 窗口销毁消息
	case WM_DESTROY:
		//CloseHandle(hTimer);
		CreateWindow_MainUI(initInstance, initnCmdShow);
		break;
	case WM_SIZE:
	{
		break;
	}
	//set_window_size(hwnd, 1000, 625);
	// 其他消息
	default:
		// pass 给系统，咱不管
		return DefWindowProc(phwnd, msg, wParam, lParam);
	}
	return 0;
}

// 注册窗口类
void RegisterWindow_SplashUI(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	// 1)配置窗口属性
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc_SplashUI; // 设置第四步的窗口过程回调函数
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = splashUIRegClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// 2)注册
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "窗口注册失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
		exit(0); // 进程退出
	}
}

/*创建窗口*/
HWND CreateWindow_SplashUI(HINSTANCE hInstance, int nCmdShow)
{
	splashUIhWnd = CreateWindowEx(
		WS_EX_LAYERED,
		splashUIRegClassName,
		splashUITitle,
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		splashUIWGA.X, splashUIWGA.Y, splashUIWGA.nWidth, splashUIWGA.nHeight, // 出现坐标 x,y,宽度 高度 
		NULL, NULL, hInstance, NULL);

	if (splashUIhWnd == NULL)
	{
		MessageBox(NULL, "窗口创建失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
		exit(0); // 进程退出
	}
	// 显示窗口
	ShowWindow(splashUIhWnd, SW_SHOW);
	UpdateWindow(splashUIhWnd);
	return splashUIhWnd;
}
