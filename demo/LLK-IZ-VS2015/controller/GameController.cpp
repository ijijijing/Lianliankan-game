#include "GameController.h"

static LPCWSTR girdImagesPaths[] = {
	L"res\\girdtype00.png",
	L"res\\girdtype01.png",
	L"res\\girdtype02.png",
	L"res\\girdtype03.png",
	L"res\\girdtype04.png",
	L"res\\girdtype05.png",
	L"res\\girdtype06.png",
	L"res\\girdtype07.png",
	L"res\\girdtype08.png",
	L"res\\girdtype09.png",
	L"res\\girdtype10.png",
	L"res\\girdtype11.png",
	L"res\\girdtype12.png",
	L"res\\girdtype13.png",
	L"res\\girdtype14.png",
	L"res\\girdtype15.png",
	L"res\\girdtype16.png",
	L"res\\girdtype17.png",
	L"res\\girdtype18.png",
	L"res\\girdtype19.png",
	L"res\\girdtype20.png",
	L"res\\girdtype21.png",
	L"res\\girdtype22.png",
	L"res\\girdtype23.png",
	L"res\\girdtype24.png",
	L"res\\girdtype25.png",
	L"res\\girdtype26.png",
	L"res\\girdtype27.png",
	L"res\\girdtype28.png",
	L"res\\girdtype29.png",
};
static LPCWSTR selectedSpecEffectImagePath = L"res\\sctspeceff.png";
static LPCWSTR oddNumImagePath = L"res\\oddnumempty.png";
static LPCWSTR evnNumImagePath = L"res\\evnnumempty.png";
static LPCWSTR gameUIBackgroundImagePath = L"res\\gameUIBgd.bmp";

/*默认边长45像素*/
static int defaultImageSize = 40;
GAMESTATUS gameStatus;
bool AIModeflag = false;
int32_t AIModeClickX;
int32_t AIModeClickY;
Paths* AIModePaths;
/*别忘记初始化girdImages这个向量,大小为30*/
GameController::GameController() :girdImages(30)
{
	graphics = NULL;
	model = NULL;
	memset(&path, 0, sizeof(path));
	mapWidth = 0;
	mapHeight = 0;
	girdTypeNum = 0;
	posiX = 0;
	posiY = 0;
	girdBoxImageSize = 0;
	selectedSpecEffectImage = NULL;
	oddNumImage = NULL;
	evnNumImage = NULL;
	gameUIBackgroundImage = NULL;
	AIModePaths = &path;
}

GameController::~GameController()
{
	recycleRes();
	if (this->model != NULL) {
		delete model;
		model = NULL;
	}
	graphics = NULL;
	model = NULL;
	memset(&path, 0, sizeof(path));
	mapWidth = 0;
	mapHeight = 0;
	girdTypeNum = 0;
	posiX = 0;
	posiY = 0;
	girdBoxImageSize = 0;
	selectedSpecEffectImage = NULL;
	oddNumImage = NULL;
	evnNumImage = NULL;
}

bool GameController::init(Gdiplus::Graphics* _graphics, int32_t _mapWidth, int32_t _mapHeight, int32_t _girdTypeNum, int32_t _posiX, int32_t _posiY) {
	if (_graphics == NULL) {
		AppGlobalLogE("bool GameController::init(): _graphics is NULL Ptr\n");
		return false;
	}
	this->graphics = _graphics;
	this->mapWidth = _mapWidth;
	this->mapHeight = _mapHeight;
	this->girdTypeNum = _girdTypeNum;
	this->posiX = _posiX;
	this->posiY = _posiY;
	invalidatePaths(&this->path);
	/*初始化游戏模型*/
	this->model = new LLKModel();
	this->model->init(_mapWidth, _mapHeight, _girdTypeNum);
	/*模型生成带有最外圈留空的随机地图*/
	//if (!this->model->generateModelWithBlankAround()) {
	if (!this->model->generateModelByDefault()) {
		/*如果生成失败进行相关提示*/
		AppGlobalLogE("bool GameController::init(): generateModel Failed\ninfo: MapWidth: %2d, MapHeight: %2d, GirdTypeNum: %2d\n", _mapWidth, _mapHeight, _girdTypeNum);
		return false;
	}
	this->model->dumpMemData("modeldumpdata.txt");
	/*载入图片资源*/
	loadRes();
	gameStatus = GAMESTATUS::G_PLAYING;
}

Gdiplus::Graphics* GameController::getGDIGraphics() {
	return this->graphics;
}

void GameController::actionProc(int actiontype, void* param1, void* param2) {
	/*如果游戏状态为输或者赢,不响应动作消息*/
	if (gameStatus != GAMESTATUS::G_PLAYING) {
		return;
	}
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

		int gx = 0, gy = 0;
		if (!calculateGirdBoxPosi(*(int*)param1, *(int*)param2, &gx, &gy)) {
			AppGlobalLogI("ClickPosiNOTInGameMap\n");
			break;
		}
		if (model->getStatus() == LLKML_PLAYING) {
			AppGlobalLogI("void GameController::actionProc() SUCCESS CLICK\n");
			model->processTappedGirdBox(gx, gy, &this->path);
			if (this->path.pathNum > 0) {
				setUseTimes(&this->path, DEFAULT_MAX_PATHUSETIMES);
			}
			/*此处绘图逻辑可能会调整*/
			draw();
			static LPRECT rect;
			GetClientRect(gameUIhWnd, rect);
			InvalidateRect(gameUIhWnd, rect, false);
			if (model->getRemainingGirdNum() == 0) {
				gameStatus = GAMESTATUS::G_WIN;
				AppGlobalLogI("YOU WIN!");
			}
			
		}
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

void GameController::draw() {
	if (this->graphics == NULL) {
		AppGlobalLogE("void GameController::draw(): graphics is NULL Ptr\n");
		return;
	}
	/*绘制背景*/
	this->graphics->DrawImage(gameUIBackgroundImage, 0, 0, this->gameUIBackgroundImage->GetWidth(), this->gameUIBackgroundImage->GetHeight());
	/*绘制map*/
	GirdBoxItem* gbi = NULL;
	Array2D<GirdBox>* map = this->model->getMap();
	for (int32_t i = 0; i < this->mapWidth; i++) {
		for (int32_t j = 0; j < this->mapHeight; j++) {
			if ((i + j) % 2) {
				this->graphics->DrawImage(oddNumImage, posiX + i * girdBoxImageSize, posiY + j * girdBoxImageSize, girdBoxImageSize, girdBoxImageSize);
			}
			else {
				this->graphics->DrawImage(evnNumImage, posiX + i * girdBoxImageSize, posiY + j * girdBoxImageSize, girdBoxImageSize, girdBoxImageSize);
			}
			gbi = &(*map)[i][j];
			if (gbi->exist) {
				this->graphics->DrawImage(girdImages[gbi->type], posiX + i * girdBoxImageSize, posiY + j * girdBoxImageSize, girdBoxImageSize, girdBoxImageSize);
			}
		}
	}
	int32_t selectedGirdBoxPosiX, selectedGirdBoxPosiY;
	this->model->getSelectedPosi(&selectedGirdBoxPosiX, &selectedGirdBoxPosiY);
	if (selectedGirdBoxPosiX > 0 && selectedGirdBoxPosiY > 0) {
		this->graphics->DrawImage(selectedSpecEffectImage, posiX + selectedGirdBoxPosiX * girdBoxImageSize, posiY + selectedGirdBoxPosiY * girdBoxImageSize, girdBoxImageSize, girdBoxImageSize);
	}
	

}

void GameController::rearrangeMap() {
	if (this->model->getStatus() == LLKML_PLAYING && gameStatus == GAMESTATUS::G_PLAYING) {
		AppGlobalLogI("void GameController::rearrangeMap(): this->model->disruptMap()");
		this->model->disruptMap();
		draw();
	}
}

LLKModel* GameController::getModel() {
	return this->model;
}

void GameController::getBaseInfo(int32_t& _posiX, int32_t& _posiY, int32_t& _mapWidthPix, int32_t& _mapHeightPix, int32_t& _girdBoxImageSize) {
	_posiX = this->posiX;
	_posiY = this->posiY;
	_mapWidthPix = this->mapWidthPix;
	_mapHeightPix = this->mapHeightPix;
	_girdBoxImageSize = this->girdBoxImageSize;
}

int32_t GameController::getRemainingNum() {
	return this->model->getRemainingGirdNum();
}

Paths* GameController::getPaths() {
	return &this->path;
}

void GameController::loadRes() {
	for (int i = 0; i <= this->girdTypeNum;i++) {
		girdImages[i] = new Gdiplus::Image(girdImagesPaths[i]);
	}
	selectedSpecEffectImage = new Gdiplus::Image(selectedSpecEffectImagePath);
	oddNumImage = new Gdiplus::Image(oddNumImagePath);
	evnNumImage = new Gdiplus::Image(evnNumImagePath);
	gameUIBackgroundImage = new Gdiplus::Image(gameUIBackgroundImagePath);
	this->girdBoxImageSize = defaultImageSize;
	mapWidthPix = mapWidth * girdBoxImageSize;
	mapHeightPix = mapHeight * girdBoxImageSize;
	AppGlobalLogI("ResLoad Finish!\n");


	//Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
	//graphics->DrawString();
}

void GameController::recycleRes() {
	for (int i = 0; i < this->girdTypeNum; i++) {
		if (girdImages[i]) {
			delete girdImages[i];
			girdImages[i] = NULL;
		}
	}
	if (selectedSpecEffectImage) {
		delete selectedSpecEffectImage;
		selectedSpecEffectImage = NULL;
	}
	if (oddNumImage) {
		delete oddNumImage;
		oddNumImage = NULL;
	}
	if (evnNumImage) {
		delete evnNumImage;
		evnNumImage = NULL;
	}
	if (gameUIBackgroundImage) {
		delete gameUIBackgroundImage;
		gameUIBackgroundImage = NULL;
	}
}

inline bool GameController::isInGameMapArea(int x, int y) {
	/*判断点击位置是否在棋盘上*/
	if (x >= posiX && x <= posiX + mapWidthPix && y >= posiY && y <= posiY + mapHeightPix) {
		return true;
	}
	return false;
}
bool GameController::calculateGirdBoxPosi(int mbupx, int mbupy, int* girdx, int* girdy) {
	if (!isInGameMapArea(mbupx, mbupy)) {
		/*点击位置不在board有效区域内不响应coverGirdBox操作*/
		return false;
	}
	*girdx = (mbupx - posiX) / girdBoxImageSize;
	*girdy = (mbupy - posiY) / girdBoxImageSize;
	return true;
}
