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
	/*��ʼ���������*/
	initInstance = hInstance;
	inithPrevInstance = hPrevInstance;
	initlpCmdLine = lpCmdLine;
	initnCmdShow = nCmdShow;
	/*����WindowsDPI����,��ֹͼƬ������ģ��,�ú���������win8������ϵͳ��ʹ��*/
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	FullScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	FullScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);


	ULONG_PTR GdiToken;
	GdiplusStartupInput	gin;
	GdiplusStartup(&GdiToken, &gin, NULL);


	// ��һ��,ע�ᴰ����
	RegisterWindow_GameUI(hInstance);
	RegisterWindow_MainUI(hInstance);
	//RegisterWindow_RankingUI(hInstance);
	RegisterWindow_SplashUI(hInstance);

	// �ڶ���:��������
	HWND hWnd = CreateWindow_SplashUI(hInstance, nCmdShow);
	// ������:��Ϣѭ��
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	GdiplusShutdown(GdiToken);
	return Msg.wParam;
}
