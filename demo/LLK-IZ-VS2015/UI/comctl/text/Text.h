#ifndef _TEXT_H_
#define _TEXT_H_

#include "..\..\WindowUtil.h"
#include <string>

class Text
{
public:
	Text();

	Text(char* _srcA);
	Text(wchar_t* _srcW);
	Text(std::string& _srcStringA);
	Text(std::wstring& _srcStringW);
	virtual ~Text();
	
	void setText(char* _srcA);
	void setText(wchar_t* _srcW);
	void setText(std::string& _srcStringA);
	void setText(std::wstring& _srcStringW);
	
	void draw();
	void draw(int _x, int _y);
	void draw(Gdiplus::Graphics* _graphics);
	void draw(Gdiplus::Graphics* _graphics, int _x, int _y);

	void setPosi(int _x, int _y);
	void setGDIGraphics(Gdiplus::Graphics* _graphics);

	void setFontName(char* _fontNameA);
	void setFontName(wchar_t* _fontNameW);
	void setFontName(std::string& _fontNameA);
	void setFontName(std::wstring& _fontNameW);

	void setFontSize(int _fontSize);
	void setFontStyle(Gdiplus::FontStyle _fontStyle);
	void setStringAlignment(Gdiplus::StringAlignment _stringAlignment);

	void setColor(Gdiplus::Color& _color);
	void setColor(int _R, int _G, int _B);
	void setColor(int _A, int _R, int _G, int _B);

	void measureTextOutputArea(Gdiplus::RectF& rect);

	std::string getTextA();
	std::wstring getTextW();

	int getPosiX();
	int getPosiY();

	Gdiplus::Graphics* getGDIGraphics();

	int getFontSize();
	Gdiplus::Color getColor();
	Gdiplus::StringAlignment getStringAlignment();


private:
	std::string srcStringA;
	std::wstring srcStringW;
	std::string fontNameA;
	std::wstring fontNameW;
	int length;
	int rgbcolor;
	Gdiplus::Color color;
	int fontSize;
	int x, y;
	Gdiplus::Graphics* graphics;
	void drawProcess();
	void init();
	Gdiplus::RectF rectf;
	//Gdiplus::FontFamily fontFamily;
	/*字体样式:正常,加粗,倾斜等*/
	Gdiplus::FontStyle fontStyle;
	/*文本对齐方式:左对齐,居中等*/
	Gdiplus::StringAlignment stringAlignment;
};



#endif // !_TEXT_H_