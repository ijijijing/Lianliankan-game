#ifndef _BORDER_H_
#define _BORDER_H_

#include "..\..\WindowUtil.h"

enum BorderType
{
	BRECT,
	BROUNDRECT,
	BROUND,
};

class Border
{
public:
	Border();
	virtual ~Border();

	void draw();
	void draw(int _x, int _y);
	void draw(int _x, int _y, int _width, int _height);
	void draw(Gdiplus::Graphics* _graphics);
	void draw(Gdiplus::Graphics* _graphics, int _x, int _y);
	void draw(Gdiplus::Graphics* _graphics, int _x, int _y, int _width, int _height);

	void setGDIGraphics(Gdiplus::Graphics* _graphics);
	void setPosi(int _x, int _y);
	void setSize(int _width, int _height);
	void setType(BorderType _borderType);
	//void setRadius(int _radius);
	void setRectRadius(int _rectRadius);

	void setStroke(float _f);
	void setColor(Gdiplus::Color& _color);
	void setColor(int _R, int _G, int _B);
	void setColor(int _A, int _R, int _G, int _B);

	int getPosiX();
	int getPosiY();
	int getPosiWidth();
	int getPosiHeight();
	BorderType getType();

	int getRectRadius();
	float getStroke();
	Gdiplus::Color getColor();
	Gdiplus::Graphics* getGDIGraphics();
	void generateGraphicsPath(Gdiplus::GraphicsPath& graphicsPath);
private:
	Gdiplus::Graphics* graphics;
	int x, y;
	
	int width, height;
	float stroke;
	Gdiplus::Color color;
	void init();
	void drawProcess();
	

	BorderType borderType;
	int rectRadius;

};

#endif