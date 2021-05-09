#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "..\..\WindowUtil.h"
#include "..\..\MouseUtil.h"
#include "..\text\Text.h"
#include "..\border\Border.h"

typedef void (*BCallBack)();

class Button
{
public:
	
	Button();
	Button(Text& _text);
	virtual ~Button();
	void draw();
	void draw(int _x, int _y);
	void draw(int _x, int _y, int _width, int _height);
	void draw(Gdiplus::Graphics* _graphics);
	void draw(Gdiplus::Graphics* _graphics, int _x, int _y);
	void draw(Gdiplus::Graphics* _graphics, int _x, int _y, int _width, int _height);
	void trigger();

	int onMouseEventProcess(MouseLRBtnGeoStatus* _mlrbgs);
	void setOnClickCallBack(BCallBack _callBack);
	
	void enableDisplayBackground(bool _b);
	void setPressedColor(int _A, int _R, int _G, int _B);
	void setFlittedColor(int _A, int _R, int _G, int _B);
	void setBackgroundColor(int _A, int _R, int _G, int _B);

	void enableDisplayBorder(bool _b);
	void setShapeType(BorderType _borderType);
	void setGDIGraphics(Gdiplus::Graphics* _graphics);
	void setPosi(int _x, int _y);
	void setSize(int _width, int _height);
	
	int getPosiX();
	int getPosiY();
	int getPosiWidth();
	int getPosiHeight();
	BorderType getShapeType();
	Gdiplus::Graphics* getGDIGraphics();
	
	void setText(char* _srcA);
	void setText(wchar_t* _srcW);
	void setText(std::string& _srcStringA);
	void setText(std::wstring& _srcStringW);
	
	void setFontName(char* _fontNameA);
	void setFontName(wchar_t* _fontNameW);
	void setFontName(std::string& _fontNameA);
	void setFontName(std::wstring& _fontNameW);

	void setFontSize(int _fontSize);
	void setFontStyle(Gdiplus::FontStyle _fontStyle);
	void setStringAlignment(Gdiplus::StringAlignment _stringAlignment);

	void setTextColor(Gdiplus::Color& _color);
	void setTextColor(int _R, int _G, int _B);
	void setTextColor(int _A, int _R, int _G, int _B);

	void setPressedTextColor(int _A, int _R, int _G, int _B);
	void setFlittedTextColor(int _A, int _R, int _G, int _B);

	std::string getTextA();
	std::wstring getTextW();

	int getTextPosiX();
	int getTextPosiY();
	int getFontSize();
	Gdiplus::Color getTextColor();

	void setBorderStroke(float _f);
	void setBorderRectRadius(int _rectRadius);
	void setBorderColor(Gdiplus::Color& _color);
	void setBorderColor(int _R, int _G, int _B);
	void setBorderColor(int _A, int _R, int _G, int _B);

	float getBorderStroke(float _f);
	int getBorderRectRadius();
	Gdiplus::Color getBorderColor(Gdiplus::Color& _color);

protected:
	Gdiplus::Graphics* graphics;
	Gdiplus::RectF rect;
	/*点击事件回调函数指针*/
	BCallBack callBack;
	int x, y;
	int width, height;
	Text text;
	/*int tarA, tarR, tarG, tarB;
	int curA, curR, curG, curB;*/
	bool displayBorder;
	bool displayBackground;
	
	Gdiplus::Color pressedColor;
	Gdiplus::Color flittedColor;
	Gdiplus::Color normalColor;
	Gdiplus::Color backgroundColor;

	Gdiplus::Color pressedTextColor;
	Gdiplus::Color flittedTextColor;
	Gdiplus::Color normalTextColor;
	Gdiplus::Color textColor;

	Border border;
	BorderType borderType;

	void init();
	void drawProcess();
	void calcTextPosi();
	void calcBorderPosi();
};

#endif // !_BUTTON_H_

