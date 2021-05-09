#include "Button.h"

Button::Button()
{
	init();
}

Button::~Button()
{

}

Button::Button(Text& _text) {
	init();
	this->text = _text;
	setGDIGraphics(text.getGDIGraphics());
	//setPosi(text.getPosiX(), text.getPosiX());

}

/*Button����������*/

void Button::draw() {
	drawProcess();
}

void Button::draw(int _x, int _y) {
	setPosi(_x, _y);
	drawProcess();
}

void Button::draw(int _x, int _y, int _width, int _height) {
	setPosi(_x, _y);
	setSize(_width, _height);
	drawProcess();
}

void Button::draw(Gdiplus::Graphics* _graphics) {
	setGDIGraphics(_graphics);
	drawProcess();
}

void Button::draw(Gdiplus::Graphics* _graphics, int _x, int _y) {
	setGDIGraphics(_graphics);
	setPosi(_x, _y);
	drawProcess();
}

void Button::draw(Gdiplus::Graphics* _graphics, int _x, int _y, int _width, int _height) {
	setGDIGraphics(_graphics);
	setPosi(_x, _y);
	setSize(_width, _height);
	drawProcess();
}

void Button::trigger() {

}

int Button::onMouseEventProcess(MouseLRBtnGeoStatus* _mlrbgs) {
	if (_mlrbgs == NULL) {
		AppGlobalLogI("void Button::onMouseEventProcess(): _mlrbgs is NULL\n");
		return -1;
	}
	/*�ж����ָ���Ƿ���Button�ڲ�,������ʱֻ������ε����*/
	bool b = (_mlrbgs->mpx > this->x && _mlrbgs->mpx < this->x + this->width && _mlrbgs->mpy > this->y && _mlrbgs->mpy < this->y + this->height);
	if (b) {
		if (_mlrbgs->prelbs == _mlrbgs->lbs) {
			if (_mlrbgs->prelbs == 0) {
				/*��괦�ڲ����»���״̬*/
				this->backgroundColor = this->flittedColor;
				this->text.setColor(this->flittedTextColor);
				//this->textColor = ;
				return 1;
			}
			else {
				/*��괦�ڰ��»���״̬*/
				this->backgroundColor = this->pressedColor;
				this->text.setColor(this->pressedTextColor);
				return 2;
			}
		}
		if (_mlrbgs->prelbs != _mlrbgs->lbs) {
			if (_mlrbgs->prelbs == 0) {
				/*����ɲ������л�Ϊ����״̬*/
				this->backgroundColor = this->pressedColor;
				this->text.setColor(this->pressedTextColor);
				_mlrbgs->prelbs = _mlrbgs->lbs;
				return 3;
			}
			else {
				/*����ɰ����л�Ϊ������״̬*/
				this->backgroundColor = this->flittedColor;
				this->text.setColor(this->flittedTextColor);
				AppGlobalLogI("int Button::onMouseEventProcess(): clicked! Text: %S", this->text.getTextW().c_str());
				AppGlobalLogI("\n");
				if (this->callBack != NULL) {
					/*�ڰ�ť�����ɿ������Ϊ�����˰�ť*/
					AppGlobalLogI("int Button::onMouseEventProcess(): callBack func is called\n");
					this->callBack();
				}
				_mlrbgs->prelbs = _mlrbgs->lbs;
				return 4;
			}
		}
	}
	else {
		/*���ָ�벻��Button��Χ��*/
		this->backgroundColor = this->normalColor;
		this->setTextColor(this->normalTextColor);
		return 0;
	}
}

void Button::setOnClickCallBack(BCallBack _callBack) {
	this->callBack = _callBack;
}

/*����ΪButton����������*/



/*��ťֱ������*/

void Button::enableDisplayBackground(bool _b) {
	this->displayBackground = _b;
}

void Button::setPressedColor(int _A, int _R, int _G, int _B) {
	this->pressedColor = Gdiplus::Color(_A, _R, _G, _B);
}

void Button::setFlittedColor(int _A, int _R, int _G, int _B) {
	this->flittedColor = Gdiplus::Color(_A, _R, _G, _B);
}

void Button::setBackgroundColor(int _A, int _R, int _G, int _B) {
	this->normalColor = Gdiplus::Color(_A, _R, _G, _B);
}

void Button::enableDisplayBorder(bool _b) {
	this->displayBorder = _b;
}

void Button::setShapeType(BorderType _borderType) {
	this->borderType = _borderType;
	this->border.setType(_borderType);
}

void Button::setGDIGraphics(Gdiplus::Graphics* _graphics) {
	this->text.setGDIGraphics(_graphics);
	this->border.setGDIGraphics(_graphics);
	this->graphics = _graphics;
}

void Button::setPosi(int _x, int _y) {
	//this->text.setPosi(_x, _y);
	this->x = _x;
	this->y = _y;
	calcTextPosi();
	calcBorderPosi();
}

void Button::setSize(int _width, int _height) {
	this->width = _width;
	this->height = _height;
	calcTextPosi();
	calcBorderPosi();
}

int Button::getPosiX() {
	return this->x;
}

int Button::getPosiY() {
	return this->y;
}

int Button::getPosiWidth() {
	return this->width;
}

int Button::getPosiHeight() {
	return this->height;
}

BorderType Button::getShapeType() {
	return this->borderType;
}

Gdiplus::Graphics* Button::getGDIGraphics() {
	return this->graphics;
}

/*����Ϊ��ťֱ������*/


