#ifndef _CVARS_H_
#define _CVARS_H_
#include "Include.h"

#define g_pEngine Ikaros.m_pEngine
#define g_pEngineTrace Ikaros.m_pEnginetrace
#define g_pClient Ikaros.m_pClient
#define g_pRenderView Ikaros.m_pRender
#define g_pPrediction Ikaros.m_pPrediction
#define g_pGameMovement Ikaros.m_pGameMovement
#define g_pModelRender Ikaros.m_pModelRender
#define g_pEntityList Ikaros.m_pEntList
#define g_pMaterialSurface Ikaros.m_pMatSurface
#define g_pSurface Ikaros.m_pSurface
#define g_pCVar Ikaros.m_pCvar
#define g_pModelInfo Ikaros.m_pModelinfo
#define g_pInput Ikaros.m_pInput
#define g_pGlobals Ikaros.m_pGlobals
#define g_pGameEventManager Ikaros.m_pGameEventManager
#define g_pMaterialSystem Ikaros.m_pMaterialSystem
#define g_pStudioRender Ikaros.m_pStudioRender
#define g_pPhysics Ikaros.m_pPhysicAPI
#define g_pPanel Ikaros.m_pPanel
#define g_pNetChannel Ikaros.m_pNetChannel

class cVars
{
public: 
	bool bAutoAim, bTeamAim, bAutoShoot, bSilent, bPSilent, bAutoWall, bAimActive, bShowShake, bBodyAWP, bDM, bAdvancedSmooth, bHitscan, bMultipoint;
	bool bTrigHead, bTrigChest, bTrigPelvis, bTrigArms, bTrigLegs, bTrigTeam;
	float flFOV, flAimHeight, flSmoothX, flSmoothY, iAimSpot;
	int iAim, iAim_Mode;
	
	int iAimKey, iSpeedKey, iTrigKey;
		
	bool bAntiAim, bEdgeAntiAim, bAimAtTargets;
	int iAntiAimTypeX, iAntiAimTypeY;
	float flAA_X, flAA_Y, flA_Y;

	bool bESP, bNameESP, bRadar2D, bRadarName, bEnemyOnlyESP, bEnemyOnlyRadar, bBoneESP, bAimspotESP;
	int iHealthESP, i2DBox;

	bool bNoSmoke, bNoFlash, bFullbright, bBunnyHop, bNoHands, bAutoPistol, bSpeed, bPanicKey, bMenuActive, bShowGUI, bTriggerActive, RadioKillInformer, Wireframe, fakelag, KnifeAA, Cheats, Airstuck, FakeDuck, LagExploit, ChatSpam, CircleStrafe, ChatClear, FastWalk, Knifebot, KillSay, LagFix, bHandsWireframe, bNetFakelag, bRecoilCrosshair, bShowVelocity, bRainbowOutline, bRainbowInline, bPreview, bEntityOnlyEsp, LineToPlayer, bButtonClickSound, PreviewColors, bChamsThroughWalls, ArmorBarESP, FakeWalk;
	bool bChams, bXQZWalls, bWeaponChams, bRadarBG, bMonitorActive, bWatermark,  bGlassChams, bPlayerList;
	bool bAutoTrigger,btt, backtrack,btskelet,bNameStealer,btmode, bNoSky, bNoVisRecoil, bAsusWalls, bNightMode, bAiming, bSigFail, bHitSound, bFlashSpam, bEnemyCheck, bChatRepeat;
	int iSavedRadarX, iSavedRadarY, tiks ,iCrosshair, iMenuX, iMenuY, iMenuW, iMenuH, iMonitorPosX, iMonitorPosY, iMonitorSavePosX, iMonitorSavePosY, iRoundsay, iChatSpam, iChatSpam2, iOpenTabs, iPlayerChamsMaterial, iKillSay, iAutoBuy, iHandsPosition, iWatermark, iOpenTabs2, iRecoilCrosshair, iDrawCloseButton, iWeaponChamsMaterial, iDisconnectReason, iSpinMode, iFakelagMode, iThirdpersonAA;
	int iTriggerbot, roundsay,lahfix, iTrigBone,fakepingkey,backtracktr, iNoSpread, iPNoSpread,fakeping,fakepingammout,fakepingammout2, iNoRecoil, iRCS, iMouseX, iMouseY, iAutoStrafe;
	float flSavedPos_x, flSavedPos_y, bttiks,b,a, flRadarPos_x, flRadarPos_y, backtrack_tick, flStartTime, flAsusAlpha, iSpeedVal, iTrigDelay, FakelagAmount, flNightMode, ViewmodelFOV, OverrideFOV, DrawColor, DrawColor2, DrawColor3, DrawColor4, DrawColor5, DrawColor6, DrawColor7, DrawColor8, DrawColor9, DrawColor10, DrawColor11, DrawColor12, DrawColor13, DrawColor14, DrawColor15, DrawColor16, DrawColor17, DrawColor18, DrawColor19, DrawColor20, CircleSpeed, bRadarSize, DrawColor21, DrawColor22, DrawColor23, DrawColor24, iNetFakelag, DrawColor25, DrawColor26, DrawColor27, DrawColor28, DrawColor29, DrawColor30, ChatClearTime, SpinSpeed, DrawColor31, DrawColor32, DrawColor33, DrawColor34, BlackOut, bRadarRange, WindowsTextColor1, WindowsTextColor2, WindowsTextColor3, WindowsTextColor4, GlassChamsColor;
};

#endif