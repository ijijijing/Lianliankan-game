#include "Line.h"

Line::Line() : pen(Gdiplus::Color(225, 0, 0), 2.0f) {
	init();
}

Line::Line(int _sx, int _sy, int _ex, int _ey) : pen(Gdiplus::Color(225, 0, 0), 2.0f) {
	init();
	setStartPosi(_sx, _sy);
	setEndPosi(_ex, _ey);
}

Line::~Line() {
	
}

void Line::draw() {
	drawProcess();
}

void Line::draw(int _sx, int _sy, int _ex, int _ey) {
	setStartPosi(_sx, _sy);
	setEndPosi(_ex, _ey);
	drawProcess();
}

void Line::draw(Gdiplus::Graphics* _graphics) {
	setGDIGraphics(_graphics);
	drawProcess();
}

void Line::draw(Gdiplus::Graphics* _graphics, int _sx, int _sy, int _ex, int _ey) {
	setGDIGraphics(_graphics);
	setStartPosi(_sx, _sy);
	setEndPosi(_ex, _ey);
	drawProcess();
}

void Line::setStartPosi(int _sx, int _sy) {
	this->sx = _sx;
	this->sy = _sy;
}

void Line::setEndPosi(int _ex, int _ey) {
	this->ex = _ex;
	this->ey = _ey;
}

void Line::setGDIGraphics(Gdiplus::Graphics* _graphics) {
	this->graphics = _graphics;
}

void Line::setStroke(float _stroke) {
	this->pen.SetWidth(_stroke);
}

void Line::setColor(Gdiplus::Color& _color) {
	this->pen.SetColor(_color);
}

void Line::setColor(int _R, int _G, int _B) {
	this->pen.SetColor(Gdiplus::Color(_R, _G, _B));
}

void Line::setColor(int _A, int _R, int _G, int _B) {
	this->pen.SetColor(Gdiplus::Color(_A, _R, _G, _B));
}

void Line::setTransparency(int _A) {
	Gdiplus::Color color;
	this->pen.GetColor(&color);
	this->pen.SetColor(Gdiplus::Color(_A, color.GetRed(), color.GetGreen(), color.GetBlue()));
}

int Line::getStartPosiX() {
	return this->sx;
}

int Line::getStartPosiY() {
	return this->sy;
}

int Line::getEndPosiX() {
	return this->ex;
}

int Line::getEndPosiY() {
	return this->ey;
}

Gdiplus::Graphics* Line::getGDIGraphics() {
	return this->graphics;
}

float Line::getStroke() {
	return this->pen.GetWidth();
}

Gdiplus::Color Line::getColor() {
	Gdiplus::Color color;
	this->pen.GetColor(&color);
	return color;
}

void Line::drawProcess() {
	if (graphics == NULL) {
		return;
	}
	graphics->DrawLine(&pen, sx, sy, ex, ey);
	
}

void Line::init() {
	sx = 10;
	sy = 10;
	ex = 20;
	ey = 20;
	graphics = NULL;
}