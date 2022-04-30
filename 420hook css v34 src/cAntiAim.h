#ifndef __CANTIAIM_H__
#define __CANTIAIM_H__

#include "Include.h"

class cAntiAim
{
public:
	bool EnemyPlayersAlive();
	void __fastcall BackFlipAngles(CUserCmd* cmd, C_BaseCombatWeapon* m_pWeapon);
};

#endif