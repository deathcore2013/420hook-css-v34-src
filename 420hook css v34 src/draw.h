#ifndef __DRAW_H__
#define __DRAW_H__
//____________________________________________________________________________________
#include "Include.h"
//____________________________________________________________________________________
class CDraw
{
public:
	void Border(int x, int y, int w, int h, int line, int r, int g, int b, int a);
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void Rectangle(float x, float y, float w, float h, float px, int r, int g, int b, int a);
	void DrawLine(float x, float y, float x2, float y2, int r, int g, int b, int a);
	void Text(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...);
	void CenterText(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...);
};
//____________________________________________________________________________________ 
#endif