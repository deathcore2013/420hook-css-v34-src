#include "CAimbot.h"

void NormalizeAngles(QAngle& angles)
{
	while (angles.x > 180.0)
		angles.x -= 360.0;

	while (angles.x < -180.0)
		angles.x += 360.0;

	while (angles.y > 180.0)
		angles.y -= 360.0;

	while (angles.y < -180.0)
		angles.y += 360.0;

	while (angles.z > 180.0)
		angles.z -= 360.0;

	while (angles.z < -180.0)
		angles.z += 360.0;
}

cCAimbot::cCAimbot(void)
{
	flBestDist = 99999.9f;
	m_nTarget = -1;
}

void cCAimbot::DropTarget(void)
{
	flBestDist = 99999.9f;
	m_nTarget = -1;
}

int	cCAimbot::iGetTarget(void)
{
	return m_nTarget;
}

typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM,
	BULLET_PLAYER_MP5,
	BULLET_PLAYER_357,
	BULLET_PLAYER_BUCKSHOT,
	BULLET_PLAYER_CROWBAR,

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,

	// CS
	BULLET_PLAYER_45ACP,
	BULLET_PLAYER_338MAG,
	BULLET_PLAYER_762MM,
	BULLET_PLAYER_556MM,
	BULLET_PLAYER_50AE,
	BULLET_PLAYER_57MM,
	BULLET_PLAYER_357SIG,

} Bullet;

void GetBulletTypeParameters(int iBulletType, float &fPenetrationPower, float &flPenetrationDistance)
{
	if (iBulletType == BULLET_PLAYER_50AE)
	{
		fPenetrationPower = 30;
		flPenetrationDistance = 1000.0;
	}
	else if (iBulletType == BULLET_PLAYER_762MM)
	{
		fPenetrationPower = 39;
		flPenetrationDistance = 5000.0;
	}
	else if (iBulletType == BULLET_PLAYER_556MM)
	{
		fPenetrationPower = 35;
		flPenetrationDistance = 4000.0;
	}
	else if (iBulletType == BULLET_PLAYER_338MAG)
	{
		fPenetrationPower = 45;
		flPenetrationDistance = 8000.0;
	}
	else if (iBulletType == BULLET_PLAYER_9MM)
	{
		fPenetrationPower = 21;
		flPenetrationDistance = 800.0;
	}
	else if (iBulletType == BULLET_PLAYER_BUCKSHOT)
	{
		fPenetrationPower = 0;
		flPenetrationDistance = 0.0;
	}
	else if (iBulletType == BULLET_PLAYER_45ACP)
	{
		fPenetrationPower = 15;
		flPenetrationDistance = 500.0;
	}
	else if (iBulletType == BULLET_PLAYER_357SIG)
	{
		fPenetrationPower = 25;
		flPenetrationDistance = 800.0;
	}
	else if (iBulletType == BULLET_PLAYER_57MM)
	{
		fPenetrationPower = 30;
		flPenetrationDistance = 2000.0;
	}
	else
	{
		fPenetrationPower = 0;
		flPenetrationDistance = 0.0;
	}
}

static void GetMaterialParameters(int iMaterial, float &flPenetrationModifier, float &flDamageModifier)
{
	switch (iMaterial)
	{
	case CHAR_TEX_METAL:
		flPenetrationModifier = 0.5;  // If we hit metal, reduce the thickness of the brush we can't penetrate
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_DIRT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_CONCRETE:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.25;
		break;
	case CHAR_TEX_GRATE:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.99;
		break;
	case CHAR_TEX_VENT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_TILE:
		flPenetrationModifier = 0.65;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_COMPUTER:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_WOOD:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.6;
		break;
	default:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.5;
		break;
	}
}

