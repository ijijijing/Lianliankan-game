#ifndef _MOUSEUTIL_H_
#define _MOUSEUTIL_H_

#include <Windows.h>

typedef struct MouseLRBtnGeoStatus {
	/*鼠标指针位置(x,y)*/
	int mpx = 0;
	int mpy = 0;
	/*鼠标上一次左键按下位置LeftDownPointX (x,y)*/
	int ldpx = 0;
	int ldpy = 0;
	/*鼠标上一次右键按下位置RightDownPointX (x,y)*/
	int rdpx = 0;
	int rdpy = 0;
	/*鼠标上一次左键释放位置LeftUpPointX (x,y)*/
	int lupx = 0;
	int lupy = 0;
	/*鼠标上一次右键释放位置RightUpPointX (x,y)*/
	int rupx = 0;
	int rupy = 0;
	/*鼠标左右键当前状态: 是否处于按下状态*/
	bool lbs = 0;
	bool rbs = 0;
	/*鼠标左右键上一次状态: 是否处于按下状态*/
	bool prelbs = 0;
	bool prerbs = 0;
} MouseLRBtnGeoStatus;

#endif // !_MOUSEUTIL_H_
