#include "cAntiAim.h"

void EdgeDetect(CUserCmd* cmd)
{
	if (!Ikaros.m_pMyPlayer->BaseEnt())
		return;

	Vector localPosition = Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt()); 

	for (int y = 0; y < 360; y++)
	{
		QAngle tmp(10.0f, cmd->viewangles.y, 0.0f);
		tmp.y += y;
		NormalizeAngles(tmp);

		Vector forward;
		AngleVectors(tmp, &forward);

		float length = ((16.0f + 3.0f) + ((16.0f + 3.0f) * sin(DEG2RAD(10.0f)))) + 7.0f;
		forward *= length;

		Ray_t ray;
		trace_t traceData;

		ray.Init(localPosition, (localPosition + forward));
		Ikaros.m_pEnginetrace->TraceRay(ray, 0x200400B, 0, &traceData);

		if (traceData.fraction != 1.0f)
		{
			QAngle angles;
			Vector Negate = traceData.plane.normal;

			Negate *= -1;
			VectorAngles(Negate, angles);

			tmp.y = angles.y;
			NormalizeAngles(tmp);
			trace_t leftTrace, rightTrace;

			Vector left, right;
			AngleVectors(tmp + QAngle(0.0f, 45.0f, 0.0f), &left);
			AngleVectors(tmp - QAngle(0.0f, 45.0f, 0.0f), &right);

			left *= (length + (length * sin(DEG2RAD(30.0f))));
			right *= (length + (length * sin(DEG2RAD(30.0f))));

			ray.Init(localPosition, (localPosition + left));
			Ikaros.m_pEnginetrace->TraceRay(ray, 0x200400B, 0, &leftTrace);

			ray.Init(localPosition, (localPosition + right));
			Ikaros.m_pEnginetrace->TraceRay(ray, 0x200400B, 0, &rightTrace);

			if ((leftTrace.fraction == 1.0f)
				&& (rightTrace.fraction != 1.0f))
			{
				tmp.y -= 45.0f;
				// LEFT
			}
			else if ((leftTrace.fraction != 1.0f)
				&& (rightTrace.fraction == 1.0f))
			{
				tmp.y += 45.0f;
				// RIGHT
			}

			cmd->viewangles.y = tmp.y;
		}
	}
}

static void RunAimPlayerBehindWall(CUserCmd* Cmd)
{
	if (!Ikaros.m_pMyPlayer->BaseEnt())
		return;

	Vector EyePos = Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt());

	player_info_t m_pInfo;
	float flBestDis = 8192.0f;

	for (int i = 64; i >= 1; --i)
	{
		if (i == Ikaros.m_pEngine->GetLocalPlayer())
			continue;

		C_BaseEntity* TargetPlayer = (C_BaseEntity*)Ikaros.m_pEntList->GetClientEntity(i);

		if (TargetPlayer == NULL)
			continue;

		if ((TargetPlayer->GetTeamNumber() == Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber())
			|| (TargetPlayer->IsDormant())
			|| !Ikaros.m_pEntity->GetLifeState(TargetPlayer)
			|| !Ikaros.m_pEngine->GetPlayerInfo(i, &m_pInfo)
			|| Ikaros.m_pPlayerlist[TargetPlayer->index].bFriend
			|| Ikaros.m_pEntity->GetHealth(TargetPlayer) <= 0)
		{
			continue;
		}

		Vector FinalTargetPosition = Ikaros.m_pEntity->GetEyePos(TargetPlayer);

		if (flBestDis > Ikaros.m_pAimbot->GetDistance(EyePos, FinalTargetPosition))
		{
			flBestDis = Ikaros.m_pAimbot->GetDistance(EyePos, FinalTargetPosition);
			Ikaros.m_pAimbot->CalcAngle(EyePos, FinalTargetPosition, Cmd->viewangles);
		}
	}
}

