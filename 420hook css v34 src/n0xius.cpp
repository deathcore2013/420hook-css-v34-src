#include "n0xius.h"

Interfaces Ikaros;

Interfaces::Interfaces()
{
	m_pMyPlayer = new LocalPlayer();
	m_pOffsets = new C_Offsets;
	m_pNetVar = new C_NetVars;
	m_pNeeded = new Needed();
	m_pAimbot = new cCAimbot();
	m_pNoSpread = new cNoSpread();
	m_pDraw = new CDraw();
	m_pMenu = new CMenu();
	m_pCvars = new cVars();
	m_pMouse = new cMouse();
	m_pEntity = new CEntity();
	m_pSettings = new cIni();
	m_pESP = new Esp();
	m_pBunnyHop = new cBhop();
	m_pAntiAim = new cAntiAim();
	m_pRadar = new cRadar();
	m_pEvents = new cGameEventManager();
}