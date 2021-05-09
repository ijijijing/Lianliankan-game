#include "GameInfoController.h"

#include <shellapi.h>

static LPCWSTR gameUIFgdImagePath = L"res\\gameUIFgd.png";
static LPCWSTR winImagePath = L"res\\win.png";
static LPCWSTR loseImagePath = L"res\\lose.png";
static GameInfoController* thisInfoController = NULL;
static GameController* thisController = NULL;

/*一些回调函数*/
void hint() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}

}
void help() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}
	static LPCSTR HelpUrl = "https://www.baidu.com/s?wd=%E8%BF%9E%E8%BF%9E%E7%9C%8B%E6%80%8E%E4%B9%88%E7%8E%A9";
	AppGlobalLogI("void jumpToHelp(): open url: %s\n", HelpUrl);
	ShellExecute(NULL, "open", HelpUrl, NULL, NULL, SW_SHOWNORMAL);
}
void pause() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}

}
void gameover() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}

}
void rearrange() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}
	thisController->rearrangeMap();
}
void win() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}

}
void lose() {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}

}


void _cdecl AIMode(PVOID param);
void _cdecl AIMode(PVOID param) {
	if (thisInfoController == NULL || thisController == NULL) {
		return;
	}
	if (!AIModeflag) {
		AIModeflag = true;
		LLKModel* model = thisController->getModel();
		Array2D<GirdBox>* map = model->getMap();
		Paths* paths = thisController->getPaths();
		int32_t width, height, i1, j1, i2, j2, posiX, posiY, imgsize, b, c;
		map->getXY(width, height);
		thisController->getBaseInfo(posiX, posiY, b, c, imgsize);
		while (model->getRemainingGirdNum() > 0) {
			for (i1 = 0; i1 < width; i1++) {
				for (j1 = 0; j1 < height; j1++) {
					if ((*map)[i1][j1].exist == 0) {
						continue;
					}
					for (i2 = 0; i2 < width; i2++) {
						for (j2 = 0; j2 < height; j2++) {
							if ((*map)[i2][j2].exist == 0) {
								continue;
							}
							if (LLKTappedResult::LLKMLTP_ELIMINATE_SUCCESS == model->processTappedGirdBox(i1, j1, paths)) {
								j1++;
							}
							if (LLKTappedResult::LLKMLTP_ELIMINATE_SUCCESS == model->processTappedGirdBox(i2, j2, paths)) {
								PostMessage(gameUIhWnd, WM_PAINT, 666, 999);
								Sleep(210);
							}
						}
					}
				}
			}
		}
		gameStatus = GAMESTATUS::G_WIN;
	}
}
void beginAIMode() {
	_beginthread(AIMode, 0, NULL);
}



GameInfoController::GameInfoController()
{

	isTimingMode = false;
	graphics = NULL;
	memset(&path, 0, sizeof(path));
	thisInfoController = this;
	pathRemainingTime = DEFAULT_PATH_REMAINING_TIME;
	mapGridBoxImageSize = 0;
}

GameInfoController::~GameInfoController()
{
	recycleRes();
	graphics = NULL;
}

bool GameInfoController::init(Gdiplus::Graphics* _graphics, GameController* _mainGameController) {
	if (_graphics == NULL || _mainGameController == NULL) {
		AppGlobalLogE("bool GameInfoController::init(): _graphics is NULL Ptr or _mainGameController is NULL Ptr\n");
		return false;
	}
	this->graphics = _graphics;
	this->mainGameController = _mainGameController;
	this->mainGameController->getBaseInfo(this->mapPosiX, this->mapPosiY, this->mapWidthPix, this->mapHeightPix, this->mapGridBoxImageSize);
	/*分别将两个控制器赋值给两静态变量,方便回调函数使用*/
	thisController = this->mainGameController;
	thisInfoController = this;
	AIModeflag = false;
	loadRes();
}

Gdiplus::Graphics* GameInfoController::getGDIGraphics() {
	return this->graphics;
}

