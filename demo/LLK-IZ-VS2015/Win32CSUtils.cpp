#include "Win32CSUtils.h"
#include <atlconv.h>

std::string WINAPI GBK2UTF8(const std::string& strGbk)//传入的strGbk是GBK编码  
{
	//gbk转unicode  
	int len = MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, NULL, 0);
	wchar_t *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, strUnicode, len);

	//unicode转UTF-8  
	len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char * strUtf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

	std::string strTemp(strUtf8);//此时的strTemp是UTF-8编码  
	delete[]strUnicode;
	delete[]strUtf8;
	strUnicode = NULL;
	strUtf8 = NULL;
	return strTemp;
}

std::string WINAPI UTF82GBK(const std::string& strUtf8)
{
	//UTF-8转unicode  
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	wchar_t * strUnicode = new wchar_t[len];//len = 2  
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, strUnicode, len);

	//unicode转gbk  
	len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char *strGbk = new char[len];//len=3 本来为2，但是char*后面自动加上了\0  
	memset(strGbk, 0, len);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strGbk, len, NULL, NULL);
	
	std::string strTemp(strGbk);//此时的strTemp是GBK编码  
	delete[]strUnicode;
	delete[]strGbk;
	strUnicode = NULL;
	strGbk = NULL;
	return strTemp;
}

std::wstring WINAPI UTF82WCS(const char* utf8charPtr)
{
	//预转换，得到所需空间的大小;
	__int32 wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, utf8charPtr, strlen(utf8charPtr), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, utf8charPtr, strlen(utf8charPtr), wszString, wcsLen);
	
	//最后加上'\0'
	wszString[wcsLen] = '\0';
	std::wstring unicodeString(wszString);
	const wchar_t* ddd = unicodeString.c_str();
	delete[] wszString;
	wszString = NULL;
	return unicodeString;
}