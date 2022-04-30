#ifndef __CRADAR_H__
#define __CRADAR_H__

#include "Include.h"

//===================================================================================
class cRadar
{
public:
	void DrawRadar();
	void DrawRadarPoint(Vector vecOriginx, Vector vecOriginy, QAngle vAngle, const char* Name, int a, int r, int g, int b);
};

//===================================================================================
//===================================================================================
#endif