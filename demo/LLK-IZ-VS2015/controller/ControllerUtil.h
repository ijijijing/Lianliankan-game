#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "..\LLKUtil.h"
#include "..\UI\WindowUtil.h"
#include "..\model\LLKModel.h"

#define DEFAULT_MAX_PATHUSETIMES 10

enum ActionType
{
	GC_MOUSEMOVE = WM_MOUSEMOVE,
	GC_MOUSELBDW = WM_LBUTTONDOWN,
	GC_MOUSERBDW = WM_RBUTTONDOWN,
	GC_MOUSELBUP = WM_LBUTTONUP,
	GC_MOUSERBUP = WM_RBUTTONUP,
};

typedef struct _MouseEvent {
	ActionType actionType;
	void* param1;
	void* param2;
}_MouseEvent, MouseEvent;

enum GAMESTATUS {
	G_WIN,
	G_PLAYING,
	G_LOSE,
};
#endif // !_CONTROLLER_H_


