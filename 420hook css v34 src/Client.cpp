#include "Include.h"
#include "SpoofedConvar.h"
#include "XorStr.h"

bool bSendPacket = true;
void* baseptr = NULL;
bool bNoRecoil = true;
bool bRCS = true;
bool bChams = true;
bool bXQZ = true;
bool bShake = true;
bool bAntiAim = true;
bool bSilent = true; 
bool bResChange = false;
int iChokedPackets = 0;
BYTE patch[2];
QAngle qRealView;
QAngle SavedAngles;
ShutdownFn oShutdown;
bool breaking_lag_compensation;
static Vector last_sent_origin;

void ConsoleColorPrint(Color color, const char* fmt, ...)
{
    char m_szBuf[1024] = { 0 };
    va_list va_alist;
 
    va_start(va_alist, fmt);
    vsprintf(m_szBuf, fmt, va_alist);
    va_end(va_alist);
 
    strcat(m_szBuf, "\n");
 
    Ikaros.m_pGameConsole->ColorPrintf(color, m_szBuf);
}
 
void RandomColorPrint(const char*fmt, ...)
{
    char m_szBuf[1024] = { 0 };
    va_list va_alist;
 
    va_start(va_alist, fmt);
    vsprintf(m_szBuf, fmt, va_alist);
    va_end(va_alist);
 
    strcat(m_szBuf, "\n");
 
    Color clr(rand() % 255, rand() % 255, rand() % 255, 255);
    Ikaros.m_pGameConsole->ColorPrintf(clr, m_szBuf);
}

void AntiAntiAimYProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flYaw = pData->m_Value.m_Float;

	if (flYaw > 180.0f)
		flYaw -= 360.0f;
	if (flYaw < -180.0f)
		flYaw += 360.0f;

	static bool JitterFix[64];
	bool jitTemporary = JitterFix[((C_BaseEntity*)(pStruct))->index];

	JitterFix[((C_BaseEntity*)(pStruct))->index] = (flYaw >= 180 && flYaw <= 360);

	if (jitTemporary && (flYaw >= 0 && flYaw <= 181))
	{
		flYaw += 359;
	}

	flYaw = flYaw - (int)(flYaw / 360) * 360;

	if (flYaw < -181)
	{
		flYaw = flYaw + 360;
	}
	else if (flYaw > 181)
	{
		flYaw = flYaw - 360;
	}

	if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].bAdvancedYaw)
	{
		flYaw -= Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iAdvancedYaw;
	}
	if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].bJitterYaw)
	{
		static float s = -180.0f;
		s = s + Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iAddJitterYaw;
		if (s > 180.f)
			s = -180.f;

		flYaw = s;
	}
	if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].bStaticYaw)
	{
		flYaw -= 180.f;
	}
	else
	{
		if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 1)
		{
			flYaw -= 180.0f;
		}
		else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 2)
		{
			flYaw += 90.0f;
		}
		else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 3)
		{
			flYaw -= 90.0f;
		}
		else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 4)
		{
			static float s = -180.0f;
			s = s + 35.f;//Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iAddYaw;
			if (s > 180.f)
				s = -180.f;

			flYaw = s;
		}
		else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 5)
		{
			flYaw -= 290.0f;
		}
		else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iYaw == 6)
		{
			static float s = -360.0f;
			s = s + 135.f;
			if (s > 360.0f)
			s = -360.0f;

			flYaw = s;
		}
	}

	*(float*)pOut = flYaw;
}

void AntiAntiAimXProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flPitch = pData->m_Value.m_Float;

	if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iPitch == 1)
	{
		if (flPitch > 181 && flPitch < 271)
		{
			flPitch = 269.0;
		}

		if (flPitch > 89 && flPitch < 181)
		{
			flPitch = 89.0;
		}
	}
	else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iPitch == 2)
	{
		flPitch = -89.0f;
	}
	else if (Ikaros.m_pPlayerlist[((C_BaseEntity*)(pStruct))->index].iPitch == 3)
	{
		flPitch = 89.0f;
	}

	*(float*)pOut = flPitch;
}

