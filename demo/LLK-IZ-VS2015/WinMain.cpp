#include "UI\WindowSummary.h"
#include "UI\WindowUtil.h"

#include <locale.h>

int main() {
	setlocale(LC_ALL, "");
	std::cout << "main() func called,start logging......\n";
	return _tWinMain(GetModuleHandle(NULL), NULL, NULL, NULL);

}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*初始化相关数据*/
	initInstance = hInstance;
	inithPrevInstance = hPrevInstance;
	initlpCmdLine = lpCmdLine;
	initnCmdShow = nCmdShow;
	/*禁用WindowsDPI缩放,防止图片和文字模糊,该函数仅能在win8及以上系统中使用*/
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	FullScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	FullScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);


	ULONG_PTR GdiToken;
	GdiplusStartupInput	gin;
	GdiplusStartup(&GdiToken, &gin, NULL);


	// 第一步,注册窗口类
	RegisterWindow_GameUI(hInstance);
	RegisterWindow_MainUI(hInstance);
	//RegisterWindow_RankingUI(hInstance);
	RegisterWindow_SplashUI(hInstance);

	// 第二步:创建窗口
	HWND hWnd = CreateWindow_SplashUI(hInstance, nCmdShow);
	// 第三步:消息循环
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	GdiplusShutdown(GdiToken);
	return Msg.wParam;
}
