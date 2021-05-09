#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	init();
}

ProgressBar::~ProgressBar()
{

}

/*ProgressBar自身绘制相关*/

void ProgressBar::draw() {
	drawProcess();
}

void ProgressBar::draw(int _x, int _y) {
	setPosi(_x, _y);
	drawProcess();
}

void ProgressBar::draw(int _x, int _y, int _width, int _height) {
	setPosi(_x, _y);
	setSize(_width, _height);
	drawProcess();
}

void ProgressBar::draw(Gdiplus::Graphics* _graphics) {
	setGDIGraphics(_graphics);
	drawProcess();
}

void ProgressBar::draw(Gdiplus::Graphics* _graphics, int _x, int _y) {
	setGDIGraphics(_graphics);
	setPosi(_x, _y);
	drawProcess();
}

void ProgressBar::draw(Gdiplus::Graphics* _graphics, int _x, int _y, int _width, int _height) {
	setGDIGraphics(_graphics);
	setPosi(_x, _y);
	setSize(_width, _height);
	drawProcess();
}

void ProgressBar::trigger() {
	if (this->progressPercentage == 0) {
		return;
	}
	this->progressPercentage--;
	AppGlobalLogI("void ProgressBar::trigger(): progressPercentage: %d\n", progressPercentage);
	if (this->progressPercentage == 0 && this->progressFinishCallBack != NULL) {
		AppGlobalLogI("void ProgressBar::trigger(): progressFinishCallBack is Called\n");
		this->progressFinishCallBack();
	}
}

void ProgressBar::trigger(int percentage) {
	setProgressPercentage(percentage);
	AppGlobalLogI("void ProgressBar::trigger(): progressPercentage: %d\n", progressPercentage);
	if (this->progressPercentage == 0 && this->progressFinishCallBack != NULL) {
		AppGlobalLogI("void ProgressBar::trigger(): progressFinishCallBack is Called\n");
		this->progressFinishCallBack();
	}
}

void ProgressBar::setProgressPercentage(int _progressPercentage) {
	if (_progressPercentage < 0) {
		_progressPercentage = 0;
		AppGlobalLogE("void ProgressBar::setProgressPercentage(): _progressPercentage is small than 0: value: %d", _progressPercentage);
		AppGlobalLogE("\n");
	}
	if (_progressPercentage > 100) {
		_progressPercentage = 100;
		AppGlobalLogE("void ProgressBar::setProgressPercentage(): _progressPercentage is big than 100: value: %d", _progressPercentage);
		AppGlobalLogE("\n");
	}
	this->progressPercentage = _progressPercentage;
}

int ProgressBar::getProgressPercentage() {
	return this->progressPercentage;
}

void ProgressBar::setOnProgressFinishCallBack(BCallBack _callBack) {
	this->progressFinishCallBack = _callBack;
}

int ProgressBar::onMouseEventProcess(MouseLRBtnGeoStatus* _mlrbgs) {
	if (_mlrbgs == NULL) {
		AppGlobalLogI("void ProgressBar::onMouseEventProcess(): _mlrbgs is NULL\n");
		return -1;
	}
	/*判断鼠标指针是否在ProgressBar内部,这里暂时只处理矩形的情况*/
	bool b = (_mlrbgs->mpx > this->x && _mlrbgs->mpx < this->x + this->width && _mlrbgs->mpy > this->y && _mlrbgs->mpy < this->y + this->height);
	if (b) {
		if (_mlrbgs->prelbs == _mlrbgs->lbs) {
			if (_mlrbgs->prelbs == 0) {
				/*鼠标处于不按下滑动状态*/
				this->backgroundColor = this->flittedColor;
				this->text.setColor(this->flittedTextColor);
				//this->textColor = ;
				return 1;
			}
			else {
				/*鼠标处于按下滑动状态*/
				this->backgroundColor = this->pressedColor;
				this->text.setColor(this->pressedTextColor);
				return 2;
			}
		}
		if (_mlrbgs->prelbs != _mlrbgs->lbs) {
			if (_mlrbgs->prelbs == 0) {
				/*鼠标由不按下切换为按下状态*/
				this->backgroundColor = this->pressedColor;
				this->text.setColor(this->pressedTextColor);
				_mlrbgs->prelbs = _mlrbgs->lbs;
				return 3;
			}
			else {
				/*鼠标由按下切换为不按下状态*/
				this->backgroundColor = this->flittedColor;
				this->text.setColor(this->flittedTextColor);
				AppGlobalLogI("int ProgressBar::onMouseEventProcess(): clicked! Text: %S", this->text.getTextW().c_str());
				AppGlobalLogI("\n");
				if (this->clickedCallBack != NULL) {
					/*在按钮区域松开左键视为单击此按钮*/
					AppGlobalLogI("int ProgressBar::onMouseEventProcess(): callBack func is called\n");
					this->clickedCallBack();
				}
				_mlrbgs->prelbs = _mlrbgs->lbs;
				return 4;
			}
		}
	}
	else {
		/*鼠标指针不在ProgressBar范围内*/
		this->backgroundColor = this->normalColor;
		this->setTextColor(this->normalTextColor);
		return 0;
	}
}