bool GetWeaponInfo(C_BaseCombatWeapon* pWeapon, int iWeaponID, int &iBulletType, int &iPenetration, float &iDamage, float &flDistance, float &flRangeModifier)
{
	bool bRifleBit = *(bool*)((DWORD)pWeapon + (DWORD)0x780);
	bool bPistolBit = *(bool*)((DWORD)pWeapon + (DWORD)0x778);

	switch (iWeaponID)
	{
	case WEAPON_NULL:
		iBulletType = 0;
		iPenetration = 0;
		iDamage = 0;
		flDistance = 0.0f;
		flRangeModifier = 0.0f;
		break;
	case WEAPON_P228:
		iBulletType = BULLET_PLAYER_357SIG;
		iPenetration = 1;
		iDamage = 40;
		flDistance = 4096.0f;
		flRangeModifier = 0.8f;
		break;
	case WEAPON_GLOCK18:
		iBulletType = BULLET_PLAYER_9MM;
		iPenetration = 1;
		iDamage = bPistolBit ? 18.0f : 25.0f;
		flDistance = 4096.0f;
		flRangeModifier = 0.9f;
		break;
	case WEAPON_SCOUT:
		iBulletType = BULLET_PLAYER_762MM;
		iPenetration = 1;
		iDamage = 75;
		flDistance = 8192.0f;
		flRangeModifier = 0.98f;
		break;
	case WEAPON_MAC10:
		iBulletType = BULLET_PLAYER_45ACP;
		iPenetration = 1;
		iDamage = 29;
		flDistance = 4096.0f;
		flRangeModifier = 0.82f;
		break;
	case WEAPON_AUG:
		iBulletType = BULLET_PLAYER_762MM;
		iPenetration = 2;
		iDamage = 32;
		flDistance = 8192.0f;
		flRangeModifier = 0.96f;
		break;
	case WEAPON_ELITES:
		iBulletType = BULLET_PLAYER_9MM;
		iPenetration = 1;
		iDamage = 45;
		flDistance = 4096.0f;
		flRangeModifier = 0.75f;
		break;
	case WEAPON_FIVESEVEN:
		iBulletType = BULLET_PLAYER_57MM;
		iPenetration = 1;
		iDamage = 25;
		flDistance = 4096.0f;
		flRangeModifier = 0.885f;
		break;
	case WEAPON_UMP45:
		iBulletType = BULLET_PLAYER_45ACP;
		iPenetration = 1;
		iDamage = 30;
		flDistance = 4096.0f;
		flRangeModifier = 0.82f;
		break;
	case WEAPON_SG550:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 70;
		flDistance = 8192.0f;
		flRangeModifier = 0.98f;
		break;
	case WEAPON_GALIL:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 30;
		flDistance = 8192.0f;
		flRangeModifier = 0.98f;
		break;
	case WEAPON_FAMAS:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 30;
		flDistance = 8192.0f;
		flRangeModifier = 0.96f;
		break;
	case WEAPON_USP45:
		iBulletType = BULLET_PLAYER_45ACP;
		iPenetration = 1;
		iDamage = bPistolBit ? 30.0f : 34.0f;
		flDistance = 4096.0f;
		flRangeModifier = 0.79f;
		break;
	case WEAPON_AWP:
		iBulletType = BULLET_PLAYER_338MAG;
		iPenetration = 3;
		iDamage = 115;
		flDistance = 8192.0f;
		flRangeModifier = 0.99f;
		break;
	case WEAPON_MP5:
		iBulletType = BULLET_PLAYER_9MM;
		iPenetration = 1;
		iDamage = 26;
		flDistance = 4096.0f;
		flRangeModifier = 0.84f;
		break;
	case WEAPON_M249:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 32;
		flDistance = 8192.0f;
		flRangeModifier = 0.97f;
		break;
	case WEAPON_M4A1:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 33;
		flDistance = 8192.0f;
		flRangeModifier = bRifleBit ? 0.95f : 0.97f;
		break;
	case WEAPON_TMP:
		iBulletType = BULLET_PLAYER_9MM;
		iPenetration = 1;
		iDamage = 26;
		flDistance = 4096.0f;
		flRangeModifier = 0.84f;
		break;
	case WEAPON_G3SG1:
		iBulletType = BULLET_PLAYER_762MM;
		iPenetration = 3;
		iDamage = 80;
		flDistance = 8192.0f;
		flRangeModifier = 0.98f;
		break;
	case WEAPON_DEAGLE:
		iBulletType = BULLET_PLAYER_50AE;
		iPenetration = 2;
		iDamage = 54;
		flDistance = 4096.0f;
		flRangeModifier = 0.81f;
		break;
	case WEAPON_SG552:
		iBulletType = BULLET_PLAYER_556MM;
		iPenetration = 2;
		iDamage = 33;
		flDistance = 8192.0f;
		flRangeModifier = 0.955f;
		break;
	case WEAPON_AK47:
		iBulletType = BULLET_PLAYER_762MM;
		iPenetration = 2;
		iDamage = 36;
		flDistance = 8192.0f;
		flRangeModifier = 0.98f;
		break;
	case WEAPON_P90:
		iBulletType = BULLET_PLAYER_57MM;
		iPenetration = 1;
		iDamage = 26;
		flDistance = 4096.0f;
		flRangeModifier = 0.84f;
		break;
	default:
		iBulletType = BULLET_NONE;
		iPenetration = 0;
		iDamage = 0;
		flDistance = 0.0f;
		flRangeModifier = 0.0f;
		break;
	}

	if (iBulletType != 0)
		return true;
	else
		return false;
}

