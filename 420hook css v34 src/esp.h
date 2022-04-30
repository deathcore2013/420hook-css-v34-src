#ifndef __ESP_H__
#define __ESP_H__

#include "Include.h"

class Esp
{
public:
	void DrawHealthbar(int Health, Vector vScreen, int x, int y);
	void CrossHair( void );
	void MaterialStuff(void);
	void AsusWalls( void );
	void NameChange( void );
	void DrawHeader(INT x, INT y, INT w, INT r, INT g, INT b, INT HealthBarWidth);
	void DrawEsp( void );

	player_info_t pInfo;
	Vector vScreen;
	Vector vWorldPosition;
	Vector vPlayerHead;
};

typedef struct
{
	int m_iWidth;
	int m_iHeight;
} ScreenSize_t;

extern ScreenSize_t sScreenSize;
extern ScreenSize_t sScreenSize2;

#endif