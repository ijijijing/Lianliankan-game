#pragma once
#include <cassert>
#include <cstdio>
#define TDAERR(err) printf("%s:%d: failed with %s\n", __FILE__, __LINE__, err)

/*
 * ģ����Array2D
 * �������������ڴ�ռ䲢�������ȴ洢�ķ�ʽ�洢Ԫ��
 * ������ [] �����,���Է����ͨ��"array[i][j]"��ʽ����Ԫ��
 * Ҳ����ֱ�ӻ�ȡ�ڴ��ַ,��Ԫ�ش洢�ڴ�ռ����ֱ�Ӳ���,��һά����һ��ֱ������
 * ʹ��ʱע�⴫����������еķ�Χ���ܹ���,������ռ�Ϊ4GB
 */
template <typename T>
class Array2D
{
public:

	Array2D(size_t _x, size_t _y);
	~Array2D();
	T* operator[](size_t _x);
	void dumpMemData(char* fileName);
	const T* getMemBlockPtr();
	void getXY(int32_t& _x, int32_t& _y);
private:

	int status;
	size_t x, y;
	size_t TSize;
	void* memblockptr;
	size_t arraytotalnum;
	size_t memblocksize;
	void tdaMalloc();
	void tdaRecycle();
};

template <typename T>
Array2D<T>::Array2D(size_t _x, size_t _y)
{
	if (1 > _x || 1 > _y) {
		this->status = -1;
		TDAERR("-1");
		return;
	}
	if (1048576 < _x || 1048576 < _y) {
		this->status = -2;
		TDAERR("-2");
		return;
	}
	if (4294967296 < _x * _y) {
		this->status = -3;
		TDAERR("-3");
		return;
	}
	this->TSize = sizeof(T);
	if (4294967296 < _x * _y * this->TSize) {
		this->status = -4;
		TDAERR("-4");
		return;
	}
	this->status = 0;
	this->x = _x;
	this->y = _y;
	tdaMalloc();
}

template <typename T>
Array2D<T>::~Array2D()
{
	/*��������ʱ�����ѷ����ڴ�*/
	tdaRecycle();
}

template <typename T>
void Array2D<T>::tdaMalloc() {
	if (status != 0) {
		return;
	}
	this->arraytotalnum = this->x * this->y;
	this->memblocksize = this->arraytotalnum * this->TSize;
	this->memblockptr = malloc(memblocksize);
	memset(this->memblockptr, 0, memblocksize);
}

template <typename T>
void Array2D<T>::tdaRecycle() {
	status = -1;
	if (this->memblockptr != NULL) {
		free(this->memblockptr);
	}
	this->memblockptr = NULL;
	this->x = 0;
	this->y = 0;
	this->arraytotalnum = 0;
	this->memblocksize = 0;
}

template <typename T>
T* Array2D<T>::operator[](size_t _x) {
	//�������ж�Ӱ��������ʱע��
	/*
	if (_x < 0 || _x > 1048576 || memblockptr == NULL) {
	TDAERR("Value of _x is illegal OR memblockptr is NULL");
	assert(0);
	}
	*/

	if (_x < 0) {
		printf("Value of _x is illegal: _x: %u\n",_x);
		assert(0);
	}
	assert(this->memblockptr);
	/*
	* ����д��:
	* (XXXXXXX) return (T*)this->memblockptr + this->y * _x * this->TSize; (XXXXXXX)
	* ģ��չ����,ֱ�Ӷ�ָ��Ӽ��������ָ�������ʹ�С�ƶ�,����Ҫ�ٴ�*this->TSize.S
	*/
	return (T*)this->memblockptr + this->y * _x;
}

template <typename T>
void Array2D<T>::dumpMemData(char* fileName) {
	if (fileName == NULL || memblockptr == NULL) {
		return;
	}
	FILE* dumpFilePtr = fopen(fileName, "w+");
	if (dumpFilePtr == NULL) {
		printf("DUMPFILE write failed. FileName: %s", fileName);
	}
	fprintf(dumpFilePtr, "class Array2D Memory Dump File\n");
	fprintf(dumpFilePtr, "memblocksize: % 8uByte    TSize : % 8uByte\n", memblocksize, TSize);
	fprintf(dumpFilePtr, "x: %8u      y: %8u\n", x, y);
	unsigned char* p = (unsigned char*)memblockptr;
	unsigned char* q = (unsigned char*)memblockptr + memblocksize;
	size_t i = 0;
	while (p != q) {
		i++;
		fprintf(dumpFilePtr, "%02x ", (*p));
		if (i % 32 == 0) {
			fprintf(dumpFilePtr, "\n");
		}
		p++;
	}
	fflush(dumpFilePtr);
	fclose(dumpFilePtr);
}

template <typename T>
const T* Array2D<T>::getMemBlockPtr() {
	return (T*)memblockptr;
}

template <typename T>
void Array2D<T>::getXY(int32_t& _x, int32_t& _y) {
	_x = this->x;
	_y = this->y;
}