void ProgressBar::setOnClickCallBack(BCallBack _callBack) {
	this->clickedCallBack = _callBack;
}

/*以上为ProgressBar自身绘制相关*/



/*按钮直接属性*/

void ProgressBar::enableDisplayBackground(bool _b) {
	this->displayBackground = _b;
}

void ProgressBar::setPressedColor(int _A, int _R, int _G, int _B) {
	this->pressedColor = Gdiplus::Color(_A, _R, _G, _B);
}

void ProgressBar::setFlittedColor(int _A, int _R, int _G, int _B) {
	this->flittedColor = Gdiplus::Color(_A, _R, _G, _B);
}

void ProgressBar::setBackgroundColor(int _A, int _R, int _G, int _B) {
	this->normalColor = Gdiplus::Color(_A, _R, _G, _B);
}

void ProgressBar::enableDisplayBorder(bool _b) {
	this->displayBorder = _b;
}

void ProgressBar::setShapeType(BorderType _borderType) {
	this->borderType = _borderType;
	this->border.setType(_borderType);
}

void ProgressBar::setGDIGraphics(Gdiplus::Graphics* _graphics) {
	this->text.setGDIGraphics(_graphics);
	this->border.setGDIGraphics(_graphics);
	this->graphics = _graphics;
}

void ProgressBar::setPosi(int _x, int _y) {
	//this->text.setPosi(_x, _y);
	this->x = _x;
	this->y = _y;
	calcTextPosi();
	calcBorderPosi();
}

void ProgressBar::setSize(int _width, int _height) {
	this->width = _width;
	this->height = _height;
	calcTextPosi();
	calcBorderPosi();
}

int ProgressBar::getPosiX() {
	return this->x;
}

int ProgressBar::getPosiY() {
	return this->y;
}

int ProgressBar::getPosiWidth() {
	return this->width;
}

int ProgressBar::getPosiHeight() {
	return this->height;
}

BorderType ProgressBar::getShapeType() {
	return this->borderType;
}

Gdiplus::Graphics* ProgressBar::getGDIGraphics() {
	return this->graphics;
}

/*以上为按钮直接属性*/


/*以下为按钮中文字部分属性*/
void ProgressBar::setText(char* _srcA) {
	this->text.setText(_srcA);
	calcTextPosi();
}

void ProgressBar::setText(wchar_t* _srcW) {
	this->text.setText(_srcW);
	calcTextPosi();
}

void ProgressBar::setText(std::string& _srcStringA) {
	this->text.setText(_srcStringA);
	calcTextPosi();
}

void ProgressBar::setText(std::wstring& _srcStringW) {
	this->text.setText(_srcStringW);
	calcTextPosi();
}

void ProgressBar::setFontName(char* _fontNameA) {
	this->text.setFontName(_fontNameA);
	calcTextPosi();
}

void ProgressBar::setFontName(wchar_t* _fontNameW) {
	this->text.setFontName(_fontNameW);
	calcTextPosi();
}

void ProgressBar::setFontName(std::string& _fontNameA) {
	this->text.setFontName(_fontNameA);
	calcTextPosi();
}

void ProgressBar::setFontName(std::wstring& _fontNameW) {
	this->text.setFontName(_fontNameW);
	calcTextPosi();
}

void ProgressBar::setFontSize(int _fontSize) {
	this->text.setFontSize(_fontSize);
	calcTextPosi();
}

void ProgressBar::setFontStyle(Gdiplus::FontStyle _fontStyle) {
	this->text.setFontStyle(_fontStyle);
	calcTextPosi();
}

void ProgressBar::setStringAlignment(Gdiplus::StringAlignment _stringAlignment) {
	this->text.setStringAlignment(_stringAlignment);
}