void GameInfoController::actionProc(int actiontype, void* param1, void* param2) {
	switch (actiontype) {
		/*响应鼠标移动*/
	case GC_MOUSEMOVE: {

		break;
	}
	/*响应鼠标左键按下*/
	case GC_MOUSELBDW: {

		break;
	}
	/*响应鼠标右键按下*/
	case GC_MOUSERBDW: {

		break;
	}
	/*响应鼠标左键释放*/
	case GC_MOUSELBUP: {

		
		break;
	}
	/*响应鼠标右键释放*/
	case GC_MOUSERBUP: {

		break;
	}


	default: {

	}
	}

}

void GameInfoController::draw() {
	if (this->graphics == NULL) {
		AppGlobalLogE("void GameInfoController::draw(): graphics is NULL Ptr\n");
		return;
	}
	graphics->DrawImage(gameUIFgdImage, 0, 0, gameUIWGA.nWidth, gameUIWGA.nHeight);
	hintButton->draw();
	rearrangeButton->draw();
	helpButton->draw();
	pauseButton->draw();
	AIModeButton->draw();
	wchar_t tmp[20];
	
	wsprintfW(tmp, L"分数: %3d", (this->mainGameController->getModel()->getTotalNum() -this->mainGameController->getRemainingNum()));
	scoreButton->setText(tmp);
	scoreButton->draw();
	
	wsprintfW(tmp, L"剩余方格: %3d", this->mainGameController->getRemainingNum());
	remainingGirdButton->setText(tmp);
	remainingGirdButton->draw();
	static int targetWidth = 400, targetHeight = 300;
	if (isTimingMode) {
		if (remainingTime == 0) {
			gameStatus = GAMESTATUS::G_LOSE;
			
			graphics->DrawImage(loseImage, gameUIWGA.nWidth / 2 - targetWidth / 2, gameUIWGA.nHeight / 2 - targetHeight / 2, targetWidth, targetHeight);
		}
		remainingTimeProgressBar->draw();
	}
	if (gameStatus == GAMESTATUS::G_WIN) {
		graphics->DrawImage(winImage, gameUIWGA.nWidth / 2 - targetWidth / 2, gameUIWGA.nHeight / 2 - targetHeight / 2, targetWidth, targetHeight);
	}
	


	/*绘制消除Path*/
	Paths* paths = mainGameController->getPaths();
	/*pathNum有效时进行绘制*/
	if (paths->pathNum > 0 && paths->pathNum < 4 && paths->useTimes > 0) {
		
		AppGlobalLogI("DrawPath\n");
		if (paths->pathNum > 0) {
			AppGlobalLogI("Path0\n");
			if (paths->useTimes == DEFAULT_MAX_PATHUSETIMES) {
				AppGlobalLogI("Path0 SET\n");
				this->lines[0]->setStartPosi(this->mapPosiX + paths->p1x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p1y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
				this->lines[0]->setEndPosi(this->mapPosiX + paths->p2x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p2y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
			}
			this->lines[0]->setTransparency(paths->useTimes * 255 / DEFAULT_MAX_PATHUSETIMES);
			this->lines[0]->draw();
		}
		if (paths->pathNum > 1) {
			AppGlobalLogI("Path1\n");
			if (paths->useTimes == DEFAULT_MAX_PATHUSETIMES) {
				AppGlobalLogI("Path1 SET\n");
				this->lines[1]->setStartPosi(this->mapPosiX + paths->p2x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p2y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
				this->lines[1]->setEndPosi(this->mapPosiX + paths->p3x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p3y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
			}
			this->lines[1]->setTransparency(paths->useTimes * 255 / DEFAULT_MAX_PATHUSETIMES);
			this->lines[1]->draw();
		}
		if (paths->pathNum > 2) {
			AppGlobalLogI("Path2\n");
			if (paths->useTimes == DEFAULT_MAX_PATHUSETIMES) {
				AppGlobalLogI("Path2 SET\n");
				this->lines[2]->setStartPosi(this->mapPosiX + paths->p3x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p3y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
				this->lines[2]->setEndPosi(this->mapPosiX + paths->p4x * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2, this->mapPosiY + paths->p4y * this->mapGridBoxImageSize + this->mapGridBoxImageSize / 2);
			}
			this->lines[2]->setTransparency(paths->useTimes * 255 / DEFAULT_MAX_PATHUSETIMES);
			this->lines[2]->draw();
		}
		paths->useTimes--;
	}
}

void GameInfoController::setRemainingTime(int32_t _totalTime) {
	this->totalTime = _totalTime;
	this->remainingTime = _totalTime;
}

int32_t GameInfoController::getRemainingTime() {
	return this->remainingTime;
}

void GameInfoController::setTimingMode(bool _b) {
	this->isTimingMode = _b;
}

/*设置触发器*/
void GameInfoController::trigger() {
	if (gameStatus != GAMESTATUS::G_PLAYING) {
		return;
	}
	if (isTimingMode && this->remainingTime > 0) {
		this->remainingTime--;
		static wchar_t timeStr[50];
		memset(timeStr, 0, sizeof(timeStr));
		if (AIModeflag) {
			wsprintfW(timeStr, L"剩余时间: AI模式自动处理ing..");
		}
		else {
			wsprintfW(timeStr, L"剩余时间: %d", remainingTime);
		}
		
		this->remainingTimeProgressBar->setText(timeStr);
		
		int percentage = 100 * remainingTime / totalTime;
		if (percentage > 60) {
			this->remainingTimeProgressBar->setBackgroundColor(150, 12, 240, 25);
			this->remainingTimeProgressBar->setFlittedColor(175, 12, 240, 25);
			this->remainingTimeProgressBar->setPressedColor(240, 12, 240, 25);
		}
		else if(percentage < 20){
			this->remainingTimeProgressBar->setBackgroundColor(195, 230, 10, 5);
			this->remainingTimeProgressBar->setFlittedColor(220, 230, 10, 5);
			this->remainingTimeProgressBar->setPressedColor(235, 230, 10, 5);
		}
		else {
			this->remainingTimeProgressBar->setBackgroundColor(170, 255, 252, 0);
			this->remainingTimeProgressBar->setFlittedColor(180, 255, 252, 0);
			this->remainingTimeProgressBar->setPressedColor(200, 255, 252, 0);
		}
		this->remainingTimeProgressBar->trigger(percentage);
		
	}
}

/*鼠标操作消息分发*/
void GameInfoController::distributeActionMsg(MouseLRBtnGeoStatus& mlrbgs) {
	for (int i = 0; i < this->buttons.size(); i++) {
		buttons[i]->onMouseEventProcess(&mlrbgs);
	}
	remainingTimeProgressBar->onMouseEventProcess(&mlrbgs);
}

void GameInfoController::loadRes() {
	gameUIFgdImage = new Gdiplus::Image(gameUIFgdImagePath);
	winImage = new Gdiplus::Image(winImagePath);
	loseImage = new Gdiplus::Image(loseImagePath);

	int32_t unifiedX = this->mapPosiX + this->mapWidthPix + 30;

	hintButton = new Button();
	hintButton->setGDIGraphics(this->graphics);
	hintButton->setText(L"提示");
	hintButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	hintButton->setSize(160, 50);
	hintButton->setPosi(unifiedX, 150);
	hintButton->setTextColor(200, 10, 10, 10);
	hintButton->setFontSize(30);
	hintButton->setFontName(L"微软雅黑");
	hintButton->setFontStyle(FontStyle::FontStyleBold);
	hintButton->enableDisplayBorder(true);
	hintButton->setBorderColor(240, 250, 250, 250);
	hintButton->setBorderStroke(1.5f);
	hintButton->setBorderRectRadius(20);
	hintButton->setShapeType(BorderType::BROUNDRECT);
	hintButton->enableDisplayBackground(true);
	hintButton->setPressedColor(240, 0, 204, 238);
	hintButton->setFlittedColor(175, 0, 204, 238);
	hintButton->setBackgroundColor(150, 255, 255, 255);
	hintButton->setPressedTextColor(250, 255, 255, 255);
	hintButton->setFlittedTextColor(240, 255, 255, 255);
	hintButton->setOnClickCallBack(hint);
	buttons.push_back(hintButton);

	rearrangeButton = new Button();
	rearrangeButton->setGDIGraphics(this->graphics);
	rearrangeButton->setText(L"重排");
	rearrangeButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	rearrangeButton->setSize(160, 50);
	rearrangeButton->setPosi(unifiedX, 210);
	rearrangeButton->setTextColor(200, 10, 10, 10);
	rearrangeButton->setFontSize(30);
	rearrangeButton->setFontName(L"微软雅黑");
	rearrangeButton->setFontStyle(FontStyle::FontStyleBold);
	rearrangeButton->enableDisplayBorder(true);
	rearrangeButton->setBorderColor(240, 250, 250, 250);
	rearrangeButton->setBorderStroke(1.5f);
	rearrangeButton->setBorderRectRadius(20);
	rearrangeButton->setShapeType(BorderType::BROUNDRECT);
	rearrangeButton->enableDisplayBackground(true);
	rearrangeButton->setPressedColor(240, 0, 204, 238);
	rearrangeButton->setFlittedColor(175, 0, 204, 238);
	rearrangeButton->setBackgroundColor(150, 255, 255, 255);
	rearrangeButton->setPressedTextColor(250, 255, 255, 255);
	rearrangeButton->setFlittedTextColor(240, 255, 255, 255);
	rearrangeButton->setOnClickCallBack(rearrange);
	buttons.push_back(rearrangeButton);

	helpButton = new Button();
	helpButton->setGDIGraphics(this->graphics);
	helpButton->setText(L"帮助");
	helpButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	helpButton->setSize(160, 50);
	helpButton->setPosi(unifiedX, 270);
	helpButton->setTextColor(200, 10, 10, 10);
	helpButton->setFontSize(30);
	helpButton->setFontName(L"微软雅黑");
	helpButton->setFontStyle(FontStyle::FontStyleBold);
	helpButton->enableDisplayBorder(true);
	helpButton->setBorderColor(240, 250, 250, 250);
	helpButton->setBorderStroke(1.5f);
	helpButton->setBorderRectRadius(20);
	helpButton->setShapeType(BorderType::BROUNDRECT);
	helpButton->enableDisplayBackground(true);
	helpButton->setPressedColor(240, 0, 204, 238);
	helpButton->setFlittedColor(175, 0, 204, 238);
	helpButton->setBackgroundColor(150, 255, 255, 255);
	helpButton->setPressedTextColor(250, 255, 255, 255);
	helpButton->setFlittedTextColor(240, 255, 255, 255);
	helpButton->setOnClickCallBack(help);
	buttons.push_back(helpButton);

	pauseButton = new Button();
	pauseButton->setGDIGraphics(this->graphics);
	pauseButton->setText(L"暂停");
	pauseButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	pauseButton->setSize(160, 50);
	pauseButton->setPosi(unifiedX, 330);
	pauseButton->setTextColor(200, 10, 10, 10);
	pauseButton->setFontSize(30);
	pauseButton->setFontName(L"微软雅黑");
	pauseButton->setFontStyle(FontStyle::FontStyleBold);
	pauseButton->enableDisplayBorder(true);
	pauseButton->setBorderColor(240, 250, 250, 250);
	pauseButton->setBorderStroke(1.5f);
	pauseButton->setBorderRectRadius(20);
	pauseButton->setShapeType(BorderType::BROUNDRECT);
	pauseButton->enableDisplayBackground(true);
	pauseButton->setPressedColor(240, 0, 204, 238);
	pauseButton->setFlittedColor(175, 0, 204, 238);
	pauseButton->setBackgroundColor(150, 255, 255, 255);
	pauseButton->setPressedTextColor(250, 255, 255, 255);
	pauseButton->setFlittedTextColor(240, 255, 255, 255);
	pauseButton->setOnClickCallBack(pause);
	buttons.push_back(pauseButton);

	AIModeButton = new Button();
	AIModeButton->setGDIGraphics(this->graphics);
	AIModeButton->setText(L"AI模式");
	AIModeButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	AIModeButton->setSize(160, 50);
	AIModeButton->setPosi(unifiedX, 390);
	AIModeButton->setTextColor(200, 10, 10, 10);
	AIModeButton->setFontSize(30);
	AIModeButton->setFontName(L"微软雅黑");
	AIModeButton->setFontStyle(FontStyle::FontStyleBold);
	AIModeButton->enableDisplayBorder(true);
	AIModeButton->setBorderColor(240, 250, 250, 250);
	AIModeButton->setBorderStroke(1.5f);
	AIModeButton->setBorderRectRadius(20);
	AIModeButton->setShapeType(BorderType::BROUNDRECT);
	AIModeButton->enableDisplayBackground(true);
	AIModeButton->setPressedColor(240, 0, 204, 238);
	AIModeButton->setFlittedColor(175, 0, 204, 238);
	AIModeButton->setBackgroundColor(150, 255, 255, 255);
	AIModeButton->setPressedTextColor(250, 255, 255, 255);
	AIModeButton->setFlittedTextColor(240, 255, 255, 255);
	AIModeButton->setOnClickCallBack(beginAIMode);
	buttons.push_back(AIModeButton);

	scoreButton = new Button();
	scoreButton->setGDIGraphics(this->graphics);
	scoreButton->setText(L"分数: ");
	scoreButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	scoreButton->setSize(160, 50);
	scoreButton->setPosi(unifiedX, 600);
	scoreButton->setTextColor(200, 10, 10, 10);
	scoreButton->setFontSize(15);
	scoreButton->setFontName(L"微软雅黑");
	scoreButton->setFontStyle(FontStyle::FontStyleBold);
	scoreButton->enableDisplayBorder(true);
	scoreButton->setBorderColor(240, 250, 250, 250);
	scoreButton->setBorderStroke(3.0f);
	scoreButton->setBorderRectRadius(20);
	scoreButton->setShapeType(BorderType::BRECT);
	scoreButton->enableDisplayBackground(true);
	scoreButton->setPressedColor(240, 0, 204, 238);
	scoreButton->setFlittedColor(175, 0, 204, 238);
	scoreButton->setBackgroundColor(150, 255, 255, 255);
	scoreButton->setPressedTextColor(250, 255, 255, 255);
	scoreButton->setFlittedTextColor(240, 255, 255, 255);
	buttons.push_back(scoreButton);

	remainingGirdButton = new Button();
	remainingGirdButton->setGDIGraphics(this->graphics);
	remainingGirdButton->setText(L"剩余方格: ");
	remainingGirdButton->setStringAlignment(StringAlignment::StringAlignmentCenter);
	remainingGirdButton->setSize(160, 50);
	remainingGirdButton->setPosi(unifiedX, 670);
	remainingGirdButton->setTextColor(200, 10, 10, 10);
	remainingGirdButton->setFontSize(15);
	remainingGirdButton->setFontName(L"微软雅黑");
	remainingGirdButton->setFontStyle(FontStyle::FontStyleBold);
	remainingGirdButton->enableDisplayBorder(true);
	remainingGirdButton->setBorderColor(240, 250, 250, 250);
	remainingGirdButton->setBorderStroke(3.0f);
	remainingGirdButton->setBorderRectRadius(20);
	remainingGirdButton->setShapeType(BorderType::BRECT);
	remainingGirdButton->enableDisplayBackground(true);
	remainingGirdButton->setPressedColor(240, 0, 204, 238);
	remainingGirdButton->setFlittedColor(175, 0, 204, 238);
	remainingGirdButton->setBackgroundColor(150, 255, 255, 255);
	remainingGirdButton->setPressedTextColor(250, 255, 255, 255);
	remainingGirdButton->setFlittedTextColor(240, 255, 255, 255);
	buttons.push_back(remainingGirdButton);

	remainingTimeProgressBar = new ProgressBar();
	remainingTimeProgressBar->setGDIGraphics(this->graphics);
	remainingTimeProgressBar->setText(L"100");
	remainingTimeProgressBar->setStringAlignment(StringAlignment::StringAlignmentCenter);
	remainingTimeProgressBar->setSize(this->mapWidthPix, 50);
	remainingTimeProgressBar->setPosi(100, this->mapPosiY + this->mapHeightPix + 20);
	remainingTimeProgressBar->setTextColor(200, 10, 10, 10);
	remainingTimeProgressBar->setFontSize(30);
	remainingTimeProgressBar->setFontName(L"微软雅黑");
	remainingTimeProgressBar->setFontStyle(FontStyle::FontStyleBold);
	remainingTimeProgressBar->enableDisplayBorder(true);
	remainingTimeProgressBar->setBorderColor(0, 255, 255, 255);
	remainingTimeProgressBar->setBorderStroke(1.5f);
	remainingTimeProgressBar->setBorderRectRadius(20);
	remainingTimeProgressBar->setShapeType(BorderType::BROUNDRECT);
	remainingTimeProgressBar->enableDisplayBackground(true);
	remainingTimeProgressBar->setPressedColor(240, 0, 204, 238);
	remainingTimeProgressBar->setFlittedColor(175, 0, 204, 238);
	remainingTimeProgressBar->setBackgroundColor(150, 0, 204, 238);
	remainingTimeProgressBar->setPressedTextColor(250, 255, 255, 255);
	remainingTimeProgressBar->setFlittedTextColor(240, 255, 255, 255);
	remainingTimeProgressBar->setProgressPercentage(100);
	remainingTimeProgressBar->setOnProgressFinishCallBack(gameover);

	for (int i = 0; i < 3; i++) {
		this->lines[i] = new Line();
		lines[i]->setGDIGraphics(this->graphics);
		lines[i]->setStroke(5.0f);
		lines[i]->setColor(150, 255, 138, 43);
	}
}

void GameInfoController::recycleRes() {
	if (gameUIFgdImage) {
		delete gameUIFgdImage;
		gameUIFgdImage = NULL;
	}
	if (winImage) {
		delete winImage;
		winImage = NULL;
	}
	if (loseImage) {
		delete loseImage;
		loseImage = NULL;
	}
	if (hintButton) {
		delete hintButton;
		hintButton = NULL;
	}
	if (rearrangeButton) {
		delete rearrangeButton;
		rearrangeButton = NULL;
	}
	if (helpButton) {
		delete helpButton;
		helpButton = NULL;
	}
	if (pauseButton) {
		delete pauseButton;
		pauseButton = NULL;
	}
	if (AIModeButton) {
		delete AIModeButton;
		AIModeButton = NULL;
	}
	if (scoreButton) {
		delete scoreButton;
		scoreButton = NULL;
	}
	if (remainingGirdButton) {
		delete remainingGirdButton;
		remainingGirdButton = NULL;
	}
	if (remainingTimeProgressBar) {
		delete remainingTimeProgressBar;
		remainingTimeProgressBar = NULL;
	}
	buttons.clear();
	for (int i = 0; i < 3; i++) {
		if (this->lines[i]) {
			delete this->lines[i];
			this->lines[i] = NULL;
		}
	}
}

void GameInfoController::onPause() {

}

void GameInfoController::onWin() {

}

void GameInfoController::onLose() {

}

