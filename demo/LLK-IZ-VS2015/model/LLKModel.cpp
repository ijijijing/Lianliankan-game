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

/*����Ĭ�������ͼ,�ұ�֤�����Ϊż��*/
bool LLKModel::generateModelByDefault() {
	if (LLKML_READY != modelStatus) {
		LLKErrLog("bool LLKModel::generateModelByDefault() : modelStatus is not LLKML_READY\n");
		return false;
	}
	/*��ȡmap�ܷ�����*/
	int32_t girdBoxNum = this->mapWidth * this->mapHeight;
	uint16_t* rawData = new uint16_t[girdBoxNum];
	memset(rawData, 0, sizeof(uint16_t) * girdBoxNum);
	for (int32_t i = 0; i < girdBoxNum / 2; i++) {
		rawData[i] = (uint16_t)(i % this->girdTypeNum + 1);
		rawData[girdBoxNum - i - 1] = (uint16_t)(i % this->girdTypeNum + 1);
	}
	/*����*/
	shuffle(rawData, girdBoxNum);
	GirdBox* gb = (GirdBox*)map->getMemBlockPtr();
	for (int32_t j = 0; j < girdBoxNum; j++) {
		gb[j].type = rawData[j];
		/*��this->mapWidth * this->mapHeightΪ����,��һ�����ǿյ�, typeΪ0��ʾ�˷���Ϊ��,existΪ1��ʾ�˷������*/
		if (gb[j].type != 0) {
			gb[j].exist = 1;
			remainingGirdNum++;
			
		}
	}
	totalGirdNum = remainingGirdNum;
	/*�����ڴ�*/
	delete[] rawData;
	/*��ʼ���������������ֵΪ-1,����ʶ���Ƿ����ѵ㷽��*/
	selectedGirdPosiX = -1;
	//clickedGirdPosiX = -1;
	selectedGirdPosiY = -1;
	//clickedGirdPosiY = -1;
	/*���ɳɹ���״̬���ΪPlaying*/
	modelStatus = LLKML_PLAYING;
	return true;
}

/*���ɴ�������Ȧ���յ������ͼ,�ұ�֤�����Ϊż��*/
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

	/*��ʼ���������������ֵΪ-1,����ʶ���Ƿ����ѵ㷽��*/
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

