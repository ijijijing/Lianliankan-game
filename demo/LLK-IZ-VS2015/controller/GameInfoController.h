#ifndef _GAMEINFOCONTROLLER_H_
#define _GAMEINFOCONTROLLER_H_

#include "..\LLKUtil.h"
#include "..\UI\WindowUtil.h"
#include "ControllerUtil.h"
#include "..\UI\comctl\button\Button.h"
#include "..\UI\comctl\progressBar\ProgressBar.h"
#include "..\UI\comctl\Line.h"
#include "GameController.h"

#define DEFAULT_PATH_REMAINING_TIME 200

class GameInfoController
{
public:
	GameInfoController();
	~GameInfoController();
	
	bool init(Gdiplus::Graphics* _graphics, GameController* _mainGameController);
	Gdiplus::Graphics* getGDIGraphics();
	void actionProc(int, void*, void*);
	void draw();
	void setRemainingTime(int32_t);
	int32_t getRemainingTime();
	void setTimingMode(bool);
	void trigger();
	void distributeActionMsg(MouseLRBtnGeoStatus& mlrbgs);
private:
	Gdiplus::Graphics* graphics;
	Paths path;
	int32_t mapWidthPix;
	int32_t mapHeightPix;
	int32_t mapPosiX, mapPosiY;
	int32_t mapGridBoxImageSize;
	int32_t totalTime;
	int32_t remainingTime;
	int32_t pathRemainingTime;
	/*����Ҫ��ÿ��������ͼΪ������,��λ�߳�����ֵ*/
	GameController* mainGameController;
	Gdiplus::Image* gameUIFgdImage;
	Gdiplus::Image* winImage;
	Gdiplus::Image* loseImage;
	bool isTimingMode;
	void loadRes();
	void recycleRes();
	void onPause();
	void onWin();
	void onLose();
	/*GameUI�ؼ�*/
	/*��׼��ť*/
	Button* hintButton;
	Button* rearrangeButton;
	Button* helpButton;
	Button* pauseButton;
	Button* AIModeButton;
	std::vector<Button*> buttons;

	/*�ı���ť*/
	Button* scoreButton;
	Button* remainingGirdButton;

	/*������*/
	ProgressBar* remainingTimeProgressBar;

	/*����·��*/
	Line* lines[3];
};


#endif // !_GAMECONTROLLER_H_


