#include <vector>
#include "MainUI.h"
#include "GameUI.h"
#include "RankingUI.h"
#include "..\MouseUtil.h"
#include "..\WindowUtil.h"
#include "..\font\XFonts.h"
#include "..\comctl\text\Text.h"
#include "..\comctl\button\Button.h"

static HBITMAP mainUIBufferBmp = NULL;
static HDC mainUIBufferDC = NULL, mainUIDrawDC = NULL;
static int mainUICxClientPix, mainUICyClientPix;
static MouseLRBtnGeoStatus mainUIMLRBGS;
static Gdiplus::Graphics* mainUIGraphics = NULL;
static RECT mainUIRECT;
static UINT hTimer0 = 1;
/*ͼƬ��Դ·��*/
static LPCWSTR mainUIImagePath_background = L"res\\mainUIBgd.bmp";
static LPCWSTR mainUIImagePath_buttonBackground = L"res\\mainUIBtnBgd.png";

/*Gdiplus::Image��Ҫ��GDI::GdiplusStartup()��ſ�������ͼ��,������ӦWM_CREATE��Ϣʱ��̬����,��ʹ�þ�̬����*/
static Gdiplus::Image* mainUIImage_background = NULL;
static Gdiplus::Image* mainUIImage_buttonBackground = NULL;

/*��ť*/
static Button* mainUIButton_start;
static Button* mainUIButton_rankingList;
static Button* mainUIButton_help;
static Button* mainUIButton_exit;
static Button* mainUIButton_copyrightInfo;
static Button* mainUIButton_leisureMode;
static Button* mainUIButton_timingMode;
/*��ť�б�*/
static std::vector<Button*> buttons(7);
static bool exitFlag = 0;
void jumpToRanking() {
	PostMessage(mainUIhWnd, WM_CLOSE, 0, 0);
	//rankingUIhWnd = CreateWindow_RankingUI(initInstance, initnCmdShow);
}
void jumpToGame() {
	PostMessage(mainUIhWnd, WM_CLOSE, 0, 0);
	gameUIhWnd = CreateWindow_GameUI(initInstance, initnCmdShow);
}
void jumpToHelp() {
	static LPCSTR HelpUrl = "https://www.baidu.com/s?wd=%E8%BF%9E%E8%BF%9E%E7%9C%8B%E6%80%8E%E4%B9%88%E7%8E%A9";
	AppGlobalLogI("void jumpToHelp(): open url: %s\n", HelpUrl);
	ShellExecute(NULL, "open", HelpUrl, NULL, NULL, SW_SHOWNORMAL);
}
void jumpToExit() {
	exitFlag = 1;
	PostMessage(mainUIhWnd, WM_CLOSE, 0, 0);
}
void jumpToP() {
	AppGlobalLogI("\u76ae\u8fd9\u4e00\u4e0b\u5f88\u5f00\u5fc3\u0032\u0033\u0033\u0033\u0033\n");
}
void jumpToTimingMode() {
	isTimeMode = true;
	jumpToGame();
}
void jumpToLeisureMode() {
	isTimeMode = false;
	jumpToGame();
}

/*��궯����Ϣ�ַ�*/
void mainUIDistributeMouseMessage() {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i] == NULL) {
			//AppGlobalLogE("void mainUIDistributeMouseMessage(): buttons[i] is NULL Ptr, i=%d\n",i);
			continue;
		}
		buttons[i]->onMouseEventProcess(&mainUIMLRBGS);
	}
}