/*���(_tappedPosiX, _tappedPosiY)λ�õķ���������Ӧ����*/
int32_t LLKModel::processTappedGirdBox(int32_t _tappedPosiX, int32_t _tappedPosiY, Paths* paths) {
	/*λ�÷Ƿ����¼������Ϣ,ֱ�ӷ���*/
	if (_tappedPosiX < 0 || _tappedPosiY < 0 || _tappedPosiX >= mapWidth || _tappedPosiY >= mapHeight) {
		LLKErrLog("TappedGBPositionERR: X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*��paths��Ϊ��Ч״̬*/
		invalidatePaths(paths);
		/*����״̬Ϊ���λ�ò����Ƿ�(����,����map��ߵȷǷ�״̬)*/
		return LLKMLTP_ERROR_POSITION;
	}
	/*���ε��λ�÷���״̬Ϊ������ʱ����,ͬʱ��֮ǰ����ѡ�з����¼���*/
	if ((*map)[_tappedPosiX][_tappedPosiY].exist == 0) {
		selectedGirdPosiX = -1;
		selectedGirdPosiY = -1;
		//LLKInfLog("TappedGBPositionEmpty: X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*��paths��Ϊ��Ч״̬*/
		invalidatePaths(paths);
		/*����״̬Ϊ��ѡ���񲻴���*/
		return LLKMLTP_EMPTY;
	}
	/*���ε��λ������ѡ�з�����ͬʱ����,ͬʱ��֮ǰ����ѡ�з����¼���*/
	if (_tappedPosiX == selectedGirdPosiX && _tappedPosiY == selectedGirdPosiY) {
		selectedGirdPosiX = -1;
		selectedGirdPosiY = -1;
		//LLKInfLog("TappedGBPositionIsConsistentWithLastTime : X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
		/*��paths��Ϊ��Ч״̬*/
		invalidatePaths(paths);
		/*����״̬Ϊ��ѡ��������һ���ظ�*/
		return LLKMLTP_CONSISTENT;
	}
	/*����ǰģ����û����ѡ�з�������,�����ε���ķ����Ϊ�����״̬*/
	if (selectedGirdPosiX == -1 && selectedGirdPosiY == -1) {
		selectedGirdPosiX = _tappedPosiX;
		selectedGirdPosiY = _tappedPosiY;
		/*��paths��Ϊ��Ч״̬*/
		invalidatePaths(paths);
		/*����״̬Ϊѡ��ĳ����*/
		return LLKMLTP_SELECTED;
	}
	/*����ǰģ��������ѡ�з�������,���������ж�*/
	else{
		/*�ж�ѡ�з������������Ƿ�Ϊͬһ��*/
		bool b0 = ((*map)[selectedGirdPosiX][selectedGirdPosiY].type == (*map)[_tappedPosiX][_tappedPosiY].type);
		/*���������*/
		if (b0 && checkElimination(selectedGirdPosiX, selectedGirdPosiY, _tappedPosiX, _tappedPosiY, paths)) {
			/*���Ӵ���״̬��Ϊ��*/
			(*map)[selectedGirdPosiX][selectedGirdPosiY].exist = 0;
			(*map)[_tappedPosiX][_tappedPosiY].exist = 0;
			/*��ѡ�з����¼���*/
			selectedGirdPosiX = -1;
			selectedGirdPosiY = -1;
			/*ʣ��δ����������2*/
			this->remainingGirdNum -= 2;
			/*��paths��Ϊ��Ч״̬,���д洢������·����Ϣ*/
			validatePaths(paths);
			/*���������ɹ�*/
			return LLKMLTP_ELIMINATE_SUCCESS;
		}
		/*�����񲻿���*/
		else {
			/*��ѡ�з����¼ת����_tappedλ��*/
			selectedGirdPosiX = _tappedPosiX;
			selectedGirdPosiY = _tappedPosiY;
			/*��paths��Ϊ��Ч״̬*/
			invalidatePaths(paths);
			/*����ʧ��,����״̬Ϊѡ�з���ת�Ƶ�tappedλ��*/
			return LLKMLTP_SWITCH_SELECTED;
		}
	}
	/*����״̬�����ܵ�������*/
	LLKErrLog("FunctionERR -ImpossibleProcess | X: %03d, Y: %03d\n", _tappedPosiX, _tappedPosiY);
	return LLKMLTP_ERROR_STATUS;
}

///*����map��(gbPosiX, gbPosiY)������*/
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

/*����������*/
inline void swapInt(int32_t* _a,int32_t* _b) {
	int32_t tmp = *_a;
	*_a = *_b;
	*_b = tmp;
}

#define DEBUG_ON
/*�����(x, sy)Ϊ���,(x, ey)Ϊ�յ�Ĵ�ֱ·���ܷ���ͨ, �������Ӧ��֤ sy <= ey */
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

/*�����(sx, y)Ϊ���,(ex, y)Ϊ�յ��ˮƽ·���ܷ���ͨ, �������Ӧ��֤ sx <= ex*/
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

/*��ab��������������ж��㷨*/
bool LLKModel::checkElimination(int32_t aX, int32_t aY, int32_t bX, int32_t bY, Paths* paths) {
	/*���ҷ�ʽA
	 *����ֱ��y = aY��y = bYΪ��׼����·��(���ҷ�ʽ��ͼ)
	 */
	/*���߼�����,���A����B���Ϸ�,"����"A,B����
	 *���ɱ�֤B��һ����A���Ϸ�,��ϸ�����ͼ
	 */
	if (aY < bY) {
		swapInt(&aX, &bX);
		swapInt(&aY, &bY);
	}
	/*���ﲻ�ش���aY == bY�����(ԭ�����ϸ����)*/
	if (aY != bY) {
		
		/*Ѱ·��ʽ1:��B��ֱ������Ѱ��·��*/
		{
			/*��װB�㲻���ڷ���,�����Ѱ·��ʽδ�ҵ����,Ӧ��B��ָ�Ϊ���ڷ���״̬*/
			(*map)[bX][bY].exist = 0;
			
			/*����һ��·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽y = aY��һ��*/
			if (checkVerticalConnectivity(map, bX, bY, aY - 1)) {
				/*��һ��·����ͷΪA������*/
				if (bX == aX) {
					/*����A_0_0*/
					setPathNum(paths, 1);
					setP1(paths, bX, bY);
					setP2(paths, aX, aY);
					return true;
				}
				/*A���ڵ�һ��·����ͷ�������*/
				else if (aX < bX) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, bX)) {
						/*����A_0_-1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, bX, aY);
						setP3(paths, aX, aY);
						return true;
					}
				}
				/*A���ڵ�һ��·����ͷ�Ҳ�����*/
				else {
					/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, bX, aX - 1)) {
						/*����A_0_1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, bX, aY);
						setP3(paths, aX, aY);
						return true;
					}
				}
			}

			/*���û�н������,˵��Ѱ·��ʽ1�޽��·��,��B��ָ�Ϊ���ڷ���״̬*/
			(*map)[bX][bY].exist = 1;
		}

		/*Ѱ·��ʽ2:��B����࿪ʼ����,ֱ��xΪ0*/
		for (int32_t xi = bX - 1; xi >= 0;xi--) {
			/*bXΪ0ʱ,��Ϊ�����,�����б���*/
			if (bX == 0) {
				break;
			}
			/*��λ���з���,��ֹͣ�������*/
			if ((*map)[xi][bY].exist == 1) {
				break;
			}
			/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽y = aY��һ��*/
			if (checkVerticalConnectivity(map, xi, bY, aY - 1)) {
				/*�ڶ���·����ͷΪA������*/
				if (xi == aX) {
					/*����A_-1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, aX, bY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A���ڵڶ���·����ͷ�������*/
				else if (aX < xi) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, xi)) {
						/*����A_-1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX + 1, aY)Ϊ���,(xi, aY)Ϊ�յ��ˮƽ·������ͨ,�����һ��xi*/
						continue;
					}
				}
				/*A���ڵڶ���·����ͷ�Ҳ�����*/
				else {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, xi, aX - 1)) {
						/*����A_-1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(xi, aY)Ϊ���,(aX - 1, aY)Ϊ�յ��ˮƽ·������ͨ,�����һ��xi*/
						continue;
					}
				}
			}
			else {
				/*��(xi, bY)Ϊ���,(xi, aY - 1)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��xi*/
				continue;
			}
		}

		/*Ѱ·��ʽ3:��B���Ҳ࿪ʼ����,ֱ��xΪmapWidth-1(�����±����ֵ)*/
		for (int32_t xi = bX + 1; xi < this->mapWidth; xi++) {
			/*bXΪmapWidth-1ʱ,��Ϊ���Ҳ�,�����б���*/
			if (bX == this->mapWidth - 1) {
				break;
			}
			/*��λ���з���,��ֹͣ���ұ���*/
			if ((*map)[xi][bY].exist == 1) {
				break;
			}
			/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽y = aY��һ��*/
			if (checkVerticalConnectivity(map, xi, bY, aY - 1)) {
				/*�ڶ���·����ͷΪA������*/
				if (xi == aX) {
					/*����A_1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, aX, bY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A���ڵڶ���·����ͷ�������*/
				else if (aX < xi) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, aX + 1, xi)) {
						/*����A_1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX + 1, aY)Ϊ���,(xi, aY)Ϊ�յ��ˮƽ·������ͨ,�����һ��xi*/
						continue;
					}
				}
				/*A���ڵڶ���·����ͷ�Ҳ�����*/
				else {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��x = aX��һ��*/
					if (checkHorizontalConnectivity(map, aY, xi, aX - 1)) {
						/*����A_1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, xi, bY);
						setP3(paths, xi, aY);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(xi, aY)Ϊ���,(aX - 1, aY)Ϊ�յ��ˮƽ·������ͨ,�����һ��xi*/
						continue;
					}
				}
			}
			else {
				/*��(xi, bY)Ϊ���,(xi, aY - 1)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��xi*/
				continue;
			}
		}
	}
	

	/*���ҷ�ʽB
	*����ֱ��x = aX��x = bXΪ��׼����·��(���ҷ�ʽ��ͼ)
	*/
	/*���߼�����,���A����B����,"����"A,B����
	*���ɱ�֤B��һ����A����,��ϸ�����ͼ
	*/
	if (aX < bX) {
		swapInt(&aX, &bX);
		swapInt(&aY, &bY);
	}
	/*���ﲻ�ش���aX == bX�����(ԭ�����ϸ����)*/
	if (aX != bX) {
		/*Ѱ·��ʽ1:��B��ֱ������Ѱ��·��*/
		{
			/*��װB�㲻���ڷ���,�����Ѱ·��ʽδ�ҵ����,Ӧ��B��ָ�Ϊ���ڷ���״̬*/
			(*map)[bX][bY].exist = 0;

			/*����һ��·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽x = aX��һ��*/
			if (checkHorizontalConnectivity(map, bY, bX, aX - 1)) {
				/*��һ��·����ͷΪA������*/
				if (bY == aY) {
					/*����B_0_0*/
					setPathNum(paths, 1);
					setP1(paths, bX, bY);
					setP2(paths, aX, aY);
					return true;
				}
				/*A���ڵ�һ��·����ͷ�ϲ�����*/
				else if (aY < bY) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aX��һ��*/
					if (checkVerticalConnectivity(map, aX, aY + 1, bY)) {
						/*����B_0_-1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, aX, bY);
						setP3(paths, aX, aY);
						return true;
					}
				}
				/*A���ڵ�һ��·����ͷ�²�����*/
				else {
					/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aY��һ��*/
					if (checkVerticalConnectivity(map, aX, bY, aY - 1)) {
						/*����B_0_1*/
						setPathNum(paths, 2);
						setP1(paths, bX, bY);
						setP2(paths, aX, bY);
						setP3(paths, aX, aY);
						return true;
					}
				}
			}

			/*���û�н������,˵��Ѱ·��ʽ1�޽��·��,��B��ָ�Ϊ���ڷ���״̬*/
			(*map)[bX][bY].exist = 1;
		}

		/*Ѱ·��ʽ2:��B���ϲ࿪ʼ����,ֱ��yΪ0*/
		for (int32_t yj = bY - 1; yj >= 0; yj--) {
			/*bYΪ0ʱ,��Ϊ���ϲ�,�����б���*/
			if (bY == 0) {
				break;
			}
			/*��λ���з���,��ֹͣ���ϱ���*/
			if ((*map)[bX][yj].exist == 1) {
				break;
			}
			/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽x = aX��һ��*/
			if (checkHorizontalConnectivity(map, yj, bX, aX - 1)) {
				/*�ڶ���·����ͷΪA������*/
				if (yj == aY) {
					/*����B_-1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, bX, aY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A���ڵڶ���·����ͷ�ϲ�����*/
				else if (aY < yj) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aY��һ��*/
					if (checkVerticalConnectivity(map, aX, aY + 1, yj)) {
						/*����B_-1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX, aY + 1)Ϊ���,(aX, yj)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��yj*/
						continue;
					}
				}
				/*A���ڵڶ���·����ͷ�²�����*/
				else {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aY��һ��*/
					if (checkVerticalConnectivity(map, aX, yj, aY - 1)) {
						/*����B_-1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX, yj)Ϊ���,(aX, aY - 1)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��yj*/
						continue;
					}
				}
			}
			else {
				/*��(bX, yj)Ϊ���,(aX - 1, yj)Ϊ�յ��ˮƽ·������ͨ,�����һ��yj*/
				continue;
			}
		}

		/*Ѱ·��ʽ3:��B���²࿪ʼ����,ֱ��xΪmapHeight-1(�����±����ֵ)*/
		for (int32_t yj = bY + 1; yj < this->mapHeight; yj++) {
			/*bYΪmapHeight-1ʱ,��Ϊ���²�,�����б���*/
			if (bY == mapHeight - 1) {
				break;
			}
			/*��λ���з���,��ֹͣ���±���*/
			if ((*map)[bX][yj].exist == 1) {
				break;
			}
			/*���ڶ���·���Ƿ���ͨ,ע�����ﲻ�ؼ�鵽x = aX��һ��*/
			if (checkHorizontalConnectivity(map, yj, bX, aX - 1)) {
				/*�ڶ���·����ͷΪA������*/
				if (yj == aY) {
					/*����B_1_0*/
					setPathNum(paths, 2);
					setP1(paths, bX, bY);
					setP2(paths, bX, aY);
					setP3(paths, aX, aY);
					return true;
				}
				/*A���ڵڶ���·����ͷ�ϲ�����*/
				else if (aY < yj) {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aY��һ��*/
					if (checkVerticalConnectivity(map, aX, aY + 1, yj)) {
						/*����B_1_-1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX, aY + 1)Ϊ���,(aX, yj)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��yj*/
						continue;
					}
				}
				/*A���ڵڶ���·����ͷ�²�����*/
				else {
					/*��������·���Ƿ���ͨ,ע�����ﲻ�ؼ��y = aY��һ��*/
					if (checkVerticalConnectivity(map, aX, yj, aY - 1)) {
						/*����B_1_1*/
						setPathNum(paths, 3);
						setP1(paths, bX, bY);
						setP2(paths, bX, yj);
						setP3(paths, aX, yj);
						setP4(paths, aX, aY);
						return true;
					}
					else {
						/*��(aX, yj)Ϊ���,(aX, aY - 1)Ϊ�յ�Ĵ�ֱ·������ͨ,�����һ��yj*/
						continue;
					}
				}
			}
			else {
				/*��(bX, yj)Ϊ���,(aX - 1, yj)Ϊ�յ��ˮƽ·������ͨ,�����һ��yj*/
				continue;
			}
		}
	}

	return false;
}

