#ifndef _LINE_H_
#define _LINE_H_

#include "..\WindowUtil.h"
#include <string>

class Line
{
public:
	Line();
	Line(int _sx, int _sy, int _ex, int _ey);
	virtual ~Line();

	void draw();
	void draw(int _sx, int _sy, int _ex, int _ey);
	void draw(Gdiplus::Graphics* _graphics);
	void draw(Gdiplus::Graphics* _graphics, int _sx, int _sy, int _ex, int _ey);

	void setStartPosi(int _sx, int _sy);
	void setEndPosi(int _ex, int _ey);
	void setGDIGraphics(Gdiplus::Graphics* _graphics);

	
	void setStroke(float _stroke);

	void setColor(Gdiplus::Color& _color);
	void setColor(int _R, int _G, int _B);
	void setColor(int _A, int _R, int _G, int _B);
	void setTransparency(int _A);
	int getStartPosiX();
	int getStartPosiY();
	int getEndPosiX();
	int getEndPosiY();

	Gdiplus::Graphics* getGDIGraphics();

	float getStroke();
	Gdiplus::Color getColor();
private:
	int sx, sy, ex, ey;
	Gdiplus::Pen pen;
	Gdiplus::Graphics* graphics;
	void drawProcess();
	void init();
};



#endif // !_LINE_H_