/*������Դ:ͼƬ,�ı�,��ť��*/
void loadRes() {

	/*����ͼƬ*/
	mainUIImage_background = new Gdiplus::Image(mainUIImagePath_background);
	mainUIImage_buttonBackground = new Gdiplus::Image(mainUIImagePath_buttonBackground);

	/*���밴ť������*/
	mainUIButton_start = new Button();
	mainUIButton_start->setGDIGraphics(mainUIGraphics);
	mainUIButton_start->setText(L"��ʼ");
	mainUIButton_start->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_start->setSize(160, 50);
	mainUIButton_start->setPosi(685, 210);
	mainUIButton_start->setTextColor(200, 10, 10, 10);
	mainUIButton_start->setFontSize(30);
	mainUIButton_start->setFontName(L"΢���ź�");
	mainUIButton_start->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_start->enableDisplayBorder(true);
	mainUIButton_start->setBorderColor(240, 250, 250, 250);
	mainUIButton_start->setBorderStroke(1.5f);
	mainUIButton_start->setBorderRectRadius(20);
	mainUIButton_start->setShapeType(BorderType::BROUNDRECT);
	mainUIButton_start->enableDisplayBackground(true);
	mainUIButton_start->setPressedColor(240, 0, 204, 238);
	mainUIButton_start->setFlittedColor(175, 0, 204, 238);
	mainUIButton_start->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_start->setPressedTextColor(250, 255, 255, 255);
	mainUIButton_start->setFlittedTextColor(240, 255, 255, 255);
	mainUIButton_start->setOnClickCallBack(jumpToGame);
	buttons.push_back(mainUIButton_start);

	mainUIButton_rankingList = new Button();
	mainUIButton_rankingList->setGDIGraphics(mainUIGraphics);
	mainUIButton_rankingList->setText(L"���а�");
	mainUIButton_rankingList->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_rankingList->setSize(160, 50);
	mainUIButton_rankingList->setPosi(685, 290);
	mainUIButton_rankingList->setTextColor(200, 10, 10, 10);
	mainUIButton_rankingList->setFontSize(30);
	mainUIButton_rankingList->setFontName(L"΢���ź�");
	mainUIButton_rankingList->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_rankingList->enableDisplayBorder(true);
	mainUIButton_rankingList->setBorderColor(240, 250, 250, 250);
	mainUIButton_rankingList->setBorderStroke(1.5f);
	mainUIButton_rankingList->setBorderRectRadius(20);
	mainUIButton_rankingList->setShapeType(BorderType::BROUNDRECT);
	mainUIButton_rankingList->enableDisplayBackground(true);
	mainUIButton_rankingList->setPressedColor(240, 97, 65, 178);
	mainUIButton_rankingList->setFlittedColor(175, 97, 65, 178);
	mainUIButton_rankingList->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_rankingList->setPressedTextColor(250, 255, 255, 255);
	mainUIButton_rankingList->setFlittedTextColor(240, 255, 255, 255);
	mainUIButton_rankingList->setOnClickCallBack(jumpToRanking);
	buttons.push_back(mainUIButton_rankingList);

	mainUIButton_help = new Button();
	mainUIButton_help->setGDIGraphics(mainUIGraphics);
	mainUIButton_help->setText(L"����");
	mainUIButton_help->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_help->setSize(160, 50);
	mainUIButton_help->setPosi(685, 370);
	mainUIButton_help->setTextColor(200, 10, 10, 10);
	mainUIButton_help->setFontSize(30);
	mainUIButton_help->setFontName(L"΢���ź�");
	mainUIButton_help->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_help->enableDisplayBorder(true);
	mainUIButton_help->setBorderColor(240, 250, 250, 250);
	mainUIButton_help->setBorderStroke(1.5f);
	mainUIButton_help->setBorderRectRadius(20);
	mainUIButton_help->setShapeType(BorderType::BROUNDRECT);
	mainUIButton_help->enableDisplayBackground(true);
	mainUIButton_help->setPressedColor(240, 255, 153, 0);
	mainUIButton_help->setFlittedColor(175, 255, 153, 0);
	mainUIButton_help->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_help->setPressedTextColor(250, 255, 255, 255);
	mainUIButton_help->setFlittedTextColor(240, 255, 255, 255);
	mainUIButton_help->setOnClickCallBack(jumpToHelp);
	buttons.push_back(mainUIButton_help);

	mainUIButton_exit = new Button();
	mainUIButton_exit->setGDIGraphics(mainUIGraphics);
	mainUIButton_exit->setText(L"�˳�");
	mainUIButton_exit->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_exit->setSize(160, 50);
	mainUIButton_exit->setPosi(685, 450);
	mainUIButton_exit->setTextColor(200, 10, 10, 10);
	mainUIButton_exit->setFontSize(30);
	mainUIButton_exit->setFontName(L"΢���ź�");
	mainUIButton_exit->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_exit->enableDisplayBorder(false);
	mainUIButton_exit->setBorderColor(240, 250, 250, 250);
	mainUIButton_exit->setBorderStroke(1.5f);
	mainUIButton_exit->setBorderRectRadius(20);
	mainUIButton_exit->setShapeType(BorderType::BROUNDRECT);
	mainUIButton_exit->enableDisplayBackground(true);
	mainUIButton_exit->setPressedColor(240, 255, 1, 1);
	mainUIButton_exit->setFlittedColor(175, 255, 1, 1);
	mainUIButton_exit->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_exit->setPressedTextColor(250, 255, 255, 255);
	mainUIButton_exit->setFlittedTextColor(240, 255, 255, 255);
	mainUIButton_exit->setOnClickCallBack(jumpToExit);
	buttons.push_back(mainUIButton_exit);

	mainUIButton_leisureMode = new Button();
	mainUIButton_leisureMode->setGDIGraphics(mainUIGraphics);
	mainUIButton_leisureMode->setText(L"����ģʽ");
	mainUIButton_leisureMode->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_leisureMode->setSize(200, 200);
	mainUIButton_leisureMode->setPosi(200, 280);
	mainUIButton_leisureMode->setTextColor(200, 10, 10, 10);
	mainUIButton_leisureMode->setFontSize(43);
	mainUIButton_leisureMode->setFontName(L"���������");
	mainUIButton_leisureMode->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_leisureMode->enableDisplayBorder(false);
	mainUIButton_leisureMode->setBorderColor(240, 250, 250, 250);
	mainUIButton_leisureMode->setBorderStroke(1.5f);
	mainUIButton_leisureMode->setBorderRectRadius(20);
	mainUIButton_leisureMode->setShapeType(BorderType::BROUND);
	mainUIButton_leisureMode->enableDisplayBackground(true);
	mainUIButton_leisureMode->setPressedColor(170, 255,255,255);
	mainUIButton_leisureMode->setFlittedColor(100, 255, 255, 255);
	mainUIButton_leisureMode->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_leisureMode->setPressedTextColor(250, 59, 132, 96);
	mainUIButton_leisureMode->setFlittedTextColor(240, 59, 132, 96);
	mainUIButton_leisureMode->setOnClickCallBack(jumpToLeisureMode);
	buttons.push_back(mainUIButton_leisureMode);

	mainUIButton_timingMode = new Button();
	mainUIButton_timingMode->setGDIGraphics(mainUIGraphics);
	mainUIButton_timingMode->setText(L"��ʱģʽ");
	mainUIButton_timingMode->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_timingMode->setSize(134, 134);
	mainUIButton_timingMode->setPosi(412, 112);
	mainUIButton_timingMode->setTextColor(200, 10, 10, 10);
	mainUIButton_timingMode->setFontSize(24);
	mainUIButton_timingMode->setFontName(L"���Ĳ���");
	mainUIButton_timingMode->setFontStyle(FontStyle::FontStyleBold);
	mainUIButton_timingMode->enableDisplayBorder(false);
	mainUIButton_timingMode->setBorderColor(240, 250, 250, 250);
	mainUIButton_timingMode->setBorderStroke(1.5f);
	mainUIButton_timingMode->setBorderRectRadius(20);
	mainUIButton_timingMode->setShapeType(BorderType::BROUND);
	mainUIButton_timingMode->enableDisplayBackground(true);
	mainUIButton_timingMode->setPressedColor(200, 15, 200, 30);
	mainUIButton_timingMode->setFlittedColor(150, 15, 200, 30);
	mainUIButton_timingMode->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_timingMode->setPressedTextColor(250, 240, 240, 240);
	mainUIButton_timingMode->setFlittedTextColor(240, 59, 132, 96);
	mainUIButton_timingMode->setOnClickCallBack(jumpToTimingMode);
	buttons.push_back(mainUIButton_timingMode);

	mainUIButton_copyrightInfo = new Button();
	mainUIButton_copyrightInfo->setGDIGraphics(mainUIGraphics);
	mainUIButton_copyrightInfo->setText(L"Copyright initializeZero\u00AE 2016~2026. Powered & Designed by initializeZero(EE1604 ywt)\n���Ʋ�����Ϸ���ܾ�������Ϸ��ע�����ұ�����������ƭ�ϵ����ʶ���Ϸ���ԣ�������Ϸ����������ʱ�䣬���ܽ�������\nʹ��ǰ�����Ķ���ͬ��\u300A�û�Э��\u300B");
	mainUIButton_copyrightInfo->setStringAlignment(StringAlignment::StringAlignmentCenter);
	mainUIButton_copyrightInfo->setSize(mainUIWGA.nWidth, 65);
	mainUIButton_copyrightInfo->setPosi(0, 575);
	mainUIButton_copyrightInfo->setTextColor(200, 10, 10, 10);
	mainUIButton_copyrightInfo->setFontSize(14);
	mainUIButton_copyrightInfo->setFontName(L"΢���ź�");
	mainUIButton_copyrightInfo->setFontStyle(FontStyle::FontStyleRegular);
	mainUIButton_copyrightInfo->enableDisplayBorder(false);
	mainUIButton_copyrightInfo->setBorderColor(240, 250, 250, 250);
	mainUIButton_copyrightInfo->setBorderStroke(1.5f);
	mainUIButton_copyrightInfo->setBorderRectRadius(20);
	mainUIButton_copyrightInfo->setShapeType(BorderType::BRECT);
	mainUIButton_copyrightInfo->enableDisplayBackground(true);
	mainUIButton_copyrightInfo->setPressedColor(230, 255, 255, 255);
	mainUIButton_copyrightInfo->setFlittedColor(200, 255, 255, 255);
	mainUIButton_copyrightInfo->setBackgroundColor(150, 255, 255, 255);
	mainUIButton_copyrightInfo->setPressedTextColor(251, 40, 40, 40);
	mainUIButton_copyrightInfo->setFlittedTextColor(200, 40, 40, 40);
	mainUIButton_copyrightInfo->setOnClickCallBack(jumpToP);
	buttons.push_back(mainUIButton_copyrightInfo);

	LLKInfLog("BmpResLoaded!\n");
}

