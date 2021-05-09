#include "LLKModel.h"

LLKModel::LLKModel()
{
	modelStatus = LLKML_INIT;
	remainingGirdNum = 0;
	totalGirdNum = 0;
	map = NULL;
}

LLKModel::~LLKModel()
{
	if (map != NULL) {
		delete map;
		map = NULL;
		
	}
	
}

void LLKModel::init(int32_t _mapWidth, int32_t _mapHeight, int32_t _girdTypeNum) {
	setMapWidth(_mapWidth);
	setMapHeight(_mapHeight);
	setGirdTypeNum(_girdTypeNum);
	if (!checkRationality()) {
		this->modelStatus = LLKML_PARAMERROR;
		LLKErrLog("void LLKModel::init(): LLKML_PARAMERROR");
		return;
	}
	map = new Array2D<GirdBox>(this->mapWidth, this->mapHeight);

}

/*生成默认随机地图,且保证配对数为偶数*/
bool LLKModel::generateModelByDefault() {
	if (LLKML_READY != modelStatus) {
		LLKErrLog("bool LLKModel::generateModelByDefault() : modelStatus is not LLKML_READY\n");
		return false;
	}
	/*获取map总方格数*/
	int32_t girdBoxNum = this->mapWidth * this->mapHeight;
	uint16_t* rawData = new uint16_t[girdBoxNum];
	memset(rawData, 0, sizeof(uint16_t) * girdBoxNum);
	for (int32_t i = 0; i < girdBoxNum / 2; i++) {
		rawData[i] = (uint16_t)(i % this->girdTypeNum + 1);
		rawData[girdBoxNum - i - 1] = (uint16_t)(i % this->girdTypeNum + 1);
	}
	/*打乱*/
	shuffle(rawData, girdBoxNum);
	GirdBox* gb = (GirdBox*)map->getMemBlockPtr();
	for (int32_t j = 0; j < girdBoxNum; j++) {
		gb[j].type = rawData[j];
		/*当this->mapWidth * this->mapHeight为奇数,有一个格是空的, type为0表示此方格为空,exist为1表示此方格存在*/
		if (gb[j].type != 0) {
			gb[j].exist = 1;
			remainingGirdNum++;
			
		}
	}
	totalGirdNum = remainingGirdNum;
	/*回收内存*/
	delete[] rawData;
	/*初始化后两个缓存变量值为-1,用来识别是否有已点方格*/
	selectedGirdPosiX = -1;
	//clickedGirdPosiX = -1;
	selectedGirdPosiY = -1;
	//clickedGirdPosiY = -1;
	/*生成成功后状态变更为Playing*/
	modelStatus = LLKML_PLAYING;
	return true;
}

/*生成带有最外圈留空的随机地图,且保证配对数为偶数*/
bool LLKModel::generateModelWithBlankAround() {
	if (LLKML_READY != modelStatus) {
		LLKErrLog("bool LLKModel::generateModelByDefault() : modelStatus is not LLKML_READY\n");
		return false;
	}
	int32_t existGirdBoxNum = (this->mapWidth - 2) * (this->mapHeight - 2);
	uint16_t* rawData = new uint16_t[existGirdBoxNum];
	memset(rawData, 0, sizeof(uint16_t) * existGirdBoxNum);
	for (int32_t i = 0; i < existGirdBoxNum / 2; i++) {
		rawData[i] = (uint16_t)(i % this->girdTypeNum + 1);
		rawData[existGirdBoxNum - i - 1] = (uint16_t)(i % this->girdTypeNum + 1);
	}
	shuffle(rawData, existGirdBoxNum);
	//GirdBox* gb = (GirdBox*)map->getMemBlockPtr();
	for (int32_t j = 1, k = 0; j < this->mapHeight - 1; j++) {
		for (int32_t i = 1; i < this->mapWidth - 1; i++) {
			(*map)[i][j].type = rawData[k];
			if ((*map)[i][j].type != 0) {
				(*map)[i][j].exist = 1;
				remainingGirdNum++;
				k++;
			}
			
		}
	}
	totalGirdNum = remainingGirdNum;

	/*初始化后两个缓存变量值为-1,用来识别是否有已点方格*/
	selectedGirdPosiX = -1;
	//clickedGirdPosiX = -1;
	selectedGirdPosiY = -1;
	//clickedGirdPosiY = -1;
	modelStatus = LLKML_PLAYING;
	return true;
}

