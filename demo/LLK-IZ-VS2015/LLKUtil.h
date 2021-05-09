#ifndef _LLKUTIL_H_
#define _LLKUTIL_H_

#include <assert.h>
#include <ctime>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory.h>

#define LLKErrLog printf
#define LLKInfLog printf
#define AppGlobalLogE printf
#define AppGlobalLogI printf

//int _cdecl LLKErrLog(char const* const _Format, ...);
//int _cdecl LLKInfLog(char const* const _Format, ...);
void LLKApplicationExit(int exitcode, const char* exitMsg);
//void shuffle(int32_t*, int32_t);

template <typename T>
void shuffle(T* arr, int32_t sz) {
	if (sz < 2 || arr == NULL) {
		return;
	}
	int32_t i = sz;
	int32_t j = 0;
	T temp = 0;
	srand((unsigned int)time(NULL));
	while (--i) {
		j = rand() % (i + 1);
		temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}

#endif // !_LLKUTIL_H_