void SetAntiAimY(CUserCmd* cmd)
{
	float fTime = Ikaros.m_pEngine->Time();

	Vector vVelocity = Ikaros.m_pEntity->GetVelocity(Ikaros.m_pMyPlayer->BaseEnt());

	static bool flip;
	static bool flip2;
	static float spin;
	flip = !flip;
	flip2 = !flip2;

	if (Ikaros.m_pCvars->bAimAtTargets)
	{
		RunAimPlayerBehindWall(cmd);
	}
	
	if (Ikaros.m_pCvars->iAntiAimTypeY == 0)
	{
		if (bSendPacket)
			cmd->viewangles.y += 180.0f;
		else
			cmd->viewangles.y -= 180.0f;
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 1)
	{
		if (!Ikaros.m_pCvars->bAimAtTargets)
		{
			if (flip)
			{
				cmd->viewangles.y = 270.0f;
			}
			else
			{
				cmd->viewangles.y = 90.0f;
			}
		}
		else
		{
			if (flip)
			{
				cmd->viewangles.y -= 270.0f;
			}
			else
			{
				cmd->viewangles.y -= 90.0f;
			}
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 2)
	{
		cmd->viewangles.y -= Ikaros.m_pCvars->flAA_Y;
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 3)
	{
		if (Ikaros.m_pCvars->iSpinMode == 0)
		{
			cmd->viewangles.y = (vec_t)(fmod(fTime / 2.0f * 360.0f, 360.0f));
		}
		else if (Ikaros.m_pCvars->iSpinMode == 1)
		{
			cmd->viewangles.y = (vec_t)(fmod(fTime / 1.0f * 360.0f, 360.0f));
		}
		else if (Ikaros.m_pCvars->iSpinMode == 2)
		{
			cmd->viewangles.y = (vec_t)(fmod(fTime / 0.5f * 360.0f, 360.0f));
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 4)
	{
		if (Ikaros.m_pCvars->fakelag && (Ikaros.m_pCvars->FakelagAmount != 0))
		{
			if (bSendPacket)
			{
				cmd->viewangles.y += 90.0f;
			}
			else
			{
				cmd->viewangles.y -= 90.0f;
			}
		}
		else
		{
			if (flip)
			{
				cmd->viewangles.y += 90.0f;
				bSendPacket = false;
			}
			else
			{
				cmd->viewangles.y -= 90.0f;
				bSendPacket = true;
			}
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 5)
	{
		if (Ikaros.m_pCvars->fakelag && (Ikaros.m_pCvars->FakelagAmount != 0))
		{
			if (flip)
			{
				if (flip2)
					cmd->viewangles.y += 90.0f;
				else
					cmd->viewangles.y -= 90.0f;
			}
			else
			{
				cmd->viewangles.y += 180.0f;
			}
		}
		else
		{
			if (flip)
			{
				bSendPacket = false;
				if (flip2)
					cmd->viewangles.y += 90.0f;
				else
					cmd->viewangles.y -= 90.0f;
			}
			else
			{
				bSendPacket = true;
				cmd->viewangles.y += 180.0f;
			}
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeY == 6) // Fake Forward
	{
		if (bSendPacket)
		{
			cmd->viewangles.y = 180.0f;
		}
		else
		{
			cmd->viewangles.y = 180.0f;
		}
	}
	if (Ikaros.m_pCvars->iAntiAimTypeY == 7)
	{
		if (bSendPacket)
		{
			spin += 15.0f;
			cmd->viewangles.y = spin > 50.0f ? 150.0f : -150.0f;

			if (spin > 100.0f)
				spin = 0.0f;
		}
	}
	if (Ikaros.m_pCvars->iAntiAimTypeY == 8)
	{
		if (Ikaros.m_pCvars->fakelag && (Ikaros.m_pCvars->FakelagAmount != 0))
		{
			if (bSendPacket)
			{
				cmd->viewangles.y += Ikaros.m_pCvars->flAA_Y;
			}
			else
			{
				cmd->viewangles.y += Ikaros.m_pCvars->flA_Y;
			}
		}
	}
	if (Ikaros.m_pCvars->iAntiAimTypeY == 9)
	{
		if (Ikaros.m_pCvars->fakelag && (Ikaros.m_pCvars->FakelagAmount != 0))
		{
			if (bSendPacket)
			{
				cmd->viewangles.y += 180.0f;
			}
			else
			{
				cmd->viewangles.y -= 360.0f;
			}
		}
		else
		{
			if (flip)
			{
				cmd->viewangles.y += 360.0f;
				bSendPacket = false;
			}
			else
			{
				cmd->viewangles.y -= 360.0f;
				bSendPacket = true;
			}
		}
	}
	if (Ikaros.m_pCvars->iAntiAimTypeY == 10)
	{
		if (Ikaros.m_pCvars->fakelag && (Ikaros.m_pCvars->FakelagAmount != 0))
		{
			if (bSendPacket)
			{
				cmd->viewangles.y += 360.0f;
			}
			else
			{
				cmd->viewangles.y -= 180.0f;
			}
		}
		else
		{
			if (flip)
			{
				cmd->viewangles.y += 360.0f;
				bSendPacket = false;
			}
			else
			{
				cmd->viewangles.y -= 360.0f;
				bSendPacket = true;
			}
		}
	}

	if (vVelocity.Length() < 300.f)
	{
		if (Ikaros.m_pCvars->bEdgeAntiAim)
		{
			EdgeDetect(cmd);
		}
	}
}

bool cAntiAim::EnemyPlayersAlive()
{
	auto alive = 0;

	for (auto i = 1; i <= g_pEngine->GetMaxClients(); i++)
	{
		auto pBaseEntity = (C_BasePlayer*)g_pEntityList->GetClientEntity(i);

		if (!pBaseEntity
			|| pBaseEntity == Ikaros.m_pMyPlayer->BaseEnt()
			|| Ikaros.m_pPlayerlist[pBaseEntity->index].bFriend
			|| !Ikaros.m_pEntity->GetLifeState(pBaseEntity))
			continue;

		if (pBaseEntity->GetTeamNumber() != Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber())
			alive++;
	}

	return (alive == 0);
}

void __fastcall cAntiAim::BackFlipAngles(CUserCmd* cmd, C_BaseCombatWeapon* m_pWeapon)
{
	if (!Ikaros.m_pMyPlayer->BaseEnt()
	||	!m_pWeapon)
		return;

	if(Ikaros.m_pEntity->GetMoveType(Ikaros.m_pMyPlayer->BaseEnt()) == MOVETYPE_LADDER 
	|| Ikaros.m_pEntity->GetMoveType(Ikaros.m_pMyPlayer->BaseEnt()) == MOVETYPE_NOCLIP)
		return;

	static bool flip;
	flip = !flip;

	if (Ikaros.m_pCvars->bEnemyCheck && EnemyPlayersAlive()) return;
	if (Ikaros.m_pCvars->KnifeAA && Ikaros.m_pNeeded->IsCurWepKnife(m_pWeapon)) return;

	if (!(Ikaros.m_pCvars->iAntiAimTypeX == 3))
	{
		if (Ikaros.m_pCvars->bPSilent && Ikaros.m_pCvars->iAim)
		{
			if (!bSendPacket)
			{
				if (cmd->buttons & IN_ATTACK)
					return;
			}

			SetAntiAimY(cmd);

			if (Ikaros.m_pCvars->iAntiAimTypeX == 0)
			{
				cmd->viewangles.x = 89.00f;
			}

			if (Ikaros.m_pCvars->iAntiAimTypeX == 1)
			{
				cmd->viewangles.x = 180.00f;
			}

			if (Ikaros.m_pCvars->iAntiAimTypeX == 2)
			{
				cmd->viewangles.x = Ikaros.m_pCvars->flAA_X;
			}
		}
		else
		{
			if (cmd->buttons & IN_ATTACK)
				return;

			SetAntiAimY(cmd);

			if (Ikaros.m_pCvars->iAntiAimTypeX == 0)
			{
				cmd->viewangles.x = 89.00f;
			}

			if (Ikaros.m_pCvars->iAntiAimTypeX == 1)
			{
				cmd->viewangles.x = 180.00f;
			}

			if (Ikaros.m_pCvars->iAntiAimTypeX == 2)
			{
				cmd->viewangles.x = Ikaros.m_pCvars->flAA_X;
			}
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeX == 4)
	{
		cmd->viewangles.x = -89.00f;
	}

	if (Ikaros.m_pCvars->iAntiAimTypeX == 5)
	{
		cmd->viewangles.x;
	}

	if (Ikaros.m_pCvars->iAntiAimTypeX == 6)
	{
		if (flip)
		{
			cmd->viewangles.x = -89.00f;
		}
		else
		{
			cmd->viewangles.x = +89.00f;
		}
	}

	if (Ikaros.m_pCvars->iAntiAimTypeX == 3)
	{
		if (cmd->buttons & IN_ATTACK)
		{
			if (Ikaros.m_pCvars->bPSilent && Ikaros.m_pCvars->iAim)
			{
				if (bSendPacket)
				{
					SetAntiAimY(cmd);
					cmd->viewangles.x = 180.0f;
				}
			}
			else
			{
				QAngle qPunchAngle = Ikaros.m_pEntity->GetPunchAngle(Ikaros.m_pMyPlayer->BaseEnt());
				QAngle qSpread(0, 0, 0);

				QAngle qRecoil = qPunchAngle * 2;

				float flSpread[2];
				Ikaros.m_pNoSpread->GetSpreadFix(cmd->random_seed, cmd->viewangles, flSpread);
				qSpread.x += flSpread[0];
				qSpread.y += flSpread[1];
				qSpread.z += flSpread[2];

				cmd->viewangles.x = (180.0f - cmd->viewangles.x) - qRecoil.x + qSpread.x;
				cmd->viewangles.y = -((180.0f - cmd->viewangles.y) - (-qRecoil.y) + qSpread.y);
				
			}
		}
		else
		{
			SetAntiAimY(cmd);
			cmd->viewangles.x = 180.0f;
		}
	}
}