void LLKModel::setMapWidth(int32_t _mapWidth) {
	this->mapWidth = _mapWidth;
}

void LLKModel::setMapHeight(int32_t _mapHeight) {
	this->mapHeight = _mapHeight;
}

void LLKModel::setGirdTypeNum(int32_t _girdTypeNum) {
	this->girdTypeNum = _girdTypeNum;
}

bool LLKModel::checkRationality() {
	this->modelStatus = LLKML_PARAMERROR;
	if (this->mapWidth > LLKML_MAXWID || this->mapWidth < LLKML_MINWID) {
		LLKApplicationExit(LLKML_PARAMERROR,LLKErrorMsg[LLKML_PARAMERROR]);
		return false;
	}
	if (this->mapHeight > LLKML_MAXHEI || this->mapHeight < LLKML_MINHEI) {
		LLKApplicationExit(LLKML_PARAMERROR, LLKErrorMsg[LLKML_PARAMERROR]);
		return false;
	}
	if (this->girdTypeNum > LLKML_MAXTYP || this->girdTypeNum < LLKML_MINTYP) {
		LLKApplicationExit(LLKML_PARAMERROR, LLKErrorMsg[LLKML_PARAMERROR]);
		return false;
	}
	this->modelStatus = LLKML_READY;
	return true;
}

