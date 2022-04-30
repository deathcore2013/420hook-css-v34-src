#include "cRadar.h"
#include "SDK.h"

bool bSave_Radar_Pos = true;
int size = 50;

void cRadar::DrawRadar()
{
	if(!Ikaros.m_pCvars->bRadar2D)
		return;

	//if (Ikaros.m_pMouse->IsInBox(Ikaros.m_pCvars->flRadarPos_x - (size = Ikaros.m_pCvars->bRadarSize) - 6, Ikaros.m_pCvars->flRadarPos_y - (size = Ikaros.m_pCvars->bRadarSize) - 6, size * 2 + 12, size * 2 + 12) && Ikaros.m_pMouse->bMouse1pressed)
	{
		if (!bSave_Radar_Pos)
		{
			Ikaros.m_pCvars->iSavedRadarX = Ikaros.m_pCvars->iMouseX - Ikaros.m_pCvars->flRadarPos_x;
			Ikaros.m_pCvars->iSavedRadarY = Ikaros.m_pCvars->iMouseY - Ikaros.m_pCvars->flRadarPos_y;
			bSave_Radar_Pos = true;
		}
		Ikaros.m_pCvars->flRadarPos_x = Ikaros.m_pCvars->iMouseX;
		Ikaros.m_pCvars->flRadarPos_y = Ikaros.m_pCvars->iMouseY;
		Ikaros.m_pCvars->flRadarPos_x = Ikaros.m_pCvars->flRadarPos_x - Ikaros.m_pCvars->iSavedRadarX;
		Ikaros.m_pCvars->flRadarPos_y = Ikaros.m_pCvars->flRadarPos_y - Ikaros.m_pCvars->iSavedRadarY;
	}
	//else
	{
		bSave_Radar_Pos = false;
	}

	//int centerx = Ikaros.m_pCvars->flRadarPos_x+(size = Ikaros.m_pCvars->bRadarSize);
	//int centery = Ikaros.m_pCvars->flRadarPos_y+(size = Ikaros.m_pCvars->bRadarSize); 
	if(Ikaros.m_pCvars->bRadarBG)
	{
		//Ikaros.m_pDraw->FillRGBA(centerx, centery - size, 1, 2 * size, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
		//Ikaros.m_pDraw->FillRGBA(centerx - size, centery, 2 * size, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
	}
	else
	{
		//Ikaros.m_pDraw->Border(centerx - size - 6, centery - size - 6, 2 * size + 6, 2 * size + 6, 6, Ikaros.m_pCvars->DrawColor-RED(CHEAT_RED), Ikaros.m_pCvars->DrawColor2-GREEN(CHEAT_GREEN), Ikaros.m_pCvars->DrawColor3-BLUE(CHEAT_BLUE), Ikaros.m_pCvars->DrawColor4-ALPHA(MenuColor));
		//Ikaros.m_pDraw->FillRGBA(centerx - size, centery - size, 2 * size, 2 * size, Ikaros.m_pCvars->DrawColor5-RED(CHEAT_RED), Ikaros.m_pCvars->DrawColor6-GREEN(CHEAT_GREEN), Ikaros.m_pCvars->DrawColor7-BLUE(CHEAT_BLUE), Ikaros.m_pCvars->DrawColor8-ALPHA(MenuColor));
		//Ikaros.m_pDraw->FillRGBA(centerx, centery - size, 1, 2 * size, Ikaros.m_pCvars->DrawColor-RED(CHEAT_RED), Ikaros.m_pCvars->DrawColor2-GREEN(CHEAT_GREEN), Ikaros.m_pCvars->DrawColor3-BLUE(CHEAT_BLUE), Ikaros.m_pCvars->DrawColor4-ALPHA(MenuColor));
		//Ikaros.m_pDraw->FillRGBA(centerx - size, centery, 2 * size, 1, Ikaros.m_pCvars->DrawColor-RED(CHEAT_RED), Ikaros.m_pCvars->DrawColor2-GREEN(CHEAT_GREEN), Ikaros.m_pCvars->DrawColor3-BLUE(CHEAT_BLUE), Ikaros.m_pCvars->DrawColor4-ALPHA(MenuColor));
		//Ikaros.m_pDraw->FillRGBA(centerx - size - 6, centery - size - Ikaros.m_pCvars->bRadarSize - 0, Ikaros.m_pCvars->bRadarSize - 0, Ikaros.m_pCvars->bRadarSize - 0, RED(MenuColor), GREEN(MenuColor), BLUE(MenuColor), ALPHA(MenuColor));
		//Ikaros.m_pDraw->Text(centerx - size, centery - size - 15, RED(FontColor), GREEN(FontColor), BLUE(FontColor), ALPHA(FontColor), "Radar");
		//Ikaros.m_pDraw->Border(centerx - size - 6, centery - size - 22, 2 * size + 11, 2 * size + 28, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
	}
	//Ikaros.m_pDraw->Rectangle(Ikaros.m_pCvars->flRadarPos_x + (size = Ikaros.m_pCvars->bRadarSize) - 2, Ikaros.m_pCvars->flRadarPos_y + (size = Ikaros.m_pCvars->bRadarSize) - 2, 5, 5, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
	//Ikaros.m_pDraw->FillRGBA(Ikaros.m_pCvars->flRadarPos_x + (size = Ikaros.m_pCvars->bRadarSize) - 1, Ikaros.m_pCvars->flRadarPos_y + (size = Ikaros.m_pCvars->bRadarSize) - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
}

void cRadar::DrawRadarPoint(Vector vecOriginx, Vector vecOriginy, QAngle vAngle, const char* Name, int a, int r, int g, int b)
{
	float dx  = vecOriginx.x - vecOriginy.x;
	float dy  = vecOriginx.y - vecOriginy.y;
	float flAngle = vAngle.y;
	float yaw = (flAngle) * (M_PI/180.0);
	float mainViewAngles_CosYaw = cos(yaw);
	float mainViewAngles_SinYaw = sin(yaw);
	float x =  dy*(-mainViewAngles_CosYaw)  + dx*mainViewAngles_SinYaw;
	float y =  dx*(-mainViewAngles_CosYaw)  - dy*mainViewAngles_SinYaw;
	//float range = size * 25.0f;
	//if(fabs(x)>range || fabs(y)>range)
	{ 
		if(y>x)
		{
			if(y>-x) {
				//x = range*x/y;
				//y = range;
			}  else  {
				//y = -range*y/x; 
				//x = -range; 
			}
		} else {
			if(y>-x) {
				//y = range*y/x; 
				//x = range; 
			}  else  {
				//x = -range*x/y;
				//y = -range;
			}
		}
	}
	//}
	//int ScreenX = Ikaros.m_pCvars->flRadarPos_x+(size = Ikaros.m_pCvars->bRadarSize) + int( x/range*float(Ikaros.m_pCvars->bRadarRange));
	//int ScreenY = Ikaros.m_pCvars->flRadarPos_y+(size = Ikaros.m_pCvars->bRadarSize) + int( y/range*float(Ikaros.m_pCvars->bRadarRange));
	//Ikaros.m_pDraw->FillRGBA(ScreenX - 2, ScreenY - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
	//Ikaros.m_pDraw->FillRGBA(ScreenX - 1, ScreenY - 1, 3, 3, r, g, b, a);
	//if(Ikaros.m_pCvars->bRadarName)
	//	Ikaros.m_pDraw->CenterText(ScreenX + 2, ScreenY + 4, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE), Name);
}