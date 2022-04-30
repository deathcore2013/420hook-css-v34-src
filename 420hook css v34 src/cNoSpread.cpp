#include "cNoSpread.h"

cNoSpread::cNoSpread( void )
{
	m_iLastWeaponID = WEAPON_NULL;
}

int cNoSpread::GetWeaponID(const char *szWeaponName)
{
	C_BaseCombatWeapon* m_pWeapon = Ikaros.m_pNeeded->GetBaseCombatActiveWeapon ( Ikaros.m_pMyPlayer->BaseEnt() );

	if ( !m_pWeapon )
		return WEAPON_NULL;

	//knife
	if ( strstr( szWeaponName, "_knife_t.mdl" ) )
		return WEAPON_KNIFE;

	if ( strstr( szWeaponName, "_knife_ct.mdl" ) )
		return WEAPON_KNIFE;

	//pistol
	if ( strstr( szWeaponName, "_pist_deagle.mdl" ) )
		return WEAPON_DEAGLE;

	if ( strstr( szWeaponName, "_pist_elite.mdl" ) )
		return WEAPON_ELITES;

	if ( strstr( szWeaponName, "_pist_fiveseven.mdl" ) )
		return WEAPON_FIVESEVEN;

	if ( strstr( szWeaponName, "_pist_glock18.mdl" ) )
		return WEAPON_GLOCK18;

	if ( strstr( szWeaponName, "_pist_p228.mdl" ) )
		return WEAPON_P228;

	if ( strstr( szWeaponName, "_pist_usp.mdl" ) )
		return WEAPON_USP45;

	//riffles
	if ( strstr( szWeaponName, "_rif_ak47.mdl" ) )
		return WEAPON_AK47;

	if ( strstr( szWeaponName, "_rif_aug.mdl" ) )
		return WEAPON_AUG;

	if ( strstr( szWeaponName, "_rif_m4a1.mdl" ) )
		return WEAPON_M4A1;

	if ( strstr( szWeaponName, "_rif_famas.mdl" ) )
		return WEAPON_FAMAS;

	if ( strstr( szWeaponName, "_rif_galil.mdl" ) )
		return WEAPON_GALIL;

	if ( strstr( szWeaponName, "_rif_sg552.mdl" ) )
		return WEAPON_SG552;

	if ( strstr( szWeaponName, "_mach_m249para.mdl" ) )
		return WEAPON_M249;

	//smg
	if ( strstr( szWeaponName, "_smg_tmp.mdl" ) )
		return WEAPON_TMP;

	if ( strstr( szWeaponName, "_smg_mac10.mdl" ) )
		return WEAPON_MAC10;

	if ( strstr( szWeaponName, "_smg_p90.mdl" ) )
		return WEAPON_P90;

	if ( strstr( szWeaponName, "_smg_ump45.mdl" ) )
		return WEAPON_UMP45;

	if ( strstr( szWeaponName, "_smg_mp5.mdl" ) )
		return WEAPON_MP5;

	//sniper
	if ( strstr( szWeaponName, "_snip_g3sg1.mdl" ) )
		return WEAPON_G3SG1;

	if ( strstr( szWeaponName, "_snip_scout.mdl" ) )
		return WEAPON_SCOUT;

	if ( strstr( szWeaponName, "_snip_sg550.mdl" ) )
		return WEAPON_SG550;

	if ( strstr( szWeaponName, "_snip_awp.mdl" ) )
		return WEAPON_AWP;

	//shotguns
	if ( strstr( szWeaponName, "_shot_xm1014.mdl" ) )
		return WEAPON_XM1014;

	if ( strstr( szWeaponName, "_shot_m3super90.mdl" ) )
		return WEAPON_M3;

	//misc
	if ( strstr( szWeaponName, "_eq_flashbang.mdl" ) )
		return WEAPON_FLASHBANG;

	if ( strstr( szWeaponName, "_eq_fraggrenade.mdl" ) )
		return WEAPON_HE_GRENADE;

	if ( strstr( szWeaponName, "_eq_smokegrenade.mdl" ) )
		return WEAPON_SMOKE_GRENADE;

	if ( strstr( szWeaponName, "_c4.mdl" ) )
		return WEAPON_C4;

	return WEAPON_NULL;
}