/*����map���ݵ��ļ�(ForDebug)*/
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
 *����map
 *map��ÿ�����񼸺�λ���Լ�����״̬������Ӱ��,���ҵ���ÿ������typeֵ
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

	/*��ȡmap�Ϸ�������*/
	int32_t girdBoxNum = this->mapWidth * this->mapHeight;

	/*Ϊ����rawData�����ڴ�ռ�*/
	uint16_t* rawData = new uint16_t[remainingGirdNum];

	/*��ʼ������rawDataΪȫ0*/
	memset(rawData, 0, sizeof(uint16_t) * remainingGirdNum);

	/*��ȡArray2D���ʵ��map�еײ������ڴ�ռ��׵�ַ*/
	GirdBox* p = (GirdBox*)map->getMemBlockPtr();

	/*����map�����з���,��exist����Ϊ1�ķ����typeֵ��������rawData��*/
	for (int32_t i = 0, j = 0; i < girdBoxNum && j < remainingGirdNum; i++) {
		if (p[i].exist == 1) {
			rawData[j] = p[i].type;
			j++;
		}
	}

	/*�������rawData����*/
	shuffle(rawData, remainingGirdNum);

	/*����map�����з���,�����Һ��rawData�����е�Ԫ������д��exist����Ϊ1�ķ���*/
	for (int32_t i = 0, j = 0; i < girdBoxNum && j < remainingGirdNum; i++) {
		if (p[i].exist == 1) {
			p[i].type = rawData[j];
			j++;
		}
	}

	/*�����ڴ�*/
	delete[] rawData;
}