void ForceCommands()
{
	ConVar* skybox = Ikaros.m_pCvar->FindVar("r_skybox");
	ConVar* clearsky = Ikaros.m_pCvar->FindVar("gl_clear");
	ConVar* bright = Ikaros.m_pCvar->FindVar("mat_fullbright");
	ConVar* cheats = Ikaros.m_pCvar->FindVar("sv_cheats");
	ConVar* handsposition = Ikaros.m_pCvar->FindVar("cl_righthand");
	ConVar* net_fakelag = Ikaros.m_pCvar->FindVar("net_fakelag");

	if (Ikaros.m_pCvars->iAntiAimTypeY == 3)
		Ikaros.m_pCvars->bAimAtTargets = false;

	if ((Ikaros.m_pCvars->iNoRecoil > 0) && bNoRecoil)
	{
		Ikaros.m_pCvars->iRCS = 0;
		bRCS = true;
		bNoRecoil = false;
	}

	if ((Ikaros.m_pCvars->iRCS > 0) && bRCS)
	{
		Ikaros.m_pCvars->iNoRecoil = 0;
		bRCS = false;
		bNoRecoil = true;
	}

	if (Ikaros.m_pCvars->bChams && bChams)
	{
		Ikaros.m_pCvars->bXQZWalls = false;
		bChams = false;
		bXQZ = true;
	}

	if (Ikaros.m_pCvars->bXQZWalls && bXQZ)
	{
		Ikaros.m_pCvars->bChams = false;
		bChams = true;
		bXQZ = false;
	}

	if (!Ikaros.m_pCvars->bSilent)
		Ikaros.m_pCvars->bPSilent = false;

	if (Ikaros.m_pCvars->bShowShake && bShake)
	{
		if (Ikaros.m_pCvars->iAntiAimTypeX == 3)
			Ikaros.m_pCvars->bAntiAim = false;

		Ikaros.m_pCvars->bSilent = false;
		bShake = false;
		bAntiAim = true;
		bSilent = true;
	}

	if (Ikaros.m_pCvars->bSilent && bSilent)
	{
		Ikaros.m_pCvars->bShowShake = false;
		bSilent = false;
		bAntiAim = true;
		bShake = true;
	}

	if (Ikaros.m_pCvars->bAntiAim && bAntiAim)
	{
		if (Ikaros.m_pCvars->iAntiAimTypeX == 3)
			Ikaros.m_pCvars->bShowShake = false;

		bAntiAim = false;
		bSilent = true;
		bShake = true;
	}

	if (Ikaros.m_pCvars->bNoSky)
	{
		skybox->SetValue(0);
		clearsky->SetValue(1);
	}
	else
	{
		skybox->SetValue(1);
		clearsky->SetValue(0);
	}

	if (Ikaros.m_pCvars->bFullbright)
	{
		bright->SetValue(1);
	}
	else
	{
		bright->SetValue(0);
	}

	if (Ikaros.m_pCvars->Cheats)
	{
		cheats->SetValue(1);
	}
	else
	{
		cheats->SetValue(0);
	}

	if (Ikaros.m_pCvars->bNetFakelag)
	{
		net_fakelag->SetValue(Ikaros.m_pCvars->iNetFakelag);
	}
	else
	{
		net_fakelag->SetValue(0);
	}

	if (Ikaros.m_pCvars->iHandsPosition == 0)
	{
		handsposition->SetValue(0);
	}
	else if (Ikaros.m_pCvars->iHandsPosition == 1)
	{
		handsposition->SetValue(1);
	}

	if (Ikaros.m_pCvars->bSigFail)
	{
		Ikaros.m_pCvars->bPSilent = false;
		Ikaros.m_pCvars->bSpeed = false;
		if (Ikaros.m_pCvars->iAntiAimTypeX > 3)
			Ikaros.m_pCvars->iAntiAimTypeX = 3;
		if (Ikaros.m_pCvars->iAntiAimTypeY > 3)
			Ikaros.m_pCvars->iAntiAimTypeY = 3;
	}
}

CUserCmd* __stdcall new_GetUserCmd(int sequence_number)
{
	static CUserCmd* pCommands = *(CUserCmd**)((DWORD)Ikaros.m_pInput + 0xC4);
	return &pCommands[sequence_number % MULTIPLAYER_BACKUP];
}

void __stdcall new_HudUpdate(bool bActive)
{
	ClientClass *pClass = Ikaros.m_pClient->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->GetName();

		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = pClass->m_pRecvTable->GetProp(i);

				if (!strcmp(pProp->GetName(), "m_angEyeAngles[0]"))
					pProp->SetProxyFn(AntiAntiAimXProxy);
				if (!strcmp(pProp->GetName(), "m_angEyeAngles[1]"))
					pProp->SetProxyFn(AntiAntiAimYProxy);
			}
		}
		pClass = pClass->m_pNext;
	}

	pClientHook->Function<HudUpdate_t>(8)(bActive);
}

float __stdcall hkdGetViewModelFOV()
{
	return Ikaros.m_pCvars->ViewmodelFOV;
}

bool __stdcall hkdDispatchUserMesssage(int msgtype, const void* msgdata)
{
	//speak("msgtype -> %i", msgtype);
	return pClientHook->Function<DispatchUserMessageFn>(33)(msgtype, msgdata);
}

void __stdcall hkdShutdown(const char* reason)
{
	if (Ikaros.m_pCvars->iDisconnectReason == 0)
	{
		return oShutdown(g_pNetChannel, "Disconnect by user");
	}
	else if (Ikaros.m_pCvars->iDisconnectReason == 1)
	{
		return oShutdown(g_pNetChannel, "VAC BAN!!!");
	}
	else if (Ikaros.m_pCvars->iDisconnectReason == 2)
	{
		return oShutdown(g_pNetChannel, "420hook");
	}
	else if (Ikaros.m_pCvars->iDisconnectReason == 3)
	{
		return oShutdown(g_pNetChannel, "Gay Shit");
	}
}

void __stdcall new_FrameStageNotify(ClientFrameStage_t curStage)
{
	if (Ikaros.m_pEngine->IsInGame())
	{
		if (curStage == FRAME_UNDEFINED)
			return;
	}

	QAngle *PunchAngle, OldPunchAngle;
	PunchAngle = (QAngle*)((DWORD)Ikaros.m_pMyPlayer->Ent() + Ikaros.m_pOffsets->PunchAngle);

	if (curStage == FRAME_RENDER_START)
	{
		if (Ikaros.m_pMyPlayer->Ent() && Ikaros.m_pMyPlayer->BaseEnt())
		{
			if (g_pInput->CAM_IsThirdPerson())
				*(QAngle*)((DWORD)Ikaros.m_pMyPlayer->BaseEnt() + Ikaros.m_pOffsets->DeadFlag + 4) = SavedAngles;

			if (Ikaros.m_pCvars->bNoVisRecoil)
			{
				if (PunchAngle->x != 0 || PunchAngle->y != 0 || PunchAngle->z != 0)
				{
					OldPunchAngle = *PunchAngle;
					*PunchAngle = QAngle(0, 0, 0);
				}
			}
		}
	}

	pClientHook->Function<FrameStageNotify_t>(32)(curStage);

	if (Ikaros.m_pCvars->bNoVisRecoil)
	{
		if (curStage == FRAME_RENDER_START)
		{
			if (Ikaros.m_pEngine->IsInGame() && Ikaros.m_pMyPlayer->Ent())
			{
				if (PunchAngle->x == 0 || PunchAngle->y == 0 || PunchAngle->z == 0)
				{
					*PunchAngle = OldPunchAngle;
				}
			}
		}
	}
}

