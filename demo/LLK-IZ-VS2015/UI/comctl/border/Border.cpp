#include "Border.h"

Border::Border() :color(255,0,0) {
	init();
}

Border::~Border(){

}

void Border::draw() {
	drawProcess();
}

void Border::draw(int _x, int _y) {
	this->x = _x;
	this->y = _y;
	drawProcess();
}

void Border::draw(int _x, int _y, int _width, int _height) {
	this->x = _x;
	this->y = _y;
	this->width = _width;
	this->height = _height;
	drawProcess();
}

void Border::draw(Gdiplus::Graphics* _graphics) {
	this->graphics = _graphics;
	drawProcess();
}

void Border::draw(Gdiplus::Graphics* _graphics, int _x, int _y) {
	this->graphics = _graphics;
	this->x = _x;
	this->y = _y;
	drawProcess();
}

void Border::draw(Gdiplus::Graphics* _graphics, int _x, int _y, int _width, int _height) {
	this->graphics = _graphics;
	this->x = _x;
	this->y = _y;
	this->width = _width;
	this->height = _height;
	drawProcess();
}

void Border::setGDIGraphics(Gdiplus::Graphics* _graphics) {
	this->graphics = _graphics;
}

void Border::setPosi(int _x, int _y) {
	this->x = _x;
	this->y = _y;
}

void Border::setSize(int _width, int _height) {
	this->width = _width;
	this->height = _height;
}

void Border::setType(BorderType _borderType) {
	this->borderType = _borderType;
}

void Border::setRectRadius(int _rectRadius) {
	this->rectRadius = _rectRadius;
}

void Border::setStroke(float _f) {
	this->stroke = _f;
}

void Border::setColor(Gdiplus::Color& _color) {
	this->color = Gdiplus::Color(_color);
}

void Border::setColor(int _R, int _G, int _B) {
	this->color = Gdiplus::Color(_R, _G, _B);
}

void Border::setColor(int _A, int _R, int _G, int _B) {
	this->color = Gdiplus::Color(_A, _R, _G, _B);
}

int Border::getPosiX() {
	return this->x;
}

int Border::getPosiY() {
	return this->y;
}

int Border::getPosiWidth() {
	return this->width;
}

int Border::getPosiHeight() {
	return this->height;
}

float Border::getStroke() {
	return this->stroke;
}

BorderType Border::getType() {
	return this->borderType;
}

int Border::getRectRadius() {
	return this->rectRadius;
}

Gdiplus::Color Border::getColor() {
	return this->color;
}

Gdiplus::Graphics* Border::getGDIGraphics() {
	return this->graphics;
}

void Border::generateGraphicsPath(Gdiplus::GraphicsPath& graphicsPath) {
	switch (this->borderType)
	{
	case BorderType::BRECT: {
		Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
		//Gdiplus::GraphicsPath graphicsPath;
		graphicsPath.Reset();
		graphicsPath.AddRectangle(rect);
		break;
	}
	case BorderType::BROUNDRECT: {
		graphicsPath.Reset();
		int rr = this->rectRadius;
		int sw = this->width - 2 * rr;
		int sh = this->height - 2 * rr;

		int lt1x = this->x, lt1y = this->y + rr;
		int lt2x = lt1x + rr, lt2y = this->y;
		int rt1x = lt2x + sw, rt1y = lt2y;
		int rt2x = this->x + this->width, rt2y = lt1y;

		int rb1x = rt2x, rb1y = rt2y + sh;
		int rb2x = rt1x, rb2y = this->y + this->height;
		int lb1x = lt2x, lb1y = rb2y;
		int lb2x = lt1x, lb2y = rb1y;

		graphicsPath.AddArc(lt1x, lt2y, rr, rr, 180, 90);
		graphicsPath.AddLine(lt2x, lt2y, rt1x, rt1y);
		graphicsPath.AddArc(rt1x, rt1y, rr, rr, 270, 90);
		graphicsPath.AddLine(rt2x, rt2y, rb1x, rb1y);

		graphicsPath.AddArc(rt1x, lb2y, rr, rr, 0, 90);
		graphicsPath.AddLine(rb2x, rb2y, lb1x, lb1y);
		graphicsPath.AddArc(lb2x, lb2y, rr, rr, 90, 90);
		graphicsPath.AddLine(lb2x, lb2y, lt1x, lt1y-(rr)/2);

		break;
	}
	case BorderType::BROUND: {
		graphicsPath.Reset();
		Gdiplus::Rect rect(this->x, this->y, this->width, this->height);
		graphicsPath.AddEllipse(rect);
		break;
	}
	default: {
		AppGlobalLogE("void Border::generateGraphicsPath(): borderType invaild, %d", this->borderType);
		AppGlobalLogE("\n");
		assert(0);
		break;
	}
	}
}

void Border::init() {
	x = 0;
	y = 0;
	width = 10;
	height = 10;
	stroke = 2.0f;
	rectRadius = 1;
	borderType = BorderType::BRECT;
}

void Border::drawProcess() {
	if (graphics == NULL) {
		assert(0);
		AppGlobalLogE("void Border::drawProcess(): graphics is NULL\n");
		return;
	}
	//AppGlobalLogI("%d %d %d %d\n", x, y, width, height);
	Gdiplus::Pen pen(this->color, this->stroke);
	Gdiplus::GraphicsPath graphicsPath;
	this->generateGraphicsPath(graphicsPath);
	graphics->SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);
	graphics->DrawPath(&pen, &graphicsPath);
	/*这个是用来填充的,这里暂时不用*/
	//graphics->FillPath(&SolidBrush(Color(0, 0, 0)), &graphicsPath);
}