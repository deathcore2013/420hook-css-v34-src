#ifndef __NETVAR_H__
#define __NETVAR_H__

#include "Include.h"
#include <vector>

class CEntity
{
public:
	bool GetLifeState(C_BaseEntity* pBaseEnt);
	int GetFlags(C_BaseEntity* pBaseEnt);
	QAngle GetPunchAngle(C_BaseEntity* pBaseEnt);
	int GetTeamID(C_BaseEntity* pBaseEnt);
	int GetTickBase(C_BaseEntity* pBaseEnt);
	Vector GetVelocity(C_BaseEntity* pBaseEnt);
	int GetHealth(C_BaseEntity* pBaseEnt);
	int Armor(C_BaseEntity* pBaseEnt);
	//int GetMoveType(C_BaseEntity* pBaseEnt);
	MoveType_t GetMoveType(C_BaseEntity* pBaseEnt);
	bool HasAmmo(C_BaseCombatWeapon* m_pWeapon);
	float GetNextPrimaryAttack(C_BaseCombatWeapon* m_pWeapon);
	int GetWeaponID(C_BaseCombatWeapon* m_pWeapon);
	Vector GetEyePos(C_BaseEntity* pBaseEnt);
	Vector GetOrigin(CBaseEntity* pBaseEnt);
	QAngle GetViewPos(C_BaseEntity* pBaseEnt);
};

class C_NetVars
{
public:
	void GetOffsets();

	// calls GetProp wrapper to get the absolute offset of the prop
	int GetOffset(const char *tableName, const char *propName);

	// calls GetProp wrapper to get prop and sets the proxy of the prop, returns old proxy
	RecvVarProxyFn HookProp(const char *tableName, const char *propName, RecvVarProxyFn function);

public:

	// wrapper so we can use recursion without too much performance loss
	int GetProp(const char *tableName, const char *propName, RecvProp **prop = 0);

	// uses recursion to return a the relative offset to the given prop and sets the prop param
	int GetProp(RecvTable *recvTable, const char *propName, RecvProp **prop = 0);

	RecvTable *GetTable(const char *tableName);

	std::vector<RecvTable*>	m_tables;

};

struct C_Offsets
{
	DWORD Armor;
	DWORD FOV;
	DWORD LifeState;
	DWORD Flags;
	DWORD Health;
	DWORD PunchAngle;
	DWORD TeamNum;
	DWORD TickBase;
	DWORD Clip1;
	DWORD GetInReload;
	DWORD NextPrimaryAttack;
	DWORD Velocity;
	DWORD ViewOffset;
	DWORD Origin;
	DWORD SimulationTime;
	DWORD Rotation;
	DWORD DeadFlag;
};
#endif