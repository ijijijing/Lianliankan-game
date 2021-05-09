#include "Paths.h"

void invalidatePaths(Paths* paths) {
	if (paths == NULL) {
		return;
	}
	paths->pathNum = INVALIDPATHSCODE;
}

void validatePaths(Paths* paths) {
	if (paths == NULL) {
		return;
	}
	/*这里暂时没什么用,只要pathNum不为无效码以及其值大于0即认定有效*/
	if (paths->pathNum != INVALIDPATHSCODE && paths->pathNum > 0) {
		paths->pathNum = paths->pathNum;
	}
}

void setUseTimes(Paths* paths, int32_t useTimes) {
	if (paths == NULL) {
		return;
	}
	paths->useTimes = useTimes;
}

void setPathNum(Paths* paths, int32_t pathNum) {
	if (paths == NULL) {
		return;
	}
	paths->pathNum = pathNum;
}

void setP1(Paths* paths, int32_t _p1x, int32_t _p1y) {
	if (paths == NULL) {
		return;
	}
	paths->p1x = _p1x;
	paths->p1y = _p1y;
}

void setP2(Paths* paths, int32_t _p2x, int32_t _p2y) {
	if (paths == NULL) {
		return;
	}
	paths->p2x = _p2x;
	paths->p2y = _p2y;
}

void setP3(Paths* paths, int32_t _p3x, int32_t _p3y) {
	if (paths == NULL) {
		return;
	}
	paths->p3x = _p3x;
	paths->p3y = _p3y;
}

void setP4(Paths* paths, int32_t _p4x, int32_t _p4y) {
	if (paths == NULL) {
		return;
	}
	paths->p4x = _p4x;
	paths->p4y = _p4y;
}

