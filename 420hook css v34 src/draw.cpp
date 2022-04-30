#include "draw.h"

//____________________________________________________________________________________
void CDraw::Border(int x, int y, int w, int h, int line, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, line, r, g, b, a);
	FillRGBA(x, y, line, h, r, g, b, a);
	FillRGBA((x + w), y, line, h, r, g, b, a);
	FillRGBA(x, (y + h), (w + line), line, r, g, b, a);
}
//____________________________________________________________________________________
void CDraw::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	Ikaros.m_pMatSurface->DrawSetColor( r, g, b, a );
	Ikaros.m_pMatSurface->DrawFilledRect( x, y, x + w, y + h );
}
//____________________________________________________________________________________
void CDraw::Rectangle(float x, float y, float w, float h, float px, int r, int g, int b, int a)
{
	FillRGBA(x, (y + h - px), w, px, r, g, b, a);
	FillRGBA(x, y, px, h, r, g, b, a);
	FillRGBA(x, y, w, px, r, g, b, a);
	FillRGBA((x + w - px), y, px, h, r, g, b, a);
}
//____________________________________________________________________________________
void CDraw::DrawLine(float x, float y, float x2, float y2, int r, int g, int b, int a)
{
	Ikaros.m_pMatSurface->DrawSetColor( r, g, b, a );
    Ikaros.m_pMatSurface->DrawFilledRect( x, y, x2, y2 );
}  
//____________________________________________________________________________________
void CDraw::Text(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...)
{ 
    va_list va_alist;
	char szBuffer[1024] = { '\0' };

	va_start( va_alist, text );
	vsprintf( szBuffer, text, va_alist );
	va_end( va_alist );

	Ikaros.m_pMatSurface->DrawColoredText( Ikaros.m_pNeeded->m_ESPFont, x, y, Red, Green, Blue, Alpha, "%s", szBuffer );
}
//____________________________________________________________________________________
void CDraw::CenterText(int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...)
{ 
	va_list va_alist;
	char szBuffer[1024] = { '\0' };

	va_start( va_alist, text );
	vsprintf( szBuffer, text, va_alist );
	va_end( va_alist );

	x -= (Ikaros.m_pMatSurface->DrawTextLen( Ikaros.m_pNeeded->m_ESPFont, szBuffer) / 2 );

	Ikaros.m_pMatSurface->DrawColoredText( Ikaros.m_pNeeded->m_ESPFont, x, y, Red, Green, Blue, Alpha, "%s", szBuffer );
}
//____________________________________________________________________________________