/*����Ϊ��ť�����ֲ�������*/
void Button::setText(char* _srcA) {
	this->text.setText(_srcA);
	calcTextPosi();
}

void Button::setText(wchar_t* _srcW) {
	this->text.setText(_srcW);
	calcTextPosi();
}

void Button::setText(std::string& _srcStringA) {
	this->text.setText(_srcStringA);
	calcTextPosi();
}

void Button::setText(std::wstring& _srcStringW) {
	this->text.setText(_srcStringW);
	calcTextPosi();
}

void Button::setFontName(char* _fontNameA) {
	this->text.setFontName(_fontNameA);
	calcTextPosi();
}

void Button::setFontName(wchar_t* _fontNameW) {
	this->text.setFontName(_fontNameW);
	calcTextPosi();
}

void Button::setFontName(std::string& _fontNameA) {
	this->text.setFontName(_fontNameA);
	calcTextPosi();
}

void Button::setFontName(std::wstring& _fontNameW) {
	this->text.setFontName(_fontNameW);
	calcTextPosi();
}

void Button::setFontSize(int _fontSize) {
	this->text.setFontSize(_fontSize);
	calcTextPosi();
}

void Button::setFontStyle(Gdiplus::FontStyle _fontStyle) {
	this->text.setFontStyle(_fontStyle);
	calcTextPosi();
}

void Button::setStringAlignment(Gdiplus::StringAlignment _stringAlignment) {
	this->text.setStringAlignment(_stringAlignment);
}

void Button::setTextColor(Gdiplus::Color& _color) {
	this->text.setColor(_color);
	this->normalTextColor = this->text.getColor();
}

void Button::setTextColor(int _R, int _G, int _B) {
	this->text.setColor(_R, _G, _B);
	this->normalTextColor = this->text.getColor();
}

void Button::setTextColor(int _A, int _R, int _G, int _B) {
	this->text.setColor(_A, _R, _G, _B);
	this->normalTextColor = this->text.getColor();
}

void Button::setPressedTextColor(int _A, int _R, int _G, int _B) {
	this->pressedTextColor = Gdiplus::Color(_A, _R, _G, _B);
}

void Button::setFlittedTextColor(int _A, int _R, int _G, int _B) {
	this->flittedTextColor = Gdiplus::Color(_A, _R, _G, _B);
}

std::string Button::getTextA() {
	return this->text.getTextA();
}

std::wstring Button::getTextW() {
	return this->text.getTextW();
}

int Button::getTextPosiX() {
	return this->text.getPosiX();
}

int Button::getTextPosiY() {
	return this->text.getPosiY();
}

int Button::getFontSize() {
	return this->text.getFontSize();
}

Gdiplus::Color Button::getTextColor() {
	return this->text.getColor();
}

/*����Ϊ��ť�����ֲ�������*/


/*��ť�߿򲿷�*/
void Button::setBorderStroke(float _f) {
	this->border.setStroke(_f);
}

void Button::setBorderRectRadius(int _rectRadius) {
	this->border.setRectRadius(_rectRadius);
}

void Button::setBorderColor(Gdiplus::Color& _color) {
	this->border.setColor(_color);
}

void Button::setBorderColor(int _R, int _G, int _B) {
	this->border.setColor(_R, _G, _B);
}

void Button::setBorderColor(int _A, int _R, int _G, int _B) {
	this->border.setColor(_A, _R, _G, _B);
}

float Button::getBorderStroke(float _f) {
	return this->border.getStroke();
}

int Button::getBorderRectRadius() {
	return this->border.getRectRadius();
}

Gdiplus::Color Button::getBorderColor(Gdiplus::Color& _color) {
	return this->border.getColor();
}

/*����Ϊ��ť�߿򲿷�*/



void Button::init() {
	borderType = BorderType::BRECT;
	this->displayBackground = false;
	this->displayBorder = true;
	this->callBack = NULL;
}

void Button::drawProcess() {
	/*����˳��ӦΪ����->�߿�->����*/
	if (this->displayBackground) {
		Gdiplus::GraphicsPath path;
		this->border.generateGraphicsPath(path);
		if (this->graphics != NULL) {
			Gdiplus::SolidBrush brush(this->backgroundColor);
			//AppGlobalLogI("void Button::drawProcess(): displayBackground ON, FillPath\n");
			graphics->FillPath(&brush, &path);
		}
	}
	if (this->displayBorder) {
		this->border.draw();
		
	}
	this->text.draw();

	
}

void Button::calcTextPosi() {
	this->text.measureTextOutputArea(this->rect);
	//
	switch (this->text.getStringAlignment()) {
	case Gdiplus::StringAlignment::StringAlignmentNear: {
		this->text.setPosi(this->x + this->width / 2 - rect.Width / 2, this->y + this->height / 2 - rect.Height / 2);
		break;
	}
	case Gdiplus::StringAlignment::StringAlignmentCenter: {
		/*�����������ֻ���һЩƫ��,������������С��һЩ����,ʹ�䴦��������*/
		this->text.setPosi(this->x + this->width / 2, this->y + this->height / 2 + this->text.getFontSize() / 20);
		break;
	}
	case Gdiplus::StringAlignment::StringAlignmentFar: {
		break;
	}

	}
}

void Button::calcBorderPosi() {
	this->border.setPosi(this->x, this->y);
	//AppGlobalLogI("emmm %d %d\n", this->width, this->height);
	//this->border.setPosi(this->x, this->y);
	this->border.setSize(this->width, this->height);
}