void __stdcall new_PaintTraverse(vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	pPanelHook->Function<PaintTraverse_t>(40)(vguiPanel, forceRepaint, allowForce);

	auto* netchannel = (INetChannel*)g_pEngine->GetNetChannelInfo();
	DWORD netchannelhook;
	if (netchannel && g_pNetChannel != netchannel)
	{
		// new netchannel pointer
		g_pNetChannel = netchannel;
		netchannelhook = (DWORD)netchannel;

		pNetChannelHook = new C_VMT(&netchannelhook);
		pNetChannelHook->Hook((DWORD)hkdShutdown, 32);

		oShutdown = (ShutdownFn)pNetChannelHook->OldPointer(32);
	}

	const char* pszPanelName = Ikaros.m_pPanel->GetName(vguiPanel);
	bool isValidPanel = false;

	if (pszPanelName && pszPanelName[0] == 'M' && pszPanelName[3] == 'S' &&
		pszPanelName[9] == 'T' && pszPanelName[12] == 'P')
		isValidPanel = true;

	if (isValidPanel)
	{
		DO_ONCE(
			Ikaros.m_pNeeded->FontInit();
		Ikaros.m_pEngine->GetScreenSize(sScreenSize.m_iWidth, sScreenSize.m_iHeight);
		Ikaros.m_pEvents->RegisterSelf();
		);

		Ikaros.m_pEngine->GetScreenSize(sScreenSize2.m_iWidth, sScreenSize2.m_iHeight);

		if (sScreenSize.m_iWidth != sScreenSize2.m_iWidth || sScreenSize.m_iHeight != sScreenSize2.m_iHeight)
		{
			sScreenSize.m_iWidth = sScreenSize2.m_iWidth;
			sScreenSize.m_iHeight = sScreenSize2.m_iHeight;
			bResChange = true;
		}

		if (bResChange)
		{
			Ikaros.m_pNeeded->FontInit();
			bResChange = false;
		}

		if (Ikaros.m_pCvars->bRecoilCrosshair)
		{
			static IClientEntity *local = nullptr;
			C_BaseEntity* pBaseEntity = Ikaros.m_pMyPlayer->BaseEnt();
			local = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer());
			if (Ikaros.m_pEngine->IsInGame() && local)
			{
				QAngle Angles;
				QAngle qPunchAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
				Ikaros.m_pEngine->GetViewAngles(Angles);
				Angles += (qPunchAngle * 2.f);

				Vector fowardVec;
				AngleVectors(Angles + QAngle(0, 0, 0), &fowardVec);
				fowardVec *= 10000;

				Vector start = Ikaros.m_pEntity->GetEyePos(pBaseEntity);
				Vector end = start + fowardVec, endScreen;

				if (Ikaros.m_pNeeded->WorldToScreen(end, endScreen) && Ikaros.m_pEngine->IsConnected() && Ikaros.m_pEngine->IsInGame() && Ikaros.m_pMyPlayer->BaseEnt() && !Ikaros.m_pEngine->IsTakingScreenshot() && !Ikaros.m_pEngine->IsLevelMainMenuBackground())
				{
					if (Ikaros.m_pCvars->iRecoilCrosshair == 1)
					{
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 2, (endScreen.y - 1) - 2, 5, 5, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 1, 3, 3, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_BLACK));
					}
					else if (Ikaros.m_pCvars->iRecoilCrosshair == 2)
					{
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 11, 3, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 3, 11, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_WHITE), GREEN(CHEAT_WHITE), BLUE(CHEAT_WHITE), ALPHA(CHEAT_WHITE));
					}
					else if (Ikaros.m_pCvars->iRecoilCrosshair == 3)
					{
						static float x = 0, y = 0;
						if (y >= 0.0f && y < 2.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_ORANGE), GREEN(CHEAT_ORANGE), BLUE(CHEAT_ORANGE), ALPHA(CHEAT_ORANGE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_ORANGE), GREEN(CHEAT_ORANGE), BLUE(CHEAT_ORANGE), ALPHA(CHEAT_ORANGE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_ORANGE), GREEN(CHEAT_ORANGE), BLUE(CHEAT_ORANGE), ALPHA(CHEAT_ORANGE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_ORANGE), GREEN(CHEAT_ORANGE), BLUE(CHEAT_ORANGE), ALPHA(CHEAT_ORANGE));
						}
						else if (y >= 2.0f && y < 4.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						}
						else if (y >= 4.0f && y < 6.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
						}
						else if (y >= 6.0f && y < 8.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_GREEN), GREEN(CHEAT_GREEN), BLUE(CHEAT_GREEN), ALPHA(CHEAT_GREEN));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_GREEN), GREEN(CHEAT_GREEN), BLUE(CHEAT_GREEN), ALPHA(CHEAT_GREEN));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_GREEN), GREEN(CHEAT_GREEN), BLUE(CHEAT_GREEN), ALPHA(CHEAT_GREEN));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_GREEN), GREEN(CHEAT_GREEN), BLUE(CHEAT_GREEN), ALPHA(CHEAT_GREEN));
						}
						else if (y >= 8.0f && y < 10.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_BLUE), GREEN(CHEAT_BLUE), BLUE(CHEAT_BLUE), ALPHA(CHEAT_BLUE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_BLUE), GREEN(CHEAT_BLUE), BLUE(CHEAT_BLUE), ALPHA(CHEAT_BLUE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_BLUE), GREEN(CHEAT_BLUE), BLUE(CHEAT_BLUE), ALPHA(CHEAT_BLUE));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_BLUE), GREEN(CHEAT_BLUE), BLUE(CHEAT_BLUE), ALPHA(CHEAT_BLUE));
						}
						else if (y >= 10.0f && y < 12.0f) {
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1) - 1, 10, 2, RED(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_YELLOW), ALPHA(CHEAT_YELLOW));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 5, 2, 10, RED(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_YELLOW), ALPHA(CHEAT_YELLOW));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 4, (endScreen.y - 1), 9, 1, RED(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_YELLOW), ALPHA(CHEAT_YELLOW));
							Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 4, 1, 9, RED(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_YELLOW), ALPHA(CHEAT_YELLOW));
						}

						x += 0.25f; //increase this value to switch colors faster

						if (x >= 2.0f)
							x = 0.0f;
						y += 0.25f; //increase this value to switch colors faster
						if (y > 12.0f)
							y = 0.0f;
					}
					else if (Ikaros.m_pCvars->iRecoilCrosshair == 4)
					{
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1), 20, 1, RED(CHEAT_WIREBLUE), GREEN(CHEAT_WIREBLUE), BLUE(CHEAT_WIREBLUE), ALPHA(CHEAT_WIREBLUE));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 10, 1, 20, RED(CHEAT_WIREBLUE), GREEN(CHEAT_WIREBLUE), BLUE(CHEAT_WIREBLUE), ALPHA(CHEAT_WIREBLUE));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 5, (endScreen.y - 1), 11, 1, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2), (endScreen.y - 1) - 5, 1, 11, RED(CHEAT_RED), GREEN(CHEAT_RED), BLUE(CHEAT_RED), ALPHA(CHEAT_RED));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) - 10, 20, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) + 10, 21, 1, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) - 10, 1, 20, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) + 10, (endScreen.y - 1) - 10, 1, 20, RED(CHEAT_BLACK), GREEN(CHEAT_BLACK), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					}
					else if (Ikaros.m_pCvars->iRecoilCrosshair == 5)
					{
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) - 10, 20, 1, BLUE(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) + 10, 21, 1, BLUE(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 10, (endScreen.y - 1) - 10, 1, 20, BLUE(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) + 10, (endScreen.y - 1) - 10, 1, 20, BLUE(CHEAT_YELLOW), GREEN(CHEAT_YELLOW), BLUE(CHEAT_BLACK), ALPHA(CHEAT_BLACK));
					}
					else if (Ikaros.m_pCvars->iRecoilCrosshair == 6)
					{
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 11, (endScreen.y - 1) - 1, 23, 3, 255, 255, 255, 255);
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 11, 3, 23, 255, 255, 255, 255);
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 1, (endScreen.y - 1) - 11, 13, 3, 255, 255, 255, 255); // top -> left
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 11, (endScreen.y - 1) - 11, 3, 11, 255, 255, 255, 255); // top -> bottom
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) + 9, (endScreen.y - 1), 3, 12, 255, 255, 255, 255); // right -> bottom
						Ikaros.m_pDraw->FillRGBA((endScreen.x - 2) - 11, (endScreen.y - 1) + 9, 11, 3, 255, 255, 255, 255); // bottom -> left
					}
				}
			}
		}

		ForceCommands();
		Ikaros.m_pMenu->DrawMenu();
	}
}

