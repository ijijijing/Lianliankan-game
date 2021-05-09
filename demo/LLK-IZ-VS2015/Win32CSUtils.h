#pragma once

#ifdef _WIN32

#ifndef WIN32CSUTILS_H

#define WIN32CSUTILS_H

#include <string>
#include <Windows.h>

/*GBK转UTF-8,仅限系统语言为简体中文GBK*/
extern std::string WINAPI GBK2UTF8(const std::string& strGbk);

/*UTF-8转GBK,仅限系统语言为简体中文*/
extern std::string WINAPI UTF82GBK(const std::string& strUtf8);

/*单字节UTF8文本转双字节文本方法,目标字符集与系统语言有关,本程序中为GBK*/
extern std::wstring WINAPI UTF82WCS(const char* utf8charPtr);

/*单字节GBK文本转双字节文本方法,目标字符集与系统语言有关,本程序中为GBK*/
//extern std::wstring WINAPI UTF82WCS(const char* asciiCharPtr);




#endif // !WIN32CSUTILS_H

#endif // !_WIN32