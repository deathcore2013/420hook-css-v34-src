#ifndef __CAIMBOT_H__
#define __CAIMBOT_H__

#include "include.h"

class cCAimbot
{
public:
	cCAimbot();
	//----------------------------------------------
	void	DropTarget(void);
	int		iGetTarget( void );
	void	MakeVector(QAngle angle, QAngle& vector);
	void	CalcAngle(Vector &src, Vector &dst, QAngle &angles);
	float	GetFov(QAngle angle, Vector src, Vector dst);
	bool	GetVisible( Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* pBaseEnt );
	float	GetDistance(Vector &vAbsStart, Vector &vAbsStop);
	bool	GetHitboxPosition(int iHitBox, Vector* vecOrigin, C_BaseEntity* pBaseEntity);
	void	AutoPistol(CUserCmd* pCmd, C_BaseCombatWeapon* m_pWeapon);
	void	__fastcall Seedtrigger(CUserCmd* pCmd);
	void	AimAtTarget(CUserCmd* c, C_BaseCombatWeapon* m_pWeapon);
	int		m_nTarget;
	float	flBestDist;
	Vector	vPlayer;
	Vector	PredictedTargetPosition;
	QAngle	qAim;
};

#endif