void cCAimbot::MakeVector(QAngle angle, QAngle& vector)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = (float)(angle[0] * M_PI / 180);
	yaw = (float)(angle[1] * M_PI / 180);
	tmp = (float)cos(pitch);

	vector[0] = (float)(-tmp * -cos(yaw));
	vector[1] = (float)(sin(yaw)*tmp);
	vector[2] = (float)-sin(pitch);
}

void cCAimbot::CalcAngle(Vector &src, Vector &dst, QAngle &angles)
{
	Vector tmp = dst - src;
	VectorAngles(tmp, angles);
}

float cCAimbot::GetFov(QAngle angle, Vector src, Vector dst)
{
	QAngle ang, aim;
	float fov;

	CalcAngle(src, dst, ang);
	MakeVector(angle, aim);
	MakeVector(ang, ang);

	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

	fov = acos(u_dot_v / (mag_s*mag_d)) * (360.0 / M_PI);

	return fov;
}

bool cCAimbot::GetVisible(Vector& vecAbsStart, Vector& vecAbsEnd, C_BaseEntity* pBaseEnt)
{
	player_info_t pinfo;
	trace_t tr;
	Ray_t ray;

	ray.Init(vecAbsStart, vecAbsEnd);
	Ikaros.m_pEnginetrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, NULL, &tr);

	if (tr.fraction > 0.97f)
		return true;

	if (tr.m_pEnt && pBaseEnt)
	{
		if (tr.m_pEnt->index == 0 || tr.allsolid)
			return false;

		if ((Ikaros.m_pEngine->GetPlayerInfo(tr.m_pEnt->index, &pinfo)
			|| pBaseEnt->index == tr.m_pEnt->index) && tr.fraction > 0.92)
			return true;
	}
	return false;
}

float cCAimbot::GetDistance(Vector &vAbsStart, Vector &vAbsStop)
{
	float flLength = Vector(vAbsStop - vAbsStart).Length();
	float flDistance = sqrt(flLength);

	if (flDistance < 1.0f)
		flDistance = 1.0f;

	return flDistance;
}

bool bIsLookingDown[64];

bool GetHitbox(int iHitBox, int iIndex, float flPitch, mstudiohitboxset_t *set, matrix3x4_t matrix[MAXSTUDIOBONES], Vector* out)
{
	mstudiobbox_t* pbox = set->pHitbox(iHitBox);

	// center and all the points of the hitbox hehe
	Vector points[9] = { ((pbox->bbmin + pbox->bbmax) * .5f),
		Vector(pbox->bbmin.x, pbox->bbmin.y, pbox->bbmin.z),
		Vector(pbox->bbmin.x, pbox->bbmax.y, pbox->bbmin.z),
		Vector(pbox->bbmax.x, pbox->bbmax.y, pbox->bbmin.z),
		Vector(pbox->bbmax.x, pbox->bbmin.y, pbox->bbmin.z),
		Vector(pbox->bbmax.x, pbox->bbmax.y, pbox->bbmax.z),
		Vector(pbox->bbmin.x, pbox->bbmax.y, pbox->bbmax.z),
		Vector(pbox->bbmin.x, pbox->bbmin.y, pbox->bbmax.z),
		Vector(pbox->bbmax.x, pbox->bbmin.y, pbox->bbmax.z) };

	for (int index = 0; index <= 8; ++index)
	{
		if (index != 0)
		{
			// scale down the hitbox size
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);
		}

		// transform the vector
		VectorTransform(points[index], matrix[pbox->bone], out[index]);
	}

	if (iHitBox == 12)
	{
		Vector high = ((out[3] + out[5]) * .5f);

		if ((flPitch >= 89.f) && (flPitch <= 181.f))
		{
			bIsLookingDown[iIndex] = true;
			Vector height = (((high - out[0]) / 3) * 4);
			Vector newhigh = (out[0] + (height * 0.7));

			out[0] = newhigh;
		}
		else if ((flPitch < 292.5f)
			&& (flPitch >= 271.f))
		{
			bIsLookingDown[iIndex] = false;
			out[0] -= Vector(0.f, 0.f, 1.f);
		}
		else
		{
			bIsLookingDown[iIndex] = false;
		}
	}

	return true;
}