float cNoSpread::GetVecSpread(float &flRange)
{
	float flSpread = 0.0f;
	C_BaseCombatWeapon* m_pWeapon = Ikaros.m_pNeeded->GetBaseCombatActiveWeapon(Ikaros.m_pMyPlayer->BaseEnt());

	if (!m_pWeapon)
		return 0.0f;

	int iFlags = *(int*)((DWORD)Ikaros.m_pMyPlayer->BaseEnt() + Ikaros.m_pOffsets->Flags);

	float m_fFov = *(float*)((DWORD)Ikaros.m_pMyPlayer->BaseEnt() + Ikaros.m_pOffsets->FOV);

	float m_flAccuracy = *(float*)((DWORD)m_pWeapon + (DWORD)0x760);

	bool m_RifleSilenced = *(bool*)((DWORD)m_pWeapon + (DWORD)0x780);

	bool m_PistolSilenced = *(bool*)((DWORD)m_pWeapon + (DWORD)0x778);

	Vector vVelocity = Ikaros.m_pMyPlayer->BaseEnt()->m_vecVelocity;

	int iSpeed = 0;

	if (vVelocity.IsLengthLessThan(1))
		iSpeed = 0;
	else
		iSpeed = (int)vVelocity.Length2D();

	int iWeaponID = GetWeaponID(Ikaros.m_pModelinfo->GetModelName(m_pWeapon->GetModel()));

	switch (iWeaponID){
	case WEAPON_NULL:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_KNIFE:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_FLASHBANG:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_HE_GRENADE:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_SMOKE_GRENADE:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_C4:
	{
		flSpread = 0.0f;
		flRange = 0.0f;
		break;
	}
	case WEAPON_AK47:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.04 + (0.4) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.04 + (0.07) * (m_flAccuracy));
		else
			flSpread = ((0.0275) * (m_flAccuracy));
		break;
	}
	case WEAPON_AUG:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.035 + (0.4) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.035 + (0.07) * (m_flAccuracy));
		else
			flSpread = ((0.02) * (m_flAccuracy));
		break;
	}
	case WEAPON_AWP:
	{
		flRange = 8192.0f;
		if (!(iFlags & FL_ONGROUND))
		{
			if (m_fFov == 0.0f || m_fFov >= 80.0f)
				flSpread = (0.835) + 0.08;
			else
				flSpread = (0.85);
		}
		else if (vVelocity.Length2D() > 140)
		{
			if (m_fFov == 0.0f || m_fFov >= 80.0f)
				flSpread = (0.25) + 0.08;
			else
				flSpread = (0.25);
		}
		else if (vVelocity.Length2D() > 10)
		{
			if (m_fFov == 0.0f || m_fFov >= 80.0f)
				flSpread = (0.1) + 0.08;
			else
				flSpread = (0.1);
		}
		else if ((iFlags & FL_DUCKING))
		{
			if (m_fFov == 0.0f || m_fFov >= 80.0f)
				flSpread = (0.0) + 0.08;
			else
				flSpread = (0.0);
		}
		else
		{
			if (m_fFov == 0.0f || m_fFov >= 80.0f)
				flSpread = (0.001) + 0.08;
			else
				flSpread = (0.001);
		}
		break;
	}
	case WEAPON_DEAGLE:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((1.5) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = ((0.25) * (1 - m_flAccuracy));
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.115) * (1 - m_flAccuracy));
		else
			flSpread = ((0.13) * (1 - m_flAccuracy));
		break;
	}
	case WEAPON_ELITES:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((1.3) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = ((0.175) * (1 - m_flAccuracy));
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.08) * (1 - m_flAccuracy));
		else
			flSpread = ((0.1) * (1 - m_flAccuracy));
		break;
	}
	case WEAPON_FAMAS:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.030 + (0.3) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.030 + (0.07) * (m_flAccuracy));
		else
			flSpread = ((0.020) * (m_flAccuracy));
		break;
	}
	case WEAPON_FIVESEVEN:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((1.5) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = ((0.255) * (1 - m_flAccuracy));
		else if (iFlags &FL_DUCKING)
			flSpread = ((0.075) * (1 - m_flAccuracy));
		else
			flSpread = ((0.15) * (1 - m_flAccuracy));
		break;
	}
	case WEAPON_G3SG1:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.45) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = 0.15;
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.035) * (1 - m_flAccuracy));
		else
			flSpread = ((0.055) * (1 - m_flAccuracy));

		if (m_fFov >= 80.0f)
			flSpread += 0.025;
		break;
	}
	case WEAPON_GALIL:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.04 + (0.3) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.04 + (0.07) * (m_flAccuracy));
		else
			flSpread = ((0.0375) * (m_flAccuracy));
		break;
	}
	case WEAPON_GLOCK18:
	{
		flRange = 4096.0f;
		if (m_PistolSilenced)
		{
			if (!(iFlags &FL_ONGROUND))
				flSpread = ((1.2) * (1 - m_flAccuracy));
			else if (iSpeed > 5)
				flSpread = ((0.185) * (1 - m_flAccuracy));
			else if ((iFlags &FL_DUCKING))
				flSpread = ((0.095) * (1 - m_flAccuracy));
			else
				flSpread = ((0.3) * (1 - m_flAccuracy));
		}
		else
		{
			if (!(iFlags &FL_ONGROUND))
				flSpread = ((1.0) * (1 - m_flAccuracy));
			else if (iSpeed > 5)
				flSpread = ((0.165) * (1 - m_flAccuracy));
			else if ((iFlags &FL_DUCKING))
				flSpread = ((0.075) * (1 - m_flAccuracy));
			else
				flSpread = ((0.1) * (1 - m_flAccuracy));
		}
		break;
	}
	case WEAPON_M249:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.045 + (0.5) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.045 + (0.095) * (m_flAccuracy));
		else
			flSpread = ((0.03) * (m_flAccuracy));
		break;
	}
	case WEAPON_M4A1:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.035 + (0.4) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.035 + (0.07) * (m_flAccuracy));
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.095) * (1 - m_flAccuracy));
		else if (m_RifleSilenced)
			flSpread = ((0.025) * (m_flAccuracy));
		else
			flSpread = ((0.02) * (m_flAccuracy));
		break;
	}
	case WEAPON_MAC10:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.375) * (m_flAccuracy));
		else
			flSpread = ((0.03) * (m_flAccuracy));
		break;
	}
	case WEAPON_MP5:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.2) * (m_flAccuracy));
		else
			flSpread = ((0.04) * (m_flAccuracy));
		break;
	}
	case WEAPON_P228:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((1.5) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = ((0.255) * (1 - m_flAccuracy));
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.075) * (1 - m_flAccuracy));
		else
			flSpread = ((0.15) * (1 - m_flAccuracy));
		break;
	}
	case WEAPON_P90:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.3) * (m_flAccuracy));
		else if (iSpeed > 170)
			flSpread = ((0.115) * (m_flAccuracy));
		else
			flSpread = ((0.045) * (m_flAccuracy));
		break;
	}
	case WEAPON_SCOUT:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = 0.2;
		else if (iSpeed > 170)
			flSpread = 0.075;
		else if ((iFlags &FL_DUCKING))
			flSpread = 0.0;
		else
			flSpread = 0.007;

		if (m_fFov >= 80.0f)
			flSpread += 0.025;
		break;
	}
	case WEAPON_SG550:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.45) * (1 - m_flAccuracy));
		else if (iSpeed > 5)
			flSpread = 0.15;
		else if ((iFlags &FL_DUCKING))
			flSpread = ((0.04) * (1 - m_flAccuracy));
		else
			flSpread = ((0.05) * (1 - m_flAccuracy));

		if (m_fFov >= 80.0f)
			flSpread += 0.025;
		break;
	}
	case WEAPON_SG552:
	{
		flRange = 8192.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = (0.035 + (0.45) * (m_flAccuracy));
		else if (iSpeed > 140)
			flSpread = (0.035 + (0.075) * (m_flAccuracy));
		else
			flSpread = ((0.02) * (m_flAccuracy));
		break;
	}
	case WEAPON_TMP:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.25) * (m_flAccuracy));
		else
			flSpread = ((0.03) * (m_flAccuracy));
		break;
	}
	case WEAPON_UMP45:
	{
		flRange = 4096.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.24) * (m_flAccuracy));
		else
			flSpread = ((0.04) * (m_flAccuracy));
		break;
	}
	case WEAPON_USP45:
	{
		flRange = 4096.0f;
		if (m_PistolSilenced)
		{
			if (!(iFlags &FL_ONGROUND))
				flSpread = ((1.3) * (1 - m_flAccuracy));
			else if (iSpeed > 5)
				flSpread = ((0.25) * (1 - m_flAccuracy));
			else if ((iFlags &FL_DUCKING))
				flSpread = ((0.125) * (1 - m_flAccuracy));
			else
				flSpread = ((0.15) * (1 - m_flAccuracy));
		}
		else
		{
			if (!(iFlags &FL_ONGROUND))
				flSpread = ((1.2) * (1 - m_flAccuracy));
			else if (iSpeed > 5)
				flSpread = ((0.225) * (1 - m_flAccuracy));
			else if ((iFlags &FL_DUCKING))
				flSpread = ((0.08) * (1 - m_flAccuracy));
			else
				flSpread = ((0.1) * (1 - m_flAccuracy));
		}
		break;
	}
	case WEAPON_M3:
	{
		flRange = 768.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.25) * (m_flAccuracy));
		else
			flSpread = ((0.03) * (m_flAccuracy));
		break;
	}
	case WEAPON_XM1014:
	{
		flRange = 768.0f;
		if (!(iFlags &FL_ONGROUND))
			flSpread = ((0.25) * (m_flAccuracy));
		else
			flSpread = ((0.03) * (m_flAccuracy));
		break;
	}
	default:
	{
		flSpread = 0.0f;
		break;
	}
	}
	
	return flSpread;
}

float GetSpreadXY(UINT seed, Vector2D &vec)
{
	int iSeed = seed & 255;
	iSeed++;
	RandomSeed(iSeed);
	vec.x = RandomFloat(-0.5, 0.5) + RandomFloat(-0.5, 0.5);
	vec.y = RandomFloat(-0.5, 0.5) + RandomFloat(-0.5, 0.5);
	iSeed++;
	float flRange = 0.0f;
	float flSpread = Ikaros.m_pNoSpread->GetVecSpread(flRange);
	vec.x *= flSpread;
	vec.y *= flSpread;
	return flRange;
}

void cNoSpread::GetSpreadFix(UINT seed, QAngle &qIn, float *fOut)
{
	Vector forward, right, up, vecDir, view;
	QAngle dest;
	Vector2D spread;

	AngleVectors(qIn, &forward, &right, &up);
	float range = GetSpreadXY(seed, spread);

	vecDir = forward + spread.x * right + spread.y * up;
	view = range * vecDir;

	VectorNormalize(view);

	VectorAngles(view, dest);

	fOut[0] = qIn[0] - dest[0];
	fOut[1] = qIn[1] - dest[1];
	fOut[2] = 0;
}