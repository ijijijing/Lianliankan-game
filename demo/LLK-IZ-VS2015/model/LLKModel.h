#ifndef _LLKMODEL_H_
#define _LLKMODEL_H_

#include "GirdBox.h"
#include "Paths.h"
#include "Array2D.h"
#include "../LLKUtil.h"

#define LLKML_MAXWID 200
#define LLKML_MINWID 5
#define LLKML_MAXHEI 200
#define LLKML_MINHEI 5
#define LLKML_MAXTYP 30
#define LLKML_MINTYP 5
#define LLKML_MAXGBNUM 40000

enum LLKModelStatus
{
	LLKML_INIT,
	LLKML_READY,
	//LLKML_GENFIN,
	LLKML_PLAYING,
	LLKML_PARAMERROR,
	LLKML_FINISH,
};

enum LLKTappedResult
{
	LLKMLTP_EMPTY,
	LLKMLTP_CONSISTENT,
	LLKMLTP_SELECTED,
	LLKMLTP_ELIMINATE_SUCCESS,
	LLKMLTP_SWITCH_SELECTED,
	LLKMLTP_ERROR_POSITION,
	LLKMLTP_ERROR_STATUS
};

static const char* LLKErrorMsg[5] = {
	"Initializating...",
	"Ready",
	"ParamError",
	"Finish",
	"",
};

class LLKModel
{
public:
	LLKModel();
	~LLKModel();
	void init(int32_t _mapWidth, int32_t _mapHeight, int32_t _girdTypeNum);
	bool generateModelByDefault();
	bool generateModelWithBlankAround();
	void disruptMap();
	int32_t processTappedGirdBox(int32_t _tappedPosiX, int32_t _tappedPosiY, Paths* paths);
	void dumpMemData(char* fileName);
	Array2D<GirdBox>* getMap();
	LLKModelStatus getStatus();
	int32_t getRemainingGirdNum();
	int32_t getTotalNum();
	void getSelectedPosi(int32_t* _sx, int32_t* _sy);
private:
	LLKModelStatus modelStatus;
	int32_t mapWidth;
	int32_t mapHeight;
	int32_t girdTypeNum;
	int32_t remainingGirdNum;
	int32_t totalGirdNum;
	Array2D<GirdBox>* map;
	int32_t selectedGirdPosiX;
	int32_t selectedGirdPosiY;
	/*int32_t clickedGirdPosiX;
	int32_t clickedGirdPosiY;*/

	void setMapWidth(int32_t _mapWidth);
	void setMapHeight(int32_t _mapHeight);
	void setGirdTypeNum(int32_t _girdTypeNum);
	bool checkRationality();
	bool checkElimination(int32_t aX, int32_t aY, int32_t bX, int32_t bY, Paths* paths);

	//void eliminateGirdBox(int32_t gbPosiX, int32_t gbPosiY);
};



#endif