#include "cNetVars.h"

bool CEntity::GetLifeState(C_BaseEntity* pBaseEnt)
{
	int iLifeState = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->LifeState);
	
	if ((iLifeState == LIFE_ALIVE))
		return true;
	else
		return false;
}

int CEntity::GetFlags(C_BaseEntity* pBaseEnt)
{
	int iFlags = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Flags);
	return iFlags;
}

QAngle CEntity::GetPunchAngle(C_BaseEntity* pBaseEnt)
{
	QAngle qPunchAngle = *(QAngle*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->PunchAngle);
	return qPunchAngle;
}

int CEntity::GetTeamID(C_BaseEntity* pBaseEnt)
{
	int iTeamID = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->TeamNum);
	return iTeamID;
}

int CEntity::GetTickBase(C_BaseEntity* pBaseEnt)
{
	int iTickBase = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->TickBase);
	return iTickBase;
}

Vector CEntity::GetVelocity(C_BaseEntity* pBaseEnt)
{
	Vector vVelocity = *(Vector*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Velocity);
	return vVelocity;
}

int CEntity::GetHealth(C_BaseEntity* pBaseEnt)
{
	int iHealth = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Health);
	return iHealth;
}

int CEntity::Armor(C_BaseEntity* pBaseEnt)
{
	int Armor = *(int*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Armor);
	return Armor;
}

MoveType_t CEntity::GetMoveType(C_BaseEntity* pBaseEnt)
{
	MoveType_t iMoveType = *(MoveType_t*)((DWORD)pBaseEnt + 0x134);//hardcored lol cuz i don't find the netvar
	return iMoveType;
}

bool CEntity::HasAmmo(C_BaseCombatWeapon* m_pWeapon)
{
	int iAmmo = *(int*)((DWORD)m_pWeapon + Ikaros.m_pOffsets->Clip1);
	
	if (iAmmo == 0)
		return false;
	else
		return true;
}

float CEntity::GetNextPrimaryAttack(C_BaseCombatWeapon* m_pWeapon)
{
	float flNextAttack = *(float*)((DWORD)m_pWeapon + Ikaros.m_pOffsets->NextPrimaryAttack);
	return flNextAttack;
}

int CEntity::GetWeaponID(C_BaseCombatWeapon* m_pWeapon)
{
	int iWeaponID = Ikaros.m_pNoSpread->GetWeaponID(Ikaros.m_pModelinfo->GetModelName(m_pWeapon->GetModel()));
	return iWeaponID;
}

Vector CEntity::GetEyePos(C_BaseEntity* pBaseEnt)
{
	Vector m_VecOrigin = *(Vector*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Origin);
	Vector m_vecViewOffset = *(Vector*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->ViewOffset);

	Vector EyePos = (m_VecOrigin + m_vecViewOffset);

	return EyePos;
}

Vector CEntity::GetOrigin(CBaseEntity* pBaseEnt)
{
	Vector m_vecOrigin = *(Vector*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Origin);
	return m_vecOrigin;
}

QAngle CEntity::GetViewPos(C_BaseEntity* pBaseEnt)
{
	QAngle m_angRotation = *(QAngle*)((DWORD)pBaseEnt + Ikaros.m_pOffsets->Rotation);
	return m_angRotation;
}

int C_NetVars::GetOffset(const char *tableName, const char *propName)
{
	int offset = GetProp(tableName, propName);

	if (!offset)
	{
		return 0;
	}

	return offset;
}

RecvVarProxyFn C_NetVars::HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
{
	RecvProp *recvProp = 0;

	if (!GetProp(tableName, propName, &recvProp))
	{
		return 0;
	}


	RecvVarProxyFn old = recvProp->m_ProxyFn;
	recvProp->m_ProxyFn = function;

	return old;
}

int C_NetVars::GetProp(const char *tableName, const char *propName, RecvProp **prop)
{
	RecvTable *recvTable = GetTable(tableName);

	if (!recvTable)
	{
		return 0;
	}


	int offset = GetProp(recvTable, propName, prop);

	if (!offset)
	{
		return 0;
	}


	return offset;
}

int C_NetVars::GetProp(RecvTable *recvTable, const char *propName, RecvProp **prop)
{
	int extraOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; ++i)
	{
		RecvProp *recvProp = &recvTable->m_pProps[i];


		RecvTable *child = recvProp->m_pDataTable;

		if (child
			&& (child->m_nProps > 0))
		{
			int tmp = GetProp(child, propName, prop);

			if (tmp)
			{
				extraOffset += (recvProp->m_Offset + tmp);
			}
		}

		if (stricmp(recvProp->m_pVarName, propName))
		{
			continue;
		}


		if (prop && !*prop)
		{
			*prop = recvProp;
		}

		return (recvProp->m_Offset + extraOffset);
	}

	return extraOffset;
}


RecvTable *C_NetVars::GetTable(const char *tableName)
{
	if (m_tables.empty())
	{
		return 0;
	}

	for each (RecvTable *table in m_tables)
	{
		if (!table)
		{
			continue;
		}


		if (stricmp(table->m_pNetTableName, tableName) == 0)
		{
			return table;
		}
	}

	return 0;
}

void C_NetVars::GetOffsets(void)
{
	m_tables.clear();

	ClientClass *clientClass = Ikaros.m_pClient->GetAllClasses();

	if (!clientClass)
	{
		return;
	}

	while (clientClass)
	{
		RecvTable *recvTable = clientClass->m_pRecvTable;

		m_tables.push_back(recvTable);

		clientClass = clientClass->m_pNext;
	}

	Ikaros.m_pOffsets->Rotation = Offset_BaseEntity("m_angRotation");
	Ikaros.m_pOffsets->Armor = Offset_CSPlayer("m_ArmorValue");
	Ikaros.m_pOffsets->Flags = Offset_BasePlayer("m_fFlags");
	Ikaros.m_pOffsets->FOV = Offset_BasePlayer("m_iFov");
	Ikaros.m_pOffsets->Health = Offset_BasePlayer("m_iHealth");
	Ikaros.m_pOffsets->LifeState = Offset_BasePlayer("m_lifeState");
	Ikaros.m_pOffsets->PunchAngle = Offset_BasePlayer("m_vecPunchAngle");
	Ikaros.m_pOffsets->TeamNum = Offset_BasePlayer("m_iTeamNum");
	Ikaros.m_pOffsets->TickBase = Offset_BasePlayer("m_nTickBase");
	Ikaros.m_pOffsets->SimulationTime = Offset_BasePlayer("m_flSimulationTime");
	Ikaros.m_pOffsets->Velocity = Offset_BasePlayer("m_vecVelocity[0]");
	Ikaros.m_pOffsets->ViewOffset = Offset_BasePlayer("m_vecViewOffset[0]");
	Ikaros.m_pOffsets->Origin = Offset_BaseEntity("m_vecOrigin");
	Ikaros.m_pOffsets->DeadFlag = Offset_BasePlayer("deadflag");

	Ikaros.m_pOffsets->Clip1 = Offset_BaseCombatWeapon("m_iClip1");
	Ikaros.m_pOffsets->GetInReload = Offset_BaseCombatWeapon("m_bInReload");
	Ikaros.m_pOffsets->NextPrimaryAttack = Offset_BaseCombatWeapon("m_flNextPrimaryAttack");

	//speak("Ikaros.m_pOffsets->Rotation: 0x%.8X", Ikaros.m_pOffsets->Rotation);
}