void drawMainUIImage() {
	mainUIGraphics->DrawImage(mainUIImage_background, 0, 0, mainUIImage_background->GetWidth(), mainUIImage_background->GetHeight());
	mainUIGraphics->DrawImage(mainUIImage_buttonBackground, 180, 260, mainUIImage_buttonBackground->GetWidth() / 2, mainUIImage_buttonBackground->GetHeight() / 2);
	mainUIGraphics->DrawImage(mainUIImage_buttonBackground, 400, 100, mainUIImage_buttonBackground->GetWidth() / 3-1, mainUIImage_buttonBackground->GetHeight() / 3-1);
	
}



void drawMainUIText() {

}

void drawMainUI() {
	drawMainUIImage();
	drawMainUIText();
	mainUIButton_start->draw();
	mainUIButton_rankingList->draw();
	mainUIButton_help->draw();
	mainUIButton_exit->draw();
	mainUIButton_leisureMode->draw();
	mainUIButton_timingMode->draw();
	mainUIButton_copyrightInfo->draw();
	//test->draw();
}

// ���ڴ�����Ϣ����(�ص�����)
LRESULT CALLBACK WindowProc_MainUI(HWND phwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		setWindowSize(phwnd, mainUIWGA.nWidth, mainUIWGA.nHeight);
		memset(&mainUIMLRBGS, 0, sizeof(mainUIMLRBGS));
		mainUICxClientPix = mainUIWGA.nWidth;
		mainUICyClientPix = mainUIWGA.nHeight;
		/*��ȡ�����豸������(PhysicalDC)*/
		HDC hdc = GetDC(phwnd);
		/*���������ڴ�DC*/
		mainUIBufferDC = CreateCompatibleDC(hdc);
		/*���������ڴ�DC(���ڻ���)*/
		mainUIDrawDC = CreateCompatibleDC(hdc);
		/*����һ��mainUICxClientPix, mainUICyClientPix��С������ӦDC�豸�����Ļ���λͼ,ע�������ԴDCӦѡ������DC,����ѡ���ڴ�DC*/
		mainUIBufferBmp = CreateCompatibleBitmap(hdc, mainUICxClientPix, mainUICyClientPix);
		/*�ͷ������豸������(PhysicalDC)*/
		ReleaseDC(phwnd, hdc);
		/*ѡ��mainUIBufferBmp����λͼΪmainUIBufferDC�Ļ���*/
		SelectObject(mainUIBufferDC, mainUIBufferBmp);
		/*����GDIGraphicsObject*/
		mainUIGraphics = new Gdiplus::Graphics(mainUIBufferDC);
		/*������������ͼ*/
		mainUIGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
		/*��̬������Դ*/
		loadRes();
		/*
		break������Ҫ!!!
		break������Ҫ!!!
		break������Ҫ!!!
		��Ҫ�Ļ�˵����
		*/
		GetClientRect(phwnd, &mainUIRECT);
		SetTimer(phwnd, hTimer0, 50, NULL);
		break;
	}
	// ���ڻ�����Ϣ
	/*ʹ��WS_EX_LAYERED��񴰿ڲ���� WM_PAINT ��Ϣ������Ӧ,��ͼ������WM_CREATE״̬�µ�UpdateLayeredWindow()��������*/
	case WM_PAINT: {
		PAINTSTRUCT ps;
		/*��ӦWM_PAINTʱ���������������BeginPaint��EndPaint,����WM_PAINT�ᵼ����Ч����ı���������;����������HDC��ͼ������*/
		HDC hdc = BeginPaint(phwnd, &ps);
		drawMainUI();
		BitBlt(hdc, 0, 0, mainUICxClientPix, mainUICyClientPix, mainUIBufferDC, 0, 0, SRCCOPY);
		
		/*������ͼ,�ͷŻ�ͼ��*/
		EndPaint(phwnd, &ps);
		break;
	}
	case WM_TIMER: {
		mainUIDistributeMouseMessage();
		InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
	case WM_MOUSEMOVE: {
		//mainUIMLRBGS.lbs = 1;
		mainUIMLRBGS.mpx = (int)LOWORD(lParam);
		mainUIMLRBGS.mpy = (int)HIWORD(lParam);
		mainUIMLRBGS.ldpx = mainUIMLRBGS.mpx;
		mainUIMLRBGS.ldpy = mainUIMLRBGS.mpy;
		
		//mainUIDistributeMouseMessage();
		//InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
	case WM_LBUTTONDOWN: {
		mainUIMLRBGS.prelbs = mainUIMLRBGS.lbs;
		mainUIMLRBGS.lbs = 1;
		mainUIMLRBGS.ldpx = mainUIMLRBGS.mpx;
		mainUIMLRBGS.ldpy = mainUIMLRBGS.mpy;
		//InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
	case WM_RBUTTONDOWN: {
		mainUIMLRBGS.prerbs = mainUIMLRBGS.rbs;
		mainUIMLRBGS.rbs = 1;
		mainUIMLRBGS.rdpx = mainUIMLRBGS.mpx;
		mainUIMLRBGS.rdpy = mainUIMLRBGS.mpy;
		//InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
	case WM_LBUTTONUP: {
		mainUIMLRBGS.prelbs = mainUIMLRBGS.lbs;
		mainUIMLRBGS.lbs = 0;
		mainUIMLRBGS.lupx = mainUIMLRBGS.mpx;
		mainUIMLRBGS.lupy = mainUIMLRBGS.mpy;
		//InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
	case WM_RBUTTONUP: {
		mainUIMLRBGS.prerbs = mainUIMLRBGS.rbs;
		mainUIMLRBGS.rbs = 0;
		mainUIMLRBGS.rupx = mainUIMLRBGS.mpx;
		mainUIMLRBGS.rupy = mainUIMLRBGS.mpy;
		//InvalidateRect(phwnd, &mainUIRECT, true);
		break;
	}
				   
	case WM_COMMAND:
	{

		//���û������ؼ�(���,����)���¼�������Ӧ
		break;
	}
	case WM_CLOSE:
		DestroyWindow(phwnd);
		break;
	case WM_DESTROY: {
		if (mainUIGraphics != NULL) {
			delete mainUIGraphics;
			mainUIGraphics = NULL;
		}
		DeleteObject(mainUIBufferBmp);
		ReleaseDC(phwnd, mainUIDrawDC);
		ReleaseDC(phwnd, mainUIBufferDC);
		//PostQuitMessage(0);
		/**/
		KillTimer(phwnd, hTimer0);
		//LLKApplicationExit(0, NULL);
		//LLKApplicationExit(0, "");
		break;
	}
		
	default:
		return DefWindowProc(phwnd, msg, wParam, lParam);
	}
	return 0;
}

void RegisterWindow_MainUI(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc_MainUI; // ���õ��Ĳ��Ĵ��ڹ��̻ص�����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mainUIRegClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "����ע��ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
}
HWND CreateWindow_MainUI(HINSTANCE hInstance, int nCmdShow) {
	mainUIhWnd = CreateWindow(
		mainUIRegClassName,
		mainUITitle,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		mainUIWGA.X, mainUIWGA.Y, mainUIWGA.nWidth, mainUIWGA.nHeight, // �������� x,y,��� �߶� 
		NULL, NULL, hInstance, NULL);
	if (mainUIhWnd == NULL)
	{
		MessageBox(NULL, "���ڴ���ʧ��!", "����", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}
	ShowWindow(mainUIhWnd, SW_SHOW);
	UpdateWindow(mainUIhWnd);
	return mainUIhWnd;
}