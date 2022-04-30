#include "cBhop.h"

bool MustActiveBhop = false;
bool bLastJumped = false;
bool bShouldFake = false;

bool GetGroundSurface(Vector vecAbsOrigin)
{
	Vector start(vecAbsOrigin), end;

	// Straight down
	start.x = 0.0f;
	end = end * 3 + start;
	end.x = 0.0f;
	end.z -= 64;

	Ray_t ray;
	trace_t	trace;
	ray.Init(vecAbsOrigin, end);
	Ikaros.m_pEnginetrace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, 0, &trace);

	if (trace.fraction == 1.0f)
		return false;	// no ground
	else
		return true;
}

void ApplyStrafe(CUserCmd* cmd, const QAngle& va)
{
	float yaw, speed;

	Vector& move = *(Vector*)&cmd->forwardmove;

	speed = move.Length2D();

	yaw = RAD2DEG(atan2(move.y, move.x));
	yaw = DEG2RAD(cmd->viewangles.y - va.y + yaw);

	move.x = cos(yaw) * speed;
}

void cBhop::AutoStrafe(CUserCmd* cmd, int iFlags)
{
	if (!Ikaros.m_pCvars->iAim)
	{
		if (Ikaros.m_pCvars->bAiming)
			return;
	}
	else
	{
		if (!Ikaros.m_pCvars->bSilent)
		{
			if (Ikaros.m_pCvars->bAiming)
				return;
		}
	}

	if (!(cmd->buttons & IN_BACK) && !(cmd->buttons & IN_FORWARD))
	{
		Vector vVelocity = Ikaros.m_pEntity->GetVelocity(Ikaros.m_pMyPlayer->BaseEnt());
		float Speed = sqrt(vVelocity.x * vVelocity.x + vVelocity.y * vVelocity.y);

		float Tickrate;

		if (Ikaros.m_pGlobals->interval_per_tick * 100 > 1)
			Tickrate = 1.1;
		else
			Tickrate = 1;

		static float OldY;
		float Difference = AngleNormalize(cmd->viewangles.y - OldY);

		if (Ikaros.m_pCvars->iAutoStrafe == 1)
		{
			if (Difference > 0.0f)
			{
				cmd->sidemove = -400;
			}
			if (Difference < 0.0f)
			{
				cmd->sidemove = 400;
			}
		}

		if (Ikaros.m_pCvars->iAutoStrafe == 2)
		{
			QAngle strafe = cmd->viewangles;
			float Value = (8.15 - Tickrate) - (Speed / 340);

			if (Speed > 160 && Speed < 420)
			{
				Value = (4.6 - Tickrate) - (Speed / 340);
			}
			if (Speed > 420)
			{
				Value = (3 - Tickrate) - (Speed / 1000);
			}
			if (Value <= 0.275)
			{
				Value = 0.275;
			}

			if (abs(Difference) < Value)
			{
				static bool Flip = false;
				if (Flip)
				{
					strafe.y -= Value;
					cmd->sidemove = -400;
					Flip = false;
				}
				else
				{
					strafe.y += Value;
					cmd->sidemove = 400;
					Flip = true;
				}

				ApplyStrafe(cmd, strafe);
			}
			else
			{
				if (Difference > 0.0f)
				{
					cmd->sidemove = -400;
				}
				if (Difference < 0.0f)
				{
					cmd->sidemove = 400;
				}
			}
		}

		OldY = cmd->viewangles.y;
	}
}

void cBhop::BunnyHop(CUserCmd* cmd)
{
	if (Ikaros.m_pEngine->IsInGame() == false
		|| Ikaros.m_pEngine->IsHLTV() == true
		|| Ikaros.m_pMyPlayer->BaseEnt() == NULL)
		return;

	int iFlags = Ikaros.m_pEntity->GetFlags(Ikaros.m_pMyPlayer->BaseEnt());
	int MoveType = Ikaros.m_pEntity->GetMoveType(Ikaros.m_pMyPlayer->BaseEnt());

	if (!(iFlags & FL_ONGROUND) && !(MoveType == MOVETYPE_LADDER) && !(MoveType == MOVETYPE_NOCLIP))
	{
		MustActiveBhop = true;
	}
	else
	{
		MustActiveBhop = false;
	}

	if (Ikaros.m_pEntity->GetLifeState(Ikaros.m_pMyPlayer->BaseEnt()))
	{
		if (!bLastJumped && bShouldFake) {
			bShouldFake = false;
			cmd->buttons |= IN_JUMP;
		}
		else if (cmd->buttons & IN_JUMP) {
			if (!MustActiveBhop) {
				bLastJumped = true;
				bShouldFake = true;
			}
			else {
				cmd->buttons &= ~IN_JUMP;
				cmd->buttons |= IN_JUMP;
				cmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else {
			bLastJumped = false;
			bShouldFake = false;
		}

		if (MustActiveBhop)
		{
			//if (GetGroundSurface(Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt())))
			//	cmd->buttons |= IN_DUCK;

			AutoStrafe(cmd, iFlags);
		}
		
	}
}