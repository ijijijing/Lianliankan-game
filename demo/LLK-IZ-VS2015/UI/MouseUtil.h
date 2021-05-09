#ifndef _MOUSEUTIL_H_
#define _MOUSEUTIL_H_

#include <Windows.h>

typedef struct MouseLRBtnGeoStatus {
	/*���ָ��λ��(x,y)*/
	int mpx = 0;
	int mpy = 0;
	/*�����һ���������λ��LeftDownPointX (x,y)*/
	int ldpx = 0;
	int ldpy = 0;
	/*�����һ���Ҽ�����λ��RightDownPointX (x,y)*/
	int rdpx = 0;
	int rdpy = 0;
	/*�����һ������ͷ�λ��LeftUpPointX (x,y)*/
	int lupx = 0;
	int lupy = 0;
	/*�����һ���Ҽ��ͷ�λ��RightUpPointX (x,y)*/
	int rupx = 0;
	int rupy = 0;
	/*������Ҽ���ǰ״̬: �Ƿ��ڰ���״̬*/
	bool lbs = 0;
	bool rbs = 0;
	/*������Ҽ���һ��״̬: �Ƿ��ڰ���״̬*/
	bool prelbs = 0;
	bool prerbs = 0;
} MouseLRBtnGeoStatus;

#endif // !_MOUSEUTIL_H_