void ProgressBar::setTextColor(Gdiplus::Color& _color) {
	this->text.setColor(_color);
	this->normalTextColor = this->text.getColor();
}

void ProgressBar::setTextColor(int _R, int _G, int _B) {
	this->text.setColor(_R, _G, _B);
	this->normalTextColor = this->text.getColor();
}

void ProgressBar::setTextColor(int _A, int _R, int _G, int _B) {
	this->text.setColor(_A, _R, _G, _B);
	this->normalTextColor = this->text.getColor();
}

void ProgressBar::setPressedTextColor(int _A, int _R, int _G, int _B) {
	this->pressedTextColor = Gdiplus::Color(_A, _R, _G, _B);
}

void ProgressBar::setFlittedTextColor(int _A, int _R, int _G, int _B) {
	this->flittedTextColor = Gdiplus::Color(_A, _R, _G, _B);
}

std::string ProgressBar::getTextA() {
	return this->text.getTextA();
}

std::wstring ProgressBar::getTextW() {
	return this->text.getTextW();
}

int ProgressBar::getTextPosiX() {
	return this->text.getPosiX();
}

int ProgressBar::getTextPosiY() {
	return this->text.getPosiY();
}

int ProgressBar::getFontSize() {
	return this->text.getFontSize();
}

Gdiplus::Color ProgressBar::getTextColor() {
	return this->text.getColor();
}

/*以上为按钮中文字部分属性*/


/*按钮边框部分*/
void ProgressBar::setBorderStroke(float _f) {
	this->border.setStroke(_f);
}

void ProgressBar::setBorderRectRadius(int _rectRadius) {
	this->border.setRectRadius(_rectRadius);
}

void ProgressBar::setBorderColor(Gdiplus::Color& _color) {
	this->border.setColor(_color);
}

void ProgressBar::setBorderColor(int _R, int _G, int _B) {
	this->border.setColor(_R, _G, _B);
}

void ProgressBar::setBorderColor(int _A, int _R, int _G, int _B) {
	this->border.setColor(_A, _R, _G, _B);
}

float ProgressBar::getBorderStroke(float _f) {
	return this->border.getStroke();
}

int ProgressBar::getBorderRectRadius() {
	return this->border.getRectRadius();
}

Gdiplus::Color ProgressBar::getBorderColor(Gdiplus::Color& _color) {
	return this->border.getColor();
}

/*以上为按钮边框部分*/



void ProgressBar::init() {
	borderType = BorderType::BRECT;
	this->displayBackground = false;
	this->displayBorder = true;
	this->clickedCallBack = NULL;
	this->progressFinishCallBack = NULL;
	this->progressPercentage = 0;
}

void ProgressBar::drawProcess() {
	/*绘制顺序应为背景->边框->文字*/
	if (this->displayBackground) {
		Gdiplus::GraphicsPath path;
		this->border.setSize(this->getPosiWidth() * this->progressPercentage / 100, this->getPosiHeight());
		this->border.generateGraphicsPath(path);
		if (this->graphics != NULL) {
			Gdiplus::SolidBrush brush(this->backgroundColor);
			//AppGlobalLogI("void ProgressBar::drawProcess(): displayBackground ON, FillPath\n");
			graphics->FillPath(&brush, &path);
		}
	}
	if (this->displayBorder) {
		this->border.draw();
		
	}
	this->text.draw();

	
}

void ProgressBar::calcTextPosi() {
	this->text.measureTextOutputArea(this->rect);
	//
	switch (this->text.getStringAlignment()) {
	case Gdiplus::StringAlignment::StringAlignmentNear: {
		this->text.setPosi(this->x + this->width / 2 - rect.Width / 2, this->y + this->height / 2 - rect.Height / 2);
		break;
	}
	case Gdiplus::StringAlignment::StringAlignmentCenter: {
		/*正常绘制文字会有一些偏上,这里根据字体大小做一些修正,使其处于正中央*/
		this->text.setPosi(this->x + this->width / 2, this->y + this->height / 2 + this->text.getFontSize() / 20);
		break;
	}
	case Gdiplus::StringAlignment::StringAlignmentFar: {
		break;
	}

	}
}

void ProgressBar::calcBorderPosi() {
	this->border.setPosi(this->x, this->y);
	//AppGlobalLogI("emmm %d %d\n", this->width, this->height);
	//this->border.setPosi(this->x, this->y);
	this->border.setSize(this->width, this->height);
}