int __stdcall new_DrawModelEx(ModelRenderInfo_t &pInfo)
{
	pModelHook->Unhook(19);
	studiohdr_t* hdr = Ikaros.m_pModelinfo->GetStudiomodel(pInfo.pModel);
	if (pInfo.pModel && hdr)
	{
		static float fRed, fGreen, fBlue, fRed2, fGreen2, fBlue2, fRed3, fGreen3, fBlue3, fAlpha;
		const char* pszModelName = Ikaros.m_pModelinfo->GetModelName(pInfo.pModel);

		if (Ikaros.m_pCvars->bGlassChams)
		{
			fAlpha = Ikaros.m_pCvars->GlassChamsColor;
		}
		else
		{
			fAlpha = 1.0f;
		}

		if ((strstr(pszModelName, "models/player")))
		{
			if (Ikaros.m_pCvars->bChams && !Ikaros.m_pCvars->bPanicKey)
			{
				C_BaseEntity* pModelEntity = (C_BaseEntity*)Ikaros.m_pEntList->GetClientEntity(pInfo.entity_index);

				if (pModelEntity && Ikaros.m_pMyPlayer->BaseEnt())
				{
					player_info_t pinfo;

					if (Ikaros.m_pEntity->GetLifeState(pModelEntity)
						&& Ikaros.m_pEngine->GetPlayerInfo(pInfo.entity_index, &pinfo))
					{
						int iTeamIndex = Ikaros.m_pEntity->GetTeamID(pModelEntity);

						if (iTeamIndex == 2)
						{
							fRed = Ikaros.m_pCvars->DrawColor25;
							fGreen = Ikaros.m_pCvars->DrawColor26;
							fBlue = Ikaros.m_pCvars->DrawColor27;

							fRed3 = Ikaros.m_pCvars->DrawColor25;
							fGreen3 = Ikaros.m_pCvars->DrawColor26;
							fBlue3 = Ikaros.m_pCvars->DrawColor27;
						}
						else if (iTeamIndex == 3)
						{
							fRed = Ikaros.m_pCvars->DrawColor28;
							fGreen = Ikaros.m_pCvars->DrawColor29;
							fBlue = Ikaros.m_pCvars->DrawColor30;

							fRed3 = Ikaros.m_pCvars->DrawColor28;
							fGreen3 = Ikaros.m_pCvars->DrawColor29;
							fBlue3 = Ikaros.m_pCvars->DrawColor30;
						}
					}
					else
					{
						fRed = 1.0f;
						fGreen = 1.0f;
						fBlue = 1.0f;

						fRed3 = 1.0f;
						fGreen3 = 1.0f;
						fBlue3 = 1.0f;
					}
				}
				else
				{
					if (strstr(pszModelName, "models/player/t_"))
					{
						fRed = 1.0f;
						fGreen = 0.2f;
						fBlue = 0.2f;

						fRed3 = 1.0f;
						fGreen3 = 0.2f;
						fBlue3 = 0.2f;
					}
					if (strstr(pszModelName, "models/player/ct_"))
					{
						fRed = 0.2f;
						fGreen = 0.2f;
						fBlue = 1.0f;

						fRed3 = 0.2f;
						fGreen3 = 0.2f;
						fBlue3 = 1.0f;
					}
				}
				//for (int t = 0; t < 12; t++) 

		//Ikaros.m_pModelRender->DrawModelExecute(state, pInfo, headPositions[pInfo.entity_index][t].matrix);
				Ikaros.m_pNeeded->ForceMaterial(fRed3, fGreen3, fBlue3, fAlpha, mat3); 
				Ikaros.m_pModelRender->DrawModelEx(pInfo);
				if (Ikaros.m_pCvars->iPlayerChamsMaterial == 1)
				{
				mat3->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
				else
				{
				mat3->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (Ikaros.m_pCvars->bChamsThroughWalls)
				{
					mat3->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				}
				else
				{
					mat3->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				}
				if (Ikaros.m_pCvars->iPlayerChamsMaterial == 0)
				{
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				Ikaros.m_pNeeded->ForceMaterial(fRed, fGreen, fBlue, fAlpha, mat);
				Ikaros.m_pModelRender->DrawModelEx(pInfo);
				}
			}
		}
		else if ((strstr(pszModelName, "models/weapons/w_")))
		{
			if (Ikaros.m_pCvars->bWeaponChams && !Ikaros.m_pCvars->bPanicKey)
			{
				if (strstr(pszModelName, "models/weapons/w_"))
				{
					if (Ikaros.m_pCvars->iWeaponChamsMaterial == 0)
					{
						fRed2 = 1.0f;
						fGreen2 = 1.0f;
						fBlue2 = 1.0f;
					}
					else if (Ikaros.m_pCvars->iWeaponChamsMaterial == 1)
					{
						mat2->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
					}
					if (Ikaros.m_pCvars->iWeaponChamsMaterial == 0)
					{
						mat2->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
					}
				}

				Ikaros.m_pNeeded->ForceMaterial(fRed2, fGreen2, fBlue2, fAlpha, mat2);
				Ikaros.m_pModelRender->DrawModelEx(pInfo);
				if (Ikaros.m_pCvars->bChamsThroughWalls)
				{
					mat2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				}
				else
				{
					mat2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				}
			}
		}
		else
		{
			Ikaros.m_pModelRender->ForcedMaterialOverride(NULL);
		}
	}

	int bRet = Ikaros.m_pModelRender->DrawModelEx(pInfo);

	Ikaros.m_pModelRender->ForcedMaterialOverride(NULL);

	pModelHook->Hook((DWORD)new_DrawModelEx, 19);

	return bRet;
}

void __stdcall CL_FixMove(CUserCmd* cmd, QAngle viewangles)
{
	Vector move, movenorm, dir, set;
	QAngle movenormang, adjusted;

	float len;

	move.Init(cmd->forwardmove, cmd->sidemove, cmd->upmove);

	movenorm = move;

	VectorNormalize(movenorm);

	len = move.Length();

	VectorAngles(movenorm, movenormang);

	if (Ikaros.m_pCvars->bAntiAim)
	{
		if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
		{
			if (viewangles.x >= 0.0f && viewangles.x <= 89.0f)
			{
				viewangles.x = cmd->viewangles.x + 180.0f;
			}
			if (viewangles.x <= 0.0f && viewangles.x >= -89.0f)
			{
				viewangles.x = cmd->viewangles.x - 180.0f;
			}
		}
	}

	adjusted = movenormang + (cmd->viewangles - viewangles);

	AngleVectors(adjusted, &dir);

	set = dir * len;

	if (Ikaros.m_pCvars->bAntiAim)
	{
		if ((cmd->viewangles.x > 89.0f) || (cmd->viewangles.x < -89.0f))
			cmd->forwardmove = set.x;
		else if ((cmd->viewangles.x == 89.0f || cmd->viewangles.x == -89.0f))
			cmd->forwardmove = -set.x;
		else
			cmd->forwardmove = set.x;

		if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
			cmd->sidemove = -set.y;
		else
			cmd->sidemove = set.y;
	}
	else
	{
		cmd->forwardmove = set.x;
		cmd->sidemove = set.y;
	}
}

void Prediction(CUserCmd* pCmd, C_BasePlayer* LocalPlayer)
{
	float curtime = Ikaros.m_pGlobals->curtime;
	float frametime = Ikaros.m_pGlobals->frametime;

	Ikaros.m_pGlobals->curtime = Ikaros.m_pGlobals->interval_per_tick * Ikaros.m_pEntity->GetTickBase(LocalPlayer);
	Ikaros.m_pGlobals->frametime = Ikaros.m_pGlobals->interval_per_tick;
	
	*(CUserCmd**)((DWORD)LocalPlayer + 0xD60) = pCmd;				// m_pCurrentCommand
	*(C_BasePlayer**)((DWORD)hClient + 0x3B2EF8) = LocalPlayer;		// m_pPredictedPlayer
	*(int*)((DWORD)hClient + 0x38FF74) = pCmd->random_seed;			// m_pPredictedRandomSeed

	//BYTE byMoveData[0xA3];
	CMoveData Data;
	memset(&Data, 0, sizeof(Data));
	Ikaros.m_pPrediction->SetupMove(LocalPlayer, pCmd, NULL, &Data);
	Ikaros.m_pGameMovement->ProcessMovement(LocalPlayer, &Data);
	Ikaros.m_pPrediction->FinishMove(LocalPlayer, pCmd, &Data);

	*(CUserCmd**)((DWORD)LocalPlayer + 0xD60) = 0;					// m_pCurrentCommand
	*(C_BasePlayer**)((DWORD)hClient + 0x3B2EF8) = 0;				// m_pPredictedPlayer
	*(int*)((DWORD)hClient + 0x38FF74) = -1;						// m_pPredictedRandomSeed

	Ikaros.m_pGlobals->curtime = curtime;
	Ikaros.m_pGlobals->frametime = frametime;
	++*(int*)((DWORD)LocalPlayer + Ikaros.m_pOffsets->TickBase);
}

float GetTraceFractionWorldProps(Vector startpos, Vector endpos) {
	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldOnly filter;

	ray.Init(startpos, endpos);

	Ikaros.m_pEnginetrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	return tr.fraction;
}

void __stdcall new_CreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	if (Ikaros.m_pInput)
	{
		CUserCmd* cmd = Ikaros.m_pInput->GetUserCmd(sequence_number);
		C_BaseCombatWeapon* m_pWeapon = Ikaros.m_pNeeded->GetBaseCombatActiveWeapon(Ikaros.m_pMyPlayer->BaseEnt());

		if (Ikaros.m_pEngine->IsInGame() && Ikaros.m_pMyPlayer->BaseEnt() && !Ikaros.m_pEngine->IsHLTV())
		{
			if (Ikaros.m_pCvars->bNameStealer)
				Ikaros.m_pESP->NameChange();

			bSendPacket = true;

			if (Ikaros.m_pEntity->GetLifeState(Ikaros.m_pMyPlayer->BaseEnt()) && m_pWeapon)
			{
				int iFlags = Ikaros.m_pEntity->GetFlags(Ikaros.m_pMyPlayer->BaseEnt());
				int MoveType = Ikaros.m_pEntity->GetMoveType(Ikaros.m_pMyPlayer->BaseEnt());
				QAngle qPunchAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
				int iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);

				if (Ikaros.m_pCvars->bBunnyHop)
					Ikaros.m_pBunnyHop->BunnyHop(cmd);
				gBacktracking.Run(cmd);//backtracking 
				Backtracking::CreateMove(cmd);
				qRealView = cmd->viewangles;

				Ikaros.m_pESP->MaterialStuff();


				auto netchannel = Ikaros.m_pEngine->GetNetChannelInfo();
				static auto module_engine = (DWORD)GetModuleHandleA("engine.dll");
				if (Ikaros.m_pCvars->LagExploit && module_engine)
				{
					bSendPacket = (sequence_number % 10) == 0;
					cmd->command_number += 5 * MULTIPLAYER_BACKUP;
					*(int*)((DWORD)module_engine + 0x39539C) += 5 * MULTIPLAYER_BACKUP; // lastoutgoingcommand
					*(int*)((DWORD)netchannel + 0x8) += 5 * MULTIPLAYER_BACKUP; // m_nOutSequenceNr
				}

				//gBacktracking.Run(cmd);//backtracking 
				if (Ikaros.m_pCvars->FastWalk)
				{
					static bool even = false;
					even = !even;

					static float move = 400;// value from cl_forwardmove
					float s_move = move * 0.5065f;
					if ((iFlags & FL_ONGROUND) && !(cmd->buttons & IN_JUMP))
					{
						if (((cmd->buttons & IN_FORWARD) && (cmd->buttons & IN_MOVELEFT)) || ((cmd->buttons & IN_BACK) && (cmd->buttons & IN_MOVERIGHT)))
						{
							if (even)
							{
								cmd->sidemove += s_move;
								cmd->forwardmove += s_move;
							}
							else
							{
								cmd->sidemove -= s_move;
								cmd->forwardmove -= s_move;
							}
						}
						else if (((cmd->buttons & IN_FORWARD) && (cmd->buttons & IN_MOVERIGHT)) || ((cmd->buttons & IN_BACK) && (cmd->buttons & IN_MOVELEFT)))
						{
							if (even)
							{
								cmd->sidemove += s_move;
								cmd->forwardmove -= s_move;
							}
							else
							{
								cmd->sidemove -= s_move;
								cmd->forwardmove += s_move;
							}
						}
						else if ((cmd->buttons & IN_FORWARD) || (cmd->buttons & IN_BACK))
						{
							if (even)cmd->sidemove += s_move;
							else cmd->sidemove -= s_move;
						}
						else if ((cmd->buttons & IN_MOVELEFT) || (cmd->buttons & IN_MOVERIGHT))
						{
							if (even)cmd->forwardmove += s_move;
							else cmd->forwardmove -= s_move;
						}
					}
				}

				if (Ikaros.m_pCvars->CircleStrafe)
				{
					if (GetAsyncKeyState(0x12))
					{
						if (!(iFlags & FL_ONGROUND))
						{
							static float StrafeAngle;

							Vector Velocity = Ikaros.m_pEntity->GetVelocity(Ikaros.m_pMyPlayer->BaseEnt());
							Velocity.z = 0;

							float Speed = Velocity.Length();
							if (Speed < 45) Speed = 45;
							if (Speed > 750) Speed = 750;

							float FinalPath = GetTraceFractionWorldProps(Ikaros.m_pMyPlayer->BaseEnt()->GetAbsOrigin() + Vector(0, 0, 10), Ikaros.m_pMyPlayer->BaseEnt()->GetAbsOrigin() + Vector(0, 0, 10) + Velocity / 2.0f);
							float DeltaAngle = max((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / Ikaros.m_pGlobals->interval_per_tick)) * 4.f, 2.0f);
							StrafeAngle += DeltaAngle;

							if (fabs(StrafeAngle) >= 360.0f) {
								StrafeAngle = 0.0f;
								active = false;
							}
							else
							{
								cmd->forwardmove = cos((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
								cmd->sidemove = sin((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
							}
						}
					}
				}

				if (Ikaros.m_pCvars->FakeWalk)
				{
					C_BaseEntity* pBaseEntity = Ikaros.m_pMyPlayer->BaseEnt();
					static int iChoked = -1;
					iChoked++;

					if (iChoked < 3)
					{
						bSendPacket = false;
						cmd->tick_count += 10;
						cmd += 7 + cmd->tick_count % 2 ? 0 : 1;
						cmd->buttons |= pBaseEntity->GetMoveType() == IN_BACK;
						cmd->forwardmove = cmd->sidemove = 0.f;
					}
					else
					{
						bSendPacket = true;
						iChoked = -1;
						Ikaros.m_pGlobals->frametime *= (Ikaros.m_pEntity->GetVelocity(pBaseEntity).Length2D()) / 1.f;
						cmd->buttons |= pBaseEntity->GetMoveType() == IN_FORWARD;
					}

				}

				Ikaros.m_pCvar->FindVar("cl_pred_optimize")->SetValue(0);
				Prediction(cmd, (C_BasePlayer*)Ikaros.m_pMyPlayer->Ent());
				
				if (Ikaros.m_pCvars->bAimActive)
					Ikaros.m_pAimbot->AimAtTarget(cmd, m_pWeapon);

				if (Ikaros.m_pCvars->iPNoSpread == 2)
				{
					if (cmd->buttons & IN_ATTACK)
					{
						cmd->random_seed = 141;

						for (int iCmd = cmd->command_number;; iCmd++)
						{
							UINT uSeed = MD5_PseudoRandom(iCmd) & 0x7FFFFFF;
							uSeed &= 255;

							if (uSeed == 141)
							{
								cmd->command_number = iCmd;
								break;
							}
						}
					}
				}

				if (Ikaros.m_pCvars->bTriggerActive)
					Ikaros.m_pAimbot->Seedtrigger(cmd);
				if ((!Ikaros.m_pCvars->bAntiAim
					&& (Ikaros.m_pCvars->iAntiAimTypeX == 3 || Ikaros.m_pCvars->iAntiAimTypeX == 4))
					|| (Ikaros.m_pCvars->bAntiAim && Ikaros.m_pCvars->iAntiAimTypeX != 3))
				{
					if (Ikaros.m_pCvars->iNoRecoil == 2)
					{
						cmd->viewangles -= qPunchAngle * 2;
					}

					if (Ikaros.m_pCvars->iNoSpread == 2)
					{
						float flSpread[2];
						Ikaros.m_pNoSpread->GetSpreadFix(cmd->random_seed, cmd->viewangles, flSpread);
						cmd->viewangles.x += flSpread[0];
						cmd->viewangles.y += flSpread[1];
						cmd->viewangles.z += flSpread[2];
					}
				}

				if (Ikaros.m_pCvars->bAimActive && Ikaros.m_pCvars->bSilent && Ikaros.m_pCvars->bPSilent && Ikaros.m_pCvars->iAim)
				{
					if (Ikaros.m_pNeeded->CrashCheck())
					{
						float flServer = (float)Ikaros.m_pEntity->GetTickBase(Ikaros.m_pMyPlayer->BaseEnt()) * Ikaros.m_pGlobals->interval_per_tick;
						float flPrimaryAttack = Ikaros.m_pEntity->GetNextPrimaryAttack(m_pWeapon);

						bool bCanshoot = false;

						if (flPrimaryAttack <= flServer)
							bCanshoot = true;

						if (cmd->buttons & IN_ATTACK && bCanshoot)
						{
							if (Ikaros.m_pCvars->bAimActive && Ikaros.m_pCvars->bAiming)
								cmd->viewangles = Ikaros.m_pAimbot->qAim;

							if (Ikaros.m_pCvars->iNoRecoil == 1)
							{
								cmd->viewangles -= qPunchAngle * 2;
							}

							if (Ikaros.m_pCvars->iNoSpread != 0)
							{
								float flSpread[2];
								Ikaros.m_pNoSpread->GetSpreadFix(cmd->random_seed, cmd->viewangles, flSpread);
								cmd->viewangles.x += flSpread[0];
								cmd->viewangles.y += flSpread[1];
								cmd->viewangles.z += flSpread[2];
							}
							bSendPacket = false;
						}
						else
						{
							cmd->viewangles = qRealView;
							bSendPacket = true;
						}

						if (cmd->buttons & IN_ATTACK && !bCanshoot)
						{
						cmd->buttons &= ~IN_ATTACK;
						}
						else if (cmd->buttons & IN_ATTACK && bCanshoot)
						{
						if (iChokedPackets <= 10)
						{
						if (Ikaros.m_pCvars->bAimActive && Ikaros.m_pCvars->bAiming)
						cmd->viewangles = Ikaros.m_pAimbot->qAim;

						if (Ikaros.m_pCvars->iNoRecoil == 1)
						{
						cmd->viewangles -= qPunchAngle * 2;
						}

						if (Ikaros.m_pCvars->iNoSpread == 1)
						{
						float flSpread[2];
						Ikaros.m_pNoSpread->GetSpreadFix(cmd->random_seed, cmd->viewangles, flSpread);
						cmd->viewangles.x += flSpread[0];
						cmd->viewangles.y += flSpread[1];
						cmd->viewangles.z += flSpread[2];
						}

						bSendPacket = false;
						}
						else
						{
						bSendPacket = true;
						iChokedPackets = 0;
						}
						}
						else
						{
						cmd->viewangles = qRealView;
						bSendPacket = true;
						}
					}
				}
				else
				{

				}

				if (Ikaros.m_pCvars->bAutoPistol)
					Ikaros.m_pAimbot->AutoPistol(cmd, m_pWeapon);

				if (Ikaros.m_pCvars->Airstuck)
				{
					if (GetAsyncKeyState(VK_SHIFT) && !(cmd->buttons & IN_ATTACK))
						cmd->tick_count = INT_MAX;
				}

				if (Ikaros.m_pCvars->FakeDuck && Ikaros.m_pGameMovement->LadderMove() && cmd->buttons & IN_DUCK)
				{
					static int ticks = 0;

					if (ticks != 0)
					{
						bSendPacket = false;
						ticks--;
					}
					else
					{
						cmd->buttons &= ~IN_DUCK;
						ticks = 4;
						bSendPacket = true;
					}
				}

				if (Ikaros.m_pCvars->ChatSpam)
				{
					if (Ikaros.m_pCvars->iChatSpam == 1)
					{
						if (!(cmd->tick_count % (int)(0.5f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("coverme");
					}
					else if (Ikaros.m_pCvars->iChatSpam == 2)
					{
						if (!(cmd->tick_count % (int)(0.5f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("sectorclear");
					}
					else if (Ikaros.m_pCvars->iChatSpam == 3)
					{
						if (!(cmd->tick_count % (int)(0.5f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("enemydown");
					}
				}

				if (Ikaros.m_pCvars->ChatSpam)
				{
					if (Ikaros.m_pCvars->iChatSpam2 == 1)
					{
						if (!(cmd->tick_count % (int)(1.2f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("say ez");
					}
					else if (Ikaros.m_pCvars->iChatSpam2 == 2)
					{
						if (!(cmd->tick_count % (int)(1.2f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("say noob");
					}
					else if (Ikaros.m_pCvars->iChatSpam2 == 3)
					{
						if (!(cmd->tick_count % (int)(1.2f / g_pGlobals->interval_per_tick)))
							Ikaros.m_pEngine->ClientCmd("say shit server");
					}
				}

				static int spamwait = 0;

				if (Ikaros.m_pCvars->ChatClear)
				{
					//private code
				}

				if (Ikaros.m_pCvars->bEnemyCheck && Ikaros.m_pAntiAim->EnemyPlayersAlive())
				{
					Ikaros.m_pCvars->fakelag = false;
				}
				else
				{
					Ikaros.m_pCvars->fakelag = true;
				}

				if (Ikaros.m_pCvars->fakelag)
				{
					if (Ikaros.m_pCvars->iFakelagMode == 0)
					{
						static int fake_tick_count = 0;

						if (fake_tick_count == 0)
						{
							fake_tick_count = Ikaros.m_pCvars->FakelagAmount;

							bSendPacket = true;
						}
						else
						{
							fake_tick_count--;

							bSendPacket = false;
						}
					}
				}
				if (Ikaros.m_pCvars->fakelag)
				{
					if (Ikaros.m_pCvars->iFakelagMode == 1)
					{
						static auto packets_choked = 0;
						{
							bSendPacket = packets_choked >= 14 || breaking_lag_compensation;
							!bSendPacket ? ++packets_choked : packets_choked = 0;
						}

						if (bSendPacket)
						{
							SavedAngles = cmd->viewangles;
							last_sent_origin = Ikaros.m_pEntity->GetOrigin(Ikaros.m_pMyPlayer->BaseEnt());
						}
					}
				}

				if (!(Ikaros.m_pNeeded->IsDefusing(m_pWeapon, cmd)
					|| Ikaros.m_pNeeded->IsCurWepGrenade(m_pWeapon)))

					if (Ikaros.m_pCvars->bAntiAim)
						Ikaros.m_pAntiAim->BackFlipAngles(cmd, m_pWeapon);

				NormalizeAngles(cmd->viewangles);

				if (Ikaros.m_pCvars->iThirdpersonAA == 0)
				{
					if (bSendPacket)
						SavedAngles = cmd->viewangles;
				}
				else if (Ikaros.m_pCvars->iThirdpersonAA == 1)
				{
					SavedAngles = cmd->viewangles;
				}

				CL_FixMove(cmd, qRealView);

				if (Ikaros.m_pCvars->iFakelagMode == 1)
				{
					static auto packets_choked = 0;
					if (packets_choked >= 14 || (Ikaros.m_pCvars->fakelag && packets_choked >= Ikaros.m_pCvars->FakelagAmount))
						bSendPacket = true;
				}
			}
		}
	}
}