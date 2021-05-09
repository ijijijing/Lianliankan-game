#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include <vector>

#include "..\LLKUtil.h"
#include "..\UI\WindowUtil.h"
#include "..\model\LLKModel.h"
#include "ControllerUtil.h"
extern GAMESTATUS gameStatus;
extern bool AIModeflag;
extern int32_t AIModeClickX;
extern int32_t AIModeClickY;
extern Paths* AIModePaths;
class GameController
{
public:
	GameController();
	~GameController();
	//void setGDIGraphics(Gdiplus::Graphics* _graphics);
	bool init(Gdiplus::Graphics* _graphics, int32_t _mapWidth, int32_t _mapHeight, int32_t _girdTypeNum, int32_t _posiX, int32_t _posiY);
	Gdiplus::Graphics* getGDIGraphics();
	void actionProc(int, void*, void*);
	void draw();
	void rearrangeMap();
	LLKModel* getModel();
	void getBaseInfo(int32_t&, int32_t&, int32_t&, int32_t&, int32_t&);
	int32_t getRemainingNum();
	Paths* getPaths();
private:
	Gdiplus::Graphics* graphics;
	LLKModel* model;
	Paths path;
	int32_t mapWidth;
	int32_t mapHeight;
	int32_t mapWidthPix;
	int32_t mapHeightPix;
	int32_t girdTypeNum;
	int32_t posiX, posiY;
	/*这里要求每个方格贴图为正方形,单位边长像素值*/
	int32_t girdBoxImageSize;
	std::vector<Gdiplus::Image*> girdImages;
	Gdiplus::Image* selectedSpecEffectImage;
	Gdiplus::Image* oddNumImage;
	Gdiplus::Image* evnNumImage;
	Gdiplus::Image* gameUIBackgroundImage;
	void loadRes();
	void recycleRes();
	bool GameController::isInGameMapArea(int x, int y);
	bool GameController::calculateGirdBoxPosi(int mbupx, int mbupy, int* girdx, int* girdy);
};


#endif // !_GAMECONTROLLER_H_


