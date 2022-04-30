#ifndef __N0XIUS_H__
#define __N0XIUS_H__

#include "Include.h"

class LocalPlayer;
class cCAimbot;
class cNoSpread;
class Needed;
class C_NetVars;
class C_Offsets;
class CDraw;
class cVars;
class cMouse;
class CMenu;
class CEntity;
class cIni;
class Esp;
class cBhop;
class cAntiAim;
class cRadar;
class cGameEventManager;
struct sPlayer
{
	int index;
	int pIndex;
	int iPitch;
	int iYaw;
	float iAddYaw;
	float iAdvancedYaw;
	float iAddJitterYaw;
	bool bFriend;
	bool bAdvancedYaw;
	bool bJitterYaw;
	bool bStaticYaw;
};

class Interfaces
{
public:
	Interfaces::Interfaces();
	IBaseClientDLL*			m_pClient;
	IClientMode*			m_pClientMode;
	IVRenderView*			m_pRender;
	//IPrediction*			m_pPrediction;
	CPrediction*			m_pPrediction;
	CGameMovement*			m_pGameMovement;
	IVModelRender*			m_pModelRender;
	IClientEntityList*		m_pEntList;
	IMatSystemSurface*		m_pMatSurface;
	vgui::ISurface*			m_pSurface;
	ICvar*					m_pCvar;
	IVEngineClient*			m_pEngine;
	IEngineTrace*			m_pEnginetrace;
	IVModelInfoClient*		m_pModelinfo;
	CInput*					m_pInput;
	CGlobalVarsBase*		m_pGlobals;
	IGameEventManager2*		m_pGameEventManager;
	IMaterialSystem*		m_pMaterialSystem;
	IStudioRender*			m_pStudioRender;
	IPhysicsSurfaceProps*	m_pPhysicAPI;
	vgui::IPanel*			m_pPanel;
	IGameConsole*			m_pGameConsole;
	INetChannel* m_pNetChannel;
public:
	LocalPlayer*			m_pMyPlayer;
	Needed*					m_pNeeded;
	cCAimbot*				m_pAimbot;
	cNoSpread*				m_pNoSpread;
	C_Offsets*				m_pOffsets;
	C_NetVars*				m_pNetVar;
	CDraw*					m_pDraw;
	cVars*					m_pCvars;
	cMouse*					m_pMouse;
	CMenu*					m_pMenu;
	CEntity*				m_pEntity;
	cIni*					m_pSettings;
	Esp*					m_pESP;
	cBhop*					m_pBunnyHop;
	cAntiAim*				m_pAntiAim;
	cRadar*					m_pRadar;
	cGameEventManager*		m_pEvents;
	sPlayer					m_pPlayerlist[65];
};

extern Interfaces Ikaros;

class LocalPlayer
{
public:
	//=====================================
	inline IClientEntity *Ent(void) const
	{
		return Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer());
	}
	//=====================================
	inline C_BaseEntity *BaseEnt(void) const
	{
		return Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer())->GetBaseEntity();
	}
	//=====================================
};
#endif