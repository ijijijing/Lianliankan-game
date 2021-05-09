#ifndef _PATHS_H_
#define _PATHS_H_

#include <cstdint>
/*消子路径结构体,最多三条路径,最多四个点*/
#pragma pack(2)
typedef struct Paths
{
	int32_t pathNum;
	int32_t p1x, p1y;
	int32_t p2x, p2y;
	int32_t p3x, p3y;
	int32_t p4x, p4y;
	int32_t useTimes;
}PathsInfo, Paths;
#pragma pack()

const int32_t INVALIDPATHSCODE = -1;
const int32_t VALIDPATHSCODE = 1;

void invalidatePaths(Paths*);
void validatePaths(Paths*);
void setUseTimes(Paths*, int32_t);
void setPathNum(Paths*, int32_t);
void setP1(Paths*, int32_t, int32_t);
void setP2(Paths*, int32_t, int32_t);
void setP3(Paths*, int32_t, int32_t);
void setP4(Paths*, int32_t, int32_t);
#endif // !_PATHS_H_