bool cCAimbot::GetHitboxPosition(int iHitBox, Vector* vecOrigin, C_BaseEntity* pBaseEntity)
{
	matrix3x4_t pmatrix[MAXSTUDIOBONES];

	Vector vMin, vMax;

	float flPitch = *(float*)((DWORD)pBaseEntity + 0x101C);

	const model_t * model = pBaseEntity->GetModel();

	if (model)
	{
		studiohdr_t *pStudioHdr = Ikaros.m_pModelinfo->GetStudiomodel(model);

		if (pStudioHdr == NULL)
			return false;

		if (!pBaseEntity->SetupBones(pmatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return false;

		mstudiohitboxset_t *set = pStudioHdr->pHitboxSet(0);

		if (set == NULL)
			return false;

		if (GetHitbox(iHitBox, pBaseEntity->index, flPitch, set, pmatrix, vecOrigin))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void cCAimbot::AutoPistol(CUserCmd* pCmd, C_BaseCombatWeapon* m_pWeapon)
{
	int iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);

	if (iWeaponID == WEAPON_FIVESEVEN || iWeaponID == WEAPON_DEAGLE || iWeaponID == WEAPON_ELITES || iWeaponID == WEAPON_P228 || iWeaponID == WEAPON_GLOCK18 || iWeaponID == WEAPON_USP45)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			float flCurTime = (float)Ikaros.m_pEntity->GetTickBase(Ikaros.m_pMyPlayer->BaseEnt()) * Ikaros.m_pGlobals->interval_per_tick;
			float flNextAttack = Ikaros.m_pEntity->GetNextPrimaryAttack(m_pWeapon);

			if (flNextAttack > flCurTime)
			{
				pCmd->buttons &= ~IN_ATTACK;
				pCmd->buttons |= IN_ATTACK;
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
	}
}

int iDelay = 0;

bool __fastcall bCanHit(UINT iSeed, QAngle qCurAng, CUserCmd* pCmd)
{
	trace_t tr;
	Ray_t ray;
	QAngle qFix;
	Vector vEyePos = Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt());//PREDICT(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), Ikaros.m_pMyPlayer->BaseEnt(), Ikaros.m_pGlobals->interval_per_tick);
	Vector vEnd;
	player_info_t pInfo;

	if (Ikaros.m_pCvars->iTriggerbot == 1
		|| Ikaros.m_pCvars->iTriggerbot == 2)
	{
		float flNew[2];
		Ikaros.m_pNoSpread->GetSpreadFix(iSeed, qCurAng, flNew);
		qFix.x = flNew[0];
		qFix.y = flNew[1];
		qFix.z = flNew[2];
		qCurAng -= qFix;
	}

	if (Ikaros.m_pCvars->iTriggerbot == 1
		|| Ikaros.m_pCvars->iTriggerbot == 2)
	{
		QAngle qPunchAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
		QAngle qRecoil = qPunchAngle * 2;

		qCurAng += qRecoil;
	}

	AngleVectors(qCurAng, &vEnd);

	vEnd = vEnd * 8192 + vEyePos;
	ray.Init(vEyePos, vEnd);

	Ikaros.m_pEnginetrace->TraceRay(ray, 0x46004003, 0, &tr);

	if (tr.m_pEnt == NULL)
		return false;

	if (tr.m_pEnt->GetIClientEntity() == NULL)
	{
		iDelay = 0;
		return false;
	}

	Ikaros.m_pEngine->GetPlayerInfo(tr.m_pEnt->index, &pInfo);

	if (!Ikaros.m_pEntity->GetLifeState(tr.m_pEnt))
	{
		iDelay = 0;
		return false;
	}

	if (Ikaros.m_pEntity->GetTeamID(tr.m_pEnt) <= 1)
	{
		iDelay = 0;
		return false;
	}

	if (!Ikaros.m_pCvars->bTrigTeam)
	{
		if (Ikaros.m_pEntity->GetTeamID(tr.m_pEnt) == Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()))
			return false;
	}

	if (tr.m_pEnt->index <= 0)
		return false;

	if (Ikaros.m_pPlayerlist[tr.m_pEnt->index].bFriend)
		return false;

	if (tr.allsolid)
		return false;

	if (Ikaros.m_pCvars->iTrigBone == 0)
	{
		if (tr.hitbox >= 1 && tr.hitbox <= 12)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 1)
	{
		if (tr.hitbox == 12)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 2)
	{
		if (tr.hitbox == 11 || tr.hitbox == 10)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 3)
	{
		if (tr.hitbox == 9)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 4)
	{
		if (tr.hitbox <= 17 && tr.hitbox >= 13)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 5)
	{
		if (tr.hitbox >= 1 && tr.hitbox <= 8)
			return true;
		else
			return false;
	}
	if (Ikaros.m_pCvars->iTrigBone == 6)
	{
		if (Ikaros.m_pCvars->bTrigHead)
		{
			if (tr.hitbox == 12)
				return true;
			else
				return false;
		}
		if (Ikaros.m_pCvars->bTrigChest)
		{
			if (tr.hitbox == 11 || tr.hitbox == 10)
				return true;
			else
				return false;
		}
		if (Ikaros.m_pCvars->bTrigPelvis)
		{
			if (tr.hitbox == 9)
				return true;
			else
				return false;
		}
		if (Ikaros.m_pCvars->bTrigArms)
		{
			if (tr.hitbox <= 17 && tr.hitbox >= 13)
				return true;
			else
				return false;
		}
		if (Ikaros.m_pCvars->bTrigLegs)
		{
			if (tr.hitbox >= 1 || tr.hitbox <= 8)
				return true;
			else
				return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (tr.hitbox == Ikaros.m_pCvars->iTrigBone)
			if (tr.m_pEnt->index > 0 || !tr.allsolid)
				return true;
			else
				return false;
		else
			return false;
	}
}


void __fastcall cCAimbot::Seedtrigger(CUserCmd* pCmd)
{
	if (Ikaros.m_pEngine->IsInGame() == false
		|| Ikaros.m_pEngine->IsHLTV() == true
		|| Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber() == 0
		|| Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber() == 1)
		return;

	if (Ikaros.m_pCvars->bSpeed && Ikaros.m_pNeeded->IsKeyPressed(Ikaros.m_pCvars->iSpeedKey))
		return;

	if (!Ikaros.m_pCvars->bAutoTrigger)
	{
		if (!Ikaros.m_pNeeded->IsKeyPressed(Ikaros.m_pCvars->iTrigKey))
		{
			iDelay = 0;
			return;
		}
	}

	QAngle qView;
	Ikaros.m_pEngine->GetViewAngles(qView);

	iDelay++;

	if ((Ikaros.m_pCvars->iTrigDelay) > 0)
	{
		if (iDelay < ((Ikaros.m_pCvars->iTrigDelay / 10) / 2))
			return;
	}

	if ((Ikaros.m_pCvars->iTriggerbot == 1) || (Ikaros.m_pCvars->iTriggerbot == 0))
	{
		if (bCanHit(pCmd->random_seed, qView, pCmd))
		{
			pCmd->buttons |= IN_ATTACK;
			if (Ikaros.m_pCvars->iTriggerbot == 1)
			{
				pCmd->random_seed = 141;

				for (int iCmd = pCmd->command_number;; iCmd++)
				{
					UINT uSeed = MD5_PseudoRandom(iCmd) & 0x7FFFFFF;
					uSeed &= 255;

					if (uSeed == 141)
					{
						pCmd->command_number = iCmd;
						break;
					}
				}
			}
		}
	}
	else if (Ikaros.m_pCvars->iTriggerbot == 2)
	{
		for (int Seed = 0; Seed < 256; Seed++)
		{
			if (bCanHit(Seed, qView, pCmd))
			{
				for (int iCmd = pCmd->command_number;; iCmd++)
				{
					UINT uSeed = MD5_PseudoRandom(iCmd) & 0x7FFFFFF;
					uSeed &= 255;

					if (uSeed == Seed)
					{
						pCmd->random_seed = Seed;
						pCmd->command_number = iCmd;
						pCmd->buttons |= IN_ATTACK;
						break;
					}
				}
			}
		}
	}

	if (pCmd->buttons & IN_ATTACK)
		iDelay = 0;
}

void SmoothAngles(QAngle &src, QAngle &back, QAngle &flLocalAngles, float smoothx, float smoothy, bool bAdvanced)
{
	float smoothdiff[2];
	src[0] -= flLocalAngles.x;
	src[1] -= flLocalAngles.y;
	NormalizeAngles(src);
	if (bAdvanced)
	{
		smoothdiff[0] = src[0] / smoothx;
		smoothdiff[1] = src[1] / smoothy;
	}
	else
	{
		smoothdiff[0] = src[0] / smoothx;
		smoothdiff[1] = src[1] / smoothx;
	}
	back[0] = flLocalAngles.x + smoothdiff[0];
	back[1] = flLocalAngles.y + smoothdiff[1];
	back[2] = flLocalAngles.z;
	NormalizeAngles(back);
}

bool CanShootThrought(Vector start, Vector end, C_BaseCombatWeapon* pWeapon)
{
	try
	{
		trace_t tr;
		Ray_t Ray;
		Vector vecSrc, vecEnd, vecDir, tmpVec;
		float flRangeModifier, flPenetrationDistance, flCurrentDistance = 0.0f;
		int iPenetration;
		int iPenetrationPower = 0;
		float fCurrentDamage, flLength = 0.0f;
		float flTempLength = 0.0f;
		int iBulletType = 0;
		float flTravelledDistance = 0.0f;
		VectorCopy(start, vecSrc);
		vecDir = end - start;
		flLength = VectorLength(vecDir);
		vecDir /= flLength;

		if (pWeapon == NULL)
			return false;

		int iWeaponID = Ikaros.m_pEntity->GetWeaponID(pWeapon);

		if (iWeaponID == 0)
			return false;

		GetWeaponInfo(pWeapon, iWeaponID, iBulletType, iPenetration, fCurrentDamage, flCurrentDistance, flRangeModifier);

		float fPenetrationPower = 0.0f;
		GetBulletTypeParameters(iBulletType, fPenetrationPower, flPenetrationDistance);
		iPenetrationPower = (int)fPenetrationPower;

		float flDamageModifier = 0.5f;

		C_BaseEntity *me = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer())->GetBaseEntity();

		if (me == NULL)
			return false;

		C_BaseEntity *pBaseEntity = NULL;

		while (iPenetration || fCurrentDamage > 0)
		{
			vecEnd = vecSrc + vecDir * 10;

			if (flTravelledDistance > flCurrentDistance)
				return false;

			Ray.Init(vecSrc, vecEnd);
			Ikaros.m_pEnginetrace->TraceRay(Ray, 0x4600400B, 0, &tr);

			if (tr.m_pEnt)
				pBaseEntity = tr.m_pEnt;

			Ray.Init(vecSrc, vecEnd);
			Ikaros.m_pEnginetrace->TraceRay(Ray, 0x4600400B, 0, &tr);
			surfacedata_t *g_pSurfaceData = Ikaros.m_pPhysicAPI->GetSurfaceData(tr.surface.surfaceProps);
			int iMaterial = g_pSurfaceData->game.material;

			float flPenetrationPowerModifier = 0.0f;

			GetMaterialParameters(iMaterial, flPenetrationPowerModifier, flDamageModifier);

			iPenetrationPower *= flPenetrationPowerModifier;

			flTravelledDistance += 10;

			if (tr.fraction != 1.0)
			{
				VectorSubtract(tr.endpos, start, tmpVec);

				flTempLength = VectorLength(tmpVec);

				fCurrentDamage *= pow(flRangeModifier, ((tr.fraction * flTempLength) / 500));
				if (tr.m_pEnt->GetCollideable()->GetSolid() == SOLID_BSP)
					fCurrentDamage *= flDamageModifier;
				else
					fCurrentDamage *= 0.75;

				if (flTempLength >= flLength)
				{
					if (Ikaros.m_pCvars->bBodyAWP && iWeaponID == WEAPON_AWP)
					{
						if (fCurrentDamage > 34)
							return true;
						else
							return false;
					}
					else
					{
						if (fCurrentDamage >= 5)
							return true;
						else
							return false;
					}
				}

				iPenetration--;
			}
			else
			{
				VectorSubtract(tr.endpos, start, tmpVec);

				flTempLength = VectorLength(tmpVec);

				if (flTempLength >= flLength)
				{
					if (Ikaros.m_pCvars->bBodyAWP && iWeaponID == WEAPON_AWP)
					{
						if (fCurrentDamage > 34)
							return true;
						else
							return false;
					}
					else
					{
						if (fCurrentDamage >= 5)
							return true;
						else
							return false;
					}
				}
			}
			vecSrc = vecEnd;
		}
		return false;
	}
	catch (...)
	{
		return false;
	}
}

int NextShot, NextTmp;

void cCAimbot::AimAtTarget(CUserCmd* c, C_BaseCombatWeapon* m_pWeapon)
{
	if (!Ikaros.m_pEngine->IsInGame()
		|| Ikaros.m_pEngine->IsHLTV()
		|| !Ikaros.m_pMyPlayer->BaseEnt()
		|| !Ikaros.m_pEntity->HasAmmo(m_pWeapon))
		return;

	Ikaros.m_pCvars->bAiming = false;

	if (!Ikaros.m_pCvars->bAutoAim)
	{
		if (!Ikaros.m_pNeeded->IsKeyPressed(Ikaros.m_pCvars->iAimKey))
			return;
	}

	DropTarget();

	player_info_t pinfo;
	QAngle qPunchAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
	float flPunchLenght = qPunchAngle.Length();
	int tmpNextShot = INT_MAX,
		iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);

	for (int index = Ikaros.m_pGlobals->maxClients; index >= 0; --index)
	{
		if (index == Ikaros.m_pEngine->GetLocalPlayer())
			continue;

		IClientEntity* ClientEntity = Ikaros.m_pEntList->GetClientEntity(index);

		if (ClientEntity == NULL)
			continue;

		C_BaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();

		if (pBaseEntity == NULL
			|| pBaseEntity->IsDormant()
			|| !Ikaros.m_pEntity->GetLifeState(pBaseEntity)
			|| !Ikaros.m_pEngine->GetPlayerInfo(index, &pinfo))
			continue;

		if (Ikaros.m_pPlayerlist[index].bFriend)
			continue;

		if (!Ikaros.m_pCvars->bTeamAim)
		{
			if (Ikaros.m_pEntity->GetTeamID(pBaseEntity) == Ikaros.m_pEntity->GetTeamID(Ikaros.m_pMyPlayer->BaseEnt()))
				continue;
		}

		if (Ikaros.m_pCvars->bDM)
		{
			if (Ikaros.m_pEntity->GetHealth(pBaseEntity) <= 0 || Ikaros.m_pEntity->GetHealth(pBaseEntity) > 100)
				continue;
		}
		else
		{
			if (Ikaros.m_pEntity->GetHealth(pBaseEntity) <= 0)
				continue;
		}

		float flRange = 0.0f;
		Ikaros.m_pNoSpread->GetVecSpread(flRange);

		if (GetDistance(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), Ikaros.m_pEntity->GetEyePos(pBaseEntity)) > flRange)
			continue;

		for (int i = 12; i >= 0; --i)
		{
			int iHitbox = i;

			if (Ikaros.m_pCvars->iAim)
			{
				if (!Ikaros.m_pCvars->bHitscan)
				{
					iHitbox = Ikaros.m_pCvars->iAimSpot;
				}
				if (Ikaros.m_pCvars->bBodyAWP && Ikaros.m_pCvars->iAim && iWeaponID == WEAPON_AWP)
				{
					iHitbox = 9;
				}
			}
			else
			{
				iHitbox = Ikaros.m_pCvars->iAimSpot;
			}

			//for (int in = 0; in <= 9; ++in)
			{
				Vector out[9];

				if (!GetHitboxPosition(iHitbox, out, pBaseEntity))
					continue;

				vPlayer = out[/*(Ikaros.m_pCvars->bMultipoint && Ikaros.m_pCvars->iAim) ? in : */0];

				if (Ikaros.m_pCvars->bAutoWall && Ikaros.m_pCvars->iAim)
				{
					if (!GetVisible(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer, pBaseEntity))
					{
						if (!CanShootThrought(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer, m_pWeapon))
							continue;
					}
				}
				else
				{
					if (!GetVisible(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer, pBaseEntity))
						continue;
				}

				if (bIsLookingDown[index])
					vPlayer.z += Ikaros.m_pCvars->flAimHeight;
				
				if (Ikaros.m_pCvars->iAim_Mode == 0)
				{
					if (flBestDist > GetDistance(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer))
					{
						for (int i = 0; i < Ikaros.m_pCvars->backtrack_tick; i++)

							vPlayer = headPositions[index][i].hitboxpos;
						VectorCopy(vPlayer, PredictedTargetPosition);
						flBestDist = GetDistance(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer);
						m_nTarget = index;
						break;
					}
				}
				else if (Ikaros.m_pCvars->iAim_Mode == 1)
				{
					if (GetFov(Ikaros.m_pMyPlayer->BaseEnt()->GetAbsAngles(), Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer) <= Ikaros.m_pCvars->flFOV
						&& GetFov(Ikaros.m_pMyPlayer->BaseEnt()->GetAbsAngles(), Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer) < flBestDist)
					{
						VectorCopy(vPlayer, PredictedTargetPosition);
						flBestDist = GetFov(Ikaros.m_pMyPlayer->BaseEnt()->GetAbsAngles(), Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), vPlayer);
						m_nTarget = index;
						break;
					}
				}
				else if (Ikaros.m_pCvars->iAim_Mode == 2)
				{
					int nextshottmp = (pBaseEntity->index > NextShot ? 0 : 1);

					if (nextshottmp <= tmpNextShot)
					{
						VectorCopy(vPlayer, PredictedTargetPosition);
						tmpNextShot = nextshottmp;
						NextTmp = index;
						m_nTarget = index;
						break;
					}
				}
			}

			if (m_nTarget > -1)
				break;
		}
	}

	if (m_nTarget != -1)
	{
		Ikaros.m_pCvars->bAiming = true;

		if (Ikaros.m_pCvars->bAutoShoot && Ikaros.m_pCvars->bAutoAim)
			c->buttons |= IN_ATTACK;

		if (Ikaros.m_pCvars->iPNoSpread == 1 || Ikaros.m_pCvars->iPNoSpread == 2)
		{
			c->random_seed = 141;
			if (c->buttons & IN_ATTACK)
			{
				for (int iCmd = c->command_number;; iCmd++)
				{
					UINT uSeed = MD5_PseudoRandom(iCmd) & 0x7FFFFFF;
					uSeed &= 255;

					if (uSeed == 141)
					{
						c->command_number = iCmd;
						break;
					}
				}
			}
		}

		CalcAngle(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()), PredictedTargetPosition, qAim);

		if (Ikaros.m_pCvars->iAim == 1)
		{
			if (!Ikaros.m_pCvars->bPSilent)
			{
				VectorCopy(qAim, c->viewangles);

				if (Ikaros.m_pCvars->bShowShake)
				{
					if (!(Ikaros.m_pCvars->bAntiAim && (Ikaros.m_pCvars->iAntiAimTypeX == 3 || Ikaros.m_pCvars->iAntiAimTypeX == 4)))
					{
						if (Ikaros.m_pCvars->iNoRecoil == 1)
						{
							c->viewangles -= qPunchAngle * 2;
						}

						if (Ikaros.m_pCvars->iNoSpread == 1)
						{
							float flSpread[2];
							Ikaros.m_pNoSpread->GetSpreadFix(c->random_seed, c->viewangles, flSpread);
							c->viewangles.x += flSpread[0];
							c->viewangles.y += flSpread[1];
							c->viewangles.z += flSpread[2];
						}
					}
				}

				if (!Ikaros.m_pCvars->bSilent)
					Ikaros.m_pEngine->SetViewAngles(c->viewangles);

				if (!Ikaros.m_pCvars->bShowShake)
				{
					if (!(Ikaros.m_pCvars->bAntiAim && (Ikaros.m_pCvars->iAntiAimTypeX == 3 || Ikaros.m_pCvars->iAntiAimTypeX == 4)))
					{
						if (Ikaros.m_pCvars->iNoRecoil == 1)
						{
							c->viewangles -= qPunchAngle * 2;
						}

						if (Ikaros.m_pCvars->iNoSpread == 1)
						{
							float flSpread[2];
							Ikaros.m_pNoSpread->GetSpreadFix(c->random_seed, c->viewangles, flSpread);
							c->viewangles.x += flSpread[0];
							c->viewangles.y += flSpread[1];
							c->viewangles.z += flSpread[2];
						}
					}
				}
			}
		}
		else
		{
			QAngle SmoothedAngle, cview;

			VectorCopy(qAim, cview);

			if (!(Ikaros.m_pCvars->bAntiAim && (Ikaros.m_pCvars->iAntiAimTypeX == 3 || Ikaros.m_pCvars->iAntiAimTypeX == 4)))
			{
				if (Ikaros.m_pCvars->iRCS
					&& !((iWeaponID == WEAPON_GLOCK18)
					|| (iWeaponID == WEAPON_DEAGLE)
					|| (iWeaponID == WEAPON_M3)
					|| (iWeaponID == WEAPON_C4)
					|| (iWeaponID == WEAPON_NULL)))
				{
					cview -= qPunchAngle * 2;
				}
			}

			SmoothAngles(cview, SmoothedAngle, c->viewangles, Ikaros.m_pCvars->flSmoothX, Ikaros.m_pCvars->flSmoothY, Ikaros.m_pCvars->bAdvancedSmooth);

			Ikaros.m_pEngine->SetViewAngles(SmoothedAngle);

			if (!(Ikaros.m_pCvars->bAntiAim && (Ikaros.m_pCvars->iAntiAimTypeX == 3 || Ikaros.m_pCvars->iAntiAimTypeX == 4)))
			{
				if (Ikaros.m_pCvars->iNoRecoil == 1)
				{
					c->viewangles -= qPunchAngle * 2;
				}

				if (Ikaros.m_pCvars->iNoSpread == 1)
				{
					float flSpread[2];
					Ikaros.m_pNoSpread->GetSpreadFix(c->random_seed, c->viewangles, flSpread);
					c->viewangles.x += flSpread[0];
					c->viewangles.y += flSpread[1];
					c->viewangles.z += flSpread[2];
				}
			}
		}

		if (c->buttons & IN_ATTACK)
			NextShot = NextTmp;
	}
}