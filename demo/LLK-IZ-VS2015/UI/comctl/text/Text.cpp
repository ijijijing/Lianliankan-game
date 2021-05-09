#include "Text.h"
#include "..\..\WindowUtil.h"

Text::Text() {
	init();
}

Text::Text(char* _srcA) {
	init();
	setText(_srcA);
}

Text::Text(wchar_t* _srcW) {
	init();
	setText(_srcW);
}

Text::Text(std::string& _srcStringA) {
	init();
	setText(_srcStringA);
}

Text::Text(std::wstring& _srcStringW) {
	init();
	setText(_srcStringW);
}

Text::~Text() {

}

void Text::setText(char* _srcA) {
	srcStringA = _srcA;
	//measureTextOutputArea(this->rectf);
}

void Text::setText(wchar_t* _srcW) {
	srcStringW = _srcW;
	//measureTextOutputArea(this->rectf);
}

void Text::setText(std::string& _srcStringA) {
	srcStringA = _srcStringA;
	//measureTextOutputArea(this->rectf);
}

void Text::setText(std::wstring& _srcStringW) {
	srcStringW = _srcStringW;
	//measureTextOutputArea(this->rectf);
}

void Text::draw() {
	drawProcess();
}

void Text::draw(int _x, int _y) {
	setPosi(_x, _y);
	drawProcess();
}

void Text::draw(Gdiplus::Graphics* _graphics) {
	setGDIGraphics(_graphics);
	drawProcess();
}

void Text::draw(Gdiplus::Graphics* _graphics, int _x, int _y) {
	setGDIGraphics(_graphics);
	setPosi(_x, _y);
	drawProcess();
}

void Text::setGDIGraphics(Gdiplus::Graphics* _graphics) {
	this->graphics = _graphics;
	//measureTextOutputArea(this->rectf);
}

void Text::setFontName(char* _fontNameA) {
	this->fontNameA = _fontNameA;
	//measureTextOutputArea(this->rectf);
}

void Text::setFontName(wchar_t* _fontNameW) {
	this->fontNameW = _fontNameW;
	//measureTextOutputArea(this->rectf);
}

void Text::setFontName(std::string& _fontNameA) {
	this->fontNameA = _fontNameA;
	//measureTextOutputArea(this->rectf);
}

void Text::setFontName(std::wstring& _fontNameW) {
	this->fontNameW = _fontNameW;
	//measureTextOutputArea(this->rectf);
}


void Text::setFontSize(int _size) {
	if (fontSize < 1 || fontSize > 500) {
		return;
	}
	this->fontSize = _size;
	//measureTextOutputArea(this->rectf);
}

void Text::setFontStyle(Gdiplus::FontStyle _fontStyle) {
	this->fontStyle = _fontStyle;
	//measureTextOutputArea(this->rectf);
}

void Text::setStringAlignment(Gdiplus::StringAlignment _stringAlignment) {
	this->stringAlignment = _stringAlignment;
}

void Text::setColor(Gdiplus::Color& _color) {
	this->color = Gdiplus::Color(_color);
}

void Text::setColor(int _R, int _G, int _B) {
	this->color = Gdiplus::Color(_R, _G, _B);
}

void Text::setColor(int _A, int _R, int _G, int _B) {
	this->color = Gdiplus::Color(_A, _R, _G, _B);
}

void Text::setPosi(int _x, int _y) {
	this->x = _x;
	this->y = _y;
}

void Text::measureTextOutputArea(Gdiplus::RectF& rect) {
	if (graphics == NULL) {
		AppGlobalLogE("void Text::measureTextOutputArea(): graphics is NULL!\n");
		assert(0);
		return;
	}
	Gdiplus::FontFamily fontFamily(this->fontNameW.c_str());
	Gdiplus::Font font(&fontFamily, this->fontSize, this->fontStyle, UnitPixel);
	Gdiplus::PointF pointf(this->x, this->y);
	graphics->MeasureString(srcStringW.c_str(), srcStringW.length(), &font, pointf, &rect);
	if (srcStringW.length() > 1) {
		//AppGlobalLogI("MeasureString |Width: %6.2f px, Height: %6.2f px, Text: %S", rect.Width, rect.Height, srcStringW.c_str());
		//AppGlobalLogI("\n");
	}
}

std::string Text::getTextA() {
	return this->srcStringA;
}

std::wstring Text::getTextW() {
	return this->srcStringW;
}

int Text::getPosiX() {
	return this->x;
}

int Text::getPosiY() {
	return this->y;
}

Gdiplus::Graphics* Text::getGDIGraphics() {
	return this->graphics;
}

int Text::getFontSize() {
	return this->fontSize;
}

Gdiplus::Color Text::getColor() {
	return this->color;
}

Gdiplus::StringAlignment Text::getStringAlignment() {
	return this->stringAlignment;
}

void Text::init() {
	this->fontSize = 15;
	this->color = Gdiplus::Color();
	this->x = 0;
	this->y = 0;
	this->srcStringA = "";
	this->srcStringW = L"";
	this->fontNameA = "Î¢ÈíÑÅºÚ";
	this->fontNameW = L"Î¢ÈíÑÅºÚ";
	this->length = 0;
	this->graphics = NULL;
	this->fontStyle = Gdiplus::FontStyleRegular;
	this->stringAlignment = Gdiplus::StringAlignmentNear;
	/*this->rectf.Width = 0;
	this->rectf.Height = 0;
	this->rectf.X = this->x;
	this->rectf.Y = this->y;*/
}

void Text::drawProcess() {
	if (graphics == NULL) {
		AppGlobalLogE("void Text::drawProcess(): graphics is NULL!\n");
		assert(0);
		return;
	}
	
	Gdiplus::FontFamily fontFamily(this->fontNameW.c_str());
	Gdiplus::Font font(&fontFamily, this->fontSize, this->fontStyle, UnitPixel);
	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(this->stringAlignment);
	stringFormat.SetLineAlignment(StringAlignmentCenter);
	Gdiplus::SolidBrush brush(this->color);
	Gdiplus::PointF pointf(this->x, this->y);
	graphics->DrawString(this->srcStringW.c_str(), this->srcStringW.length(), &font, pointf, &stringFormat, &brush);
	measureTextOutputArea(this->rectf);
}