/*点击(_tappedPosiX, _tappedPosiY)位置的方格作出响应函数*/
int32_t LLKModel::processTappedGirdBox(int32_t _tappedPosiX, int32_t _tappedPosiY, Paths* paths) {
	/*位置非法则记录错误信息,直接返回*/
	if (_tappedPosiX < 0 || _tappedPosiY < 0 || _tappedPosiX >= mapWidth || _tappedPosiY >= mapHeight) {
		LLKErrLog("TappedGBPositionERR: X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*将paths置为无效状态*/
		invalidatePaths(paths);
		/*返回状态为点击位置参数非法(负数,超过map宽高等非法状态)*/
		return LLKMLTP_ERROR_POSITION;
	}
	/*本次点击位置方格状态为不存在时返回,同时将之前的已选中方格记录清空*/
	if ((*map)[_tappedPosiX][_tappedPosiY].exist == 0) {
		selectedGirdPosiX = -1;
		selectedGirdPosiY = -1;
		//LLKInfLog("TappedGBPositionEmpty: X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*将paths置为无效状态*/
		invalidatePaths(paths);
		/*返回状态为所选方格不存在*/
		return LLKMLTP_EMPTY;
	}
	/*本次点击位置与已选中方格相同时返回,同时将之前的已选中方格记录清空*/
	if (_tappedPosiX == selectedGirdPosiX && _tappedPosiY == selectedGirdPosiY) {
		selectedGirdPosiX = -1;
		selectedGirdPosiY = -1;
		//LLKInfLog("TappedGBPositionIsConsistentWithLastTime : X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*将paths置为无效状态*/
		invalidatePaths(paths);
		/*返回状态为所选方格与上一次重复*/
		return LLKMLTP_CONSISTENT;
	}
	/*处理当前模型中没有已选中方格的情况,将本次点击的方格记为待配对状态*/
	if (selectedGirdPosiX == -1 && selectedGirdPosiY == -1) {
		selectedGirdPosiX = _tappedPosiX;
		selectedGirdPosiY = _tappedPosiY;
		/*将paths置为无效状态*/
		invalidatePaths(paths);
		/*返回状态为选中某方格*/
		return LLKMLTP_SELECTED;
	}
	/*处理当前模型中有已选中方格的情况,进行消子判断*/
	else{
		/*判断选中方格与点击方格是否为同一类*/
		bool b0 = ((*map)[selectedGirdPosiX][selectedGirdPosiY].type == (*map)[_tappedPosiX][_tappedPosiY].type);
		/*两方格可消*/
		if (b0 && checkElimination(selectedGirdPosiX, selectedGirdPosiY, _tappedPosiX, _tappedPosiY, paths)) {
			/*两子存在状态置为否*/
			(*map)[selectedGirdPosiX][selectedGirdPosiY].exist = 0;
			(*map)[_tappedPosiX][_tappedPosiY].exist = 0;
			/*已选中方格记录清空*/
			selectedGirdPosiX = -1;
			selectedGirdPosiY = -1;
			/*剩余未消除数量减2*/
			this->remainingGirdNum -= 2;
			/*将paths置为有效状态,其中存储了消除路径信息*/
			validatePaths(paths);
			/*返回消除成功*/
			return LLKMLTP_ELIMINATE_SUCCESS;
		}
		/*两方格不可消*/
		else {
			/*已选中方格记录转换到_tapped位置*/
			selectedGirdPosiX = _tappedPosiX;
			selectedGirdPosiY = _tappedPosiY;
			/*将paths置为无效状态*/
			invalidatePaths(paths);
			/*消除失败,返回状态为选中方格转移到tapped位置*/
			return LLKMLTP_SWITCH_SELECTED;
		}
	}
	/*正常状态不可能到达这里*/
	LLKErrLog("FunctionERR -ImpossibleProcess | X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
	return LLKMLTP_ERROR_STATUS;
}

///*消除map上(gbPosiX, gbPosiY)处方格*/
//void LLKModel::eliminateGirdBox(int32_t gbPosiX, int32_t gbPosiY) {
//	(*map)[gbPosiX][gbPosiY].exist = 0;
//	(*map)[gbPosiX][gbPosiY].type = 0;
//}

Array2D<GirdBox>* LLKModel::getMap() {
	return map;
}

LLKModelStatus LLKModel::getStatus() {
	return modelStatus;
}

int32_t LLKModel::getRemainingGirdNum() {
	return this->remainingGirdNum;
}

int32_t LLKModel::getTotalNum() {
	return this->totalGirdNum;
}

void LLKModel::getSelectedPosi(int32_t* _sx, int32_t* _sy) {
	if (_sx != NULL && _sy != NULL) {
		*_sx = selectedGirdPosiX;
		*_sy = selectedGirdPosiY;
	}
}

/*交换两整数*/
inline void swapInt(int32_t* _a,int32_t* _b) {
	int32_t tmp = *_a;
	*_a = *_b;
	*_b = tmp;
}

#define DEBUG_ON
/*检查以(x, sy)为起点,(x, ey)为终点的垂直路径能否连通, 传入参数应保证 sy <= ey */
inline bool checkVerticalConnectivity(Array2D<GirdBox>* map, int32_t x, int32_t sy, int32_t ey) {

#ifdef DEBUG_ON
	if (sy > ey) {
		LLKErrLog("checkVerticalConnectivity err");
		assert(0);
		return false;
	}
#endif // DEBUG_ON

	for (int32_t j = sy; j <= ey; j++) {
		if ((*map)[x][j].exist == 1) {
			return false;
		}
	}
	return true;
}

/*检查以(sx, y)为起点,(ex, y)为终点的水平路径能否连通, 传入参数应保证 sx <= ex*/
inline bool checkHorizontalConnectivity(Array2D<GirdBox>* map, int32_t y, int32_t sx, int32_t ex) {

#ifdef DEBUG_ON
	if (sx > ex) {
		LLKErrLog("checkHorizontalConnectivity err");
		assert(0);
		return false;
	}
#endif // DEBUG_ON

	for (int32_t i = sx; i <= ex; i++) {
		if ((*map)[i][y].exist == 1) {
			return false;
		}
	}
	return true;
}

/*对ab两方格进行消子判断算法*/
bool LLKModel::checkElimination(int32_t aX, int32_t aY, int32_t bX, int32_t bY, Paths* paths) {
	/*查找方式A
	 *以两直线y = aY和y = bY为基准查找路径(查找方式见图)
	 */
	/*简化逻辑流程,如果A点在B点上方,"交换"A,B两点
	 *即可保证B点一定在A点上方,详细情况见图
	 */
	if (aY < bY) {
		swapInt(&aX, &bX);
		swapInt(&aY, &bY);
	}
	/*这里不必处理aY == bY的情况(原因见详细解释)*/
	if (aY != bY) {
		
		/*寻路方式1:从B点直接向下寻找路径*/
		{
			/*假装B点不存在方格,如果本寻路方式未找到结果,应将B点恢复为存在方格状态*/
			(*map)[bX][bY].exist = 0;
			
			/*检查第一条路径是否连通,注意这里不必检查到y = aY这一行*/
			if (checkVerticalConnectivity(map, bX, bY, aY - 1)) {
				/*第一条路径尽头为A点的情况*/
				if (bX == aX) {
					/*出口A_0_0*/
					setPathNum(paths, 1);
					setP1(paths, bX, bY);
					setP2(paths, aX, aY);
					return true;
				}
				/*A点在第一条路径尽头左侧的情况*/
				else if (aX < bX) {
					/*检查第三条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, bX)) {
						/*出口A_0_-1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, bX, aY);
						setP3(paths, aX, aY);
						return true;
					}
				}
				/*A点在第一条路径尽头右侧的情况*/
				else {
					/*检查第二条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, bX, aX - 1)) {
						/*出口A_0_1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, bX, aY);
						setP3(paths, aX, aY);
						return true;
					}
				}
			}

			/*如果没有进入出口,说明寻路方式1无结果路径,将B点恢复为存在方格状态*/
			(*map)[bX][bY].exist = 1;
		}

		/*寻路方式2:从B点左侧开始遍历,直到x为0*/
		for (int32_t xi = bX - 1; xi >= 0;xi--) {
			/*bX为0时,已为最左侧,不进行遍历*/
			if (bX == 0) {
				break;
			}
			/*此位置有方格,则停止向左遍历*/
			if ((*map)[xi][bY].exist == 1) {
				break;
			}
			/*检查第二条路径是否连通,注意这里不必检查到y = aY这一行*/
			if (checkVerticalConnectivity(map, xi, bY, aY - 1)) {
				/*第二条路径尽头为A点的情况*/
				if (xi == aX) {
					/*出口A_-1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, aX, bY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A点在第二条路径尽头左侧的情况*/
				else if (aX < xi) {
					/*检查第三条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, xi)) {
						/*出口A_-1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX + 1, aY)为起点,(xi, aY)为终点的水平路径不连通,检查下一个xi*/
						continue;
					}
				}
				/*A点在第二条路径尽头右侧的情况*/
				else {
					/*检查第三条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, xi, aX - 1)) {
						/*出口A_-1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(xi, aY)为起点,(aX - 1, aY)为终点的水平路径不连通,检查下一个xi*/
						continue;
					}
				}
			}
			else {
				/*以(xi, bY)为起点,(xi, aY - 1)为终点的垂直路径不连通,检查下一个xi*/
				continue;
			}
		}

		/*寻路方式3:从B点右侧开始遍历,直到x为mapWidth-1(数组下标最大值)*/
		for (int32_t xi = bX + 1; xi < this->mapWidth; xi++) {
			/*bX为mapWidth-1时,已为最右侧,不进行遍历*/
			if (bX == this->mapWidth - 1) {
				break;
			}
			/*此位置有方格,则停止向右遍历*/
			if ((*map)[xi][bY].exist == 1) {
				break;
			}
			/*检查第二条路径是否连通,注意这里不必检查到y = aY这一行*/
			if (checkVerticalConnectivity(map, xi, bY, aY - 1)) {
				/*第二条路径尽头为A点的情况*/
				if (xi == aX) {
					/*出口A_1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, aX, bY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A点在第二条路径尽头左侧的情况*/
				else if (aX < xi) {
					/*检查第三条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, xi)) {
						/*出口A_1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX + 1, aY)为起点,(xi, aY)为终点的水平路径不连通,检查下一个xi*/
						continue;
					}
				}
				/*A点在第二条路径尽头右侧的情况*/
				else {
					/*检查第三条路径是否连通,注意这里不必检查x = aX这一列*/
					if (checkHorizontalConnectivity(map, aY, xi, aX - 1)) {
						/*出口A_1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(xi, aY)为起点,(aX - 1, aY)为终点的水平路径不连通,检查下一个xi*/
						continue;
					}
				}
			}
			else {
				/*以(xi, bY)为起点,(xi, aY - 1)为终点的垂直路径不连通,检查下一个xi*/
				continue;
			}
		}
	}
	

	/*查找方式B
	*以两直线x = aX和x = bX为基准查找路径(查找方式见图)
	*/
	/*简化逻辑流程,如果A点在B点左方,"交换"A,B两点
	*即可保证B点一定在A点左方,详细情况见图
	*/
	if (aX < bX) {
		swapInt(&aX, &bX);
		swapInt(&aY, &bY);
	}
	/*这里不必处理aX == bX的情况(原因见详细解释)*/
	if (aX != bX) {
		/*寻路方式1:从B点直接向右寻找路径*/
		{
			/*假装B点不存在方格,如果本寻路方式未找到结果,应将B点恢复为存在方格状态*/
			(*map)[bX][bY].exist = 0;

			/*检查第一条路径是否连通,注意这里不必检查到x = aX这一列*/
			if (checkHorizontalConnectivity(map, bY, bX, aX - 1)) {
				/*第一条路径尽头为A点的情况*/
				if (bY == aY) {
					/*出口B_0_0*/
					setPathNum(paths, 1);
					setP1(paths, bX, bY);
					setP2(paths, aX, aY);
					return true;
				}
				/*A点在第一条路径尽头上侧的情况*/
				else if (aY < bY) {
					/*检查第三条路径是否连通,注意这里不必检查y = aX这一行*/
					if (checkVerticalConnectivity(map, aX, aY + 1, bY)) {
						/*出口B_0_-1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, aX, bY);
						setP3(paths, aX, aY);
						return true;
					}
				}
				/*A点在第一条路径尽头下侧的情况*/
				else {
					/*检查第二条路径是否连通,注意这里不必检查y = aY这一行*/
					if (checkVerticalConnectivity(map, aX, bY, aY - 1)) {
						/*出口B_0_1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, aX, bY);
						setP3(paths, aX, aY);
						return true;
					}
				}
			}

			/*如果没有进入出口,说明寻路方式1无结果路径,将B点恢复为存在方格状态*/
			(*map)[bX][bY].exist = 1;
		}

		/*寻路方式2:从B点上侧开始遍历,直到y为0*/
		for (int32_t yj = bY - 1; yj >= 0; yj--) {
			/*bY为0时,已为最上侧,不进行遍历*/
			if (bY == 0) {
				break;
			}
			/*此位置有方格,则停止向上遍历*/
			if ((*map)[bX][yj].exist == 1) {
				break;
			}
			/*检查第二条路径是否连通,注意这里不必检查到x = aX这一列*/
			if (checkHorizontalConnectivity(map, yj, bX, aX - 1)) {
				/*第二条路径尽头为A点的情况*/
				if (yj == aY) {
					/*出口B_-1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, bX, aY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A点在第二条路径尽头上侧的情况*/
				else if (aY < yj) {
					/*检查第三条路径是否连通,注意这里不必检查y = aY这一行*/
					if (checkVerticalConnectivity(map, aX, aY + 1, yj)) {
						/*出口B_-1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX, aY + 1)为起点,(aX, yj)为终点的垂直路径不连通,检查下一个yj*/
						continue;
					}
				}
				/*A点在第二条路径尽头下侧的情况*/
				else {
					/*检查第三条路径是否连通,注意这里不必检查y = aY这一行*/
					if (checkVerticalConnectivity(map, aX, yj, aY - 1)) {
						/*出口B_-1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX, yj)为起点,(aX, aY - 1)为终点的垂直路径不连通,检查下一个yj*/
						continue;
					}
				}
			}
			else {
				/*以(bX, yj)为起点,(aX - 1, yj)为终点的水平路径不连通,检查下一个yj*/
				continue;
			}
		}

		/*寻路方式3:从B点下侧开始遍历,直到x为mapHeight-1(数组下标最大值)*/
		for (int32_t yj = bY + 1; yj < this->mapHeight; yj++) {
			/*bY为mapHeight-1时,已为最下侧,不进行遍历*/
			if (bY == mapHeight - 1) {
				break;
			}
			/*此位置有方格,则停止向下遍历*/
			if ((*map)[bX][yj].exist == 1) {
				break;
			}
			/*检查第二条路径是否连通,注意这里不必检查到x = aX这一列*/
			if (checkHorizontalConnectivity(map, yj, bX, aX - 1)) {
				/*第二条路径尽头为A点的情况*/
				if (yj == aY) {
					/*出口B_1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, bX, aY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A点在第二条路径尽头上侧的情况*/
				else if (aY < yj) {
					/*检查第三条路径是否连通,注意这里不必检查y = aY这一行*/
					if (checkVerticalConnectivity(map, aX, aY + 1, yj)) {
						/*出口B_1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX, aY + 1)为起点,(aX, yj)为终点的垂直路径不连通,检查下一个yj*/
						continue;
					}
				}
				/*A点在第二条路径尽头下侧的情况*/
				else {
					/*检查第三条路径是否连通,注意这里不必检查y = aY这一行*/
					if (checkVerticalConnectivity(map, aX, yj, aY - 1)) {
						/*出口B_1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*以(aX, yj)为起点,(aX, aY - 1)为终点的垂直路径不连通,检查下一个yj*/
						continue;
					}
				}
			}
			else {
				/*以(bX, yj)为起点,(aX - 1, yj)为终点的水平路径不连通,检查下一个yj*/
				continue;
			}
		}
	}

	return false;
}

/*导出map数据到文件(ForDebug)*/
void LLKModel::dumpMemData(char* fileName) {
#ifdef DEBUG_ON
	if (fileName == NULL || map == NULL) {
		return;
	}
	FILE* filePtr = fopen(fileName, "w+");
	if (filePtr == NULL) {
		LLKErrLog("function: void LLKModel::dumpMemData() File Open Failed! FileName: %s\n", fileName);
		return;
	}
	fprintf(filePtr, "LLKModel Info:\nMapWidth:  %03d\nMapHeight: %03d\n", this->mapWidth, this->mapHeight);
	for (int32_t j = 0; j < this->mapHeight; j++) {
		for (int32_t i = 0; i < this->mapWidth; i++) {
			if ((*map)[i][j].exist == 1) {
				fprintf(filePtr, "%02d ", (*map)[i][j].type);
			}
			else {
				fprintf(filePtr, "-1 ");
			}
		}
		fprintf(filePtr, "\n");
	}
	fflush(filePtr);
	fclose(filePtr);
#endif // DEBUG_ON
}

/*
 *打乱map
 *map上每个方格几何位置以及存在状态不会受影响,打乱的是每个方格type值
 */
void LLKModel::disruptMap() {
	if (map == NULL) {
		LLKErrLog("MapPtrNULL!\n");
		return;
	}
	if (remainingGirdNum < 2 || remainingGirdNum > LLKML_MAXGBNUM) {
		LLKInfLog("remainingGirdNum Illegal! value: %d\n", remainingGirdNum);
		return;
	}

	/*获取map上方格总数*/
	int32_t girdBoxNum = this->mapWidth * this->mapHeight;

	/*为数组rawData分配内存空间*/
	uint16_t* rawData = new uint16_t[remainingGirdNum];

	/*初始化数组rawData为全0*/
	memset(rawData, 0, sizeof(uint16_t) * remainingGirdNum);

	/*获取Array2D类的实例map中底层连续内存空间首地址*/
	GirdBox* p = (GirdBox*)map->getMemBlockPtr();

	/*遍历map中所有方格,将exist属性为1的方格的type值存入数组rawData中*/
	for (int32_t i = 0, j = 0; i < girdBoxNum && j < remainingGirdNum; i++) {
		if (p[i].exist == 1) {
			rawData[j] = p[i].type;
			j++;
		}
	}

	/*随机打乱rawData数组*/
	shuffle(rawData, remainingGirdNum);

	/*遍历map中所有方格,将打乱后的rawData数组中的元素重新写入exist属性为1的方格*/
	for (int32_t i = 0, j = 0; i < girdBoxNum && j < remainingGirdNum; i++) {
		if (p[i].exist == 1) {
			p[i].type = rawData[j];
			j++;
		}
	}

	/*回收内存*/
	delete[] rawData;
}