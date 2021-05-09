#include "LLKUtil.h"
//int _cdecl LLKErrLog(char const* const _Format, ...) {
//	auto p = printf;
//	(*p)(_Format);
//	return 0;
//}
//
//int _cdecl LLKInfLog(char const* const _Format, ...) {
//	auto p = printf;
//	(*p)(_Format);
//	return 0;
//}

void LLKApplicationExit(int exitcode, const char* exitMsg) {
	printf("LLKApplicationExit, Code: %d, Message: %s", exitcode, exitMsg);
	printf("\n");
	exit(-1);
	//assert(0);
}

//template <typename T>
//void shuffle(T* arr, int32_t sz) {
//	if (sz < 2 || arr == NULL) {
//		return;
//	}
//	int32_t i = sz;
//	int32_t j = 0;
//	T temp = 0;
//	srand(time(NULL));
//	while (--i) {
//		j = rand() % (i + 1);
//		temp = arr[i];
//		arr[i] = arr[j];
//		arr[j] = temp;
//	}
//}