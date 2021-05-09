#pragma once

#ifdef _WIN32

#ifndef WIN32CSUTILS_H

#define WIN32CSUTILS_H

#include <string>
#include <Windows.h>

/*GBKתUTF-8,����ϵͳ����Ϊ��������GBK*/
extern std::string WINAPI GBK2UTF8(const std::string& strGbk);

/*UTF-8תGBK,����ϵͳ����Ϊ��������*/
extern std::string WINAPI UTF82GBK(const std::string& strUtf8);

/*���ֽ�UTF8�ı�ת˫�ֽ��ı�����,Ŀ���ַ�����ϵͳ�����й�,��������ΪGBK*/
extern std::wstring WINAPI UTF82WCS(const char* utf8charPtr);

/*���ֽ�GBK�ı�ת˫�ֽ��ı�����,Ŀ���ַ�����ϵͳ�����й�,��������ΪGBK*/
//extern std::wstring WINAPI UTF82WCS(const char* asciiCharPtr);




#endif // !WIN32CSUTILS_H

#endif // !_WIN32