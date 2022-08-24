#include "Backtrack.h"

BacktrackData headPositions[24][12];

CBacktracking gBacktracking;
#define PI 3.14159265358979323846f
#define M_PI 3.14159265358979323846
//=======================================================================
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir) 
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset); 

	return (Point - PerpendicularPoint).Length();
}

inline Vector angle_vector(QAngle meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
//=======================================================================



Vector GetHitboxPOS(CBaseEntity* Player, int HitboxID)
{
	matrix3x4_t matrix[128];
	if (!Player->SetupBones(matrix, 128, 0x00000100, GetTickCount64())) return Vector(0, 0, 0);
	const model_t* mod = Player->GetModel();
	if (!mod) return Vector(0, 0, 0);
	studiohdr_t* hdr = Ikaros.m_pModelinfo->GetStudiomodel(mod);
	if (!hdr) return Vector(0, 0, 0);
	mstudiohitboxset_t* set = hdr->pHitboxSet(0);
	if (!set) return Vector(0, 0, 0);
	mstudiobbox_t* hitbox = set->pHitbox(HitboxID);
	if (!hitbox) return Vector(0, 0, 0);
	Vector vMin, vMax, vCenter, sCenter;
	VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
	vCenter = (vMin + vMax) *0.5f;
	return vCenter;
}




void CBacktracking::Run(CUserCmd* cmd)
{
	int iBestTarget = -1;
	float bestFov = 99999;
	float backtrack_tick = 0;
	
	C_BaseEntity *pLocal = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer())->GetBaseEntity();

	if (!pLocal)
		return;
	if (!Ikaros.m_pCvars->backtrack)
		return;
	player_info_t pinfo;
	for (auto index = 1; index <= g_pEngine->GetMaxClients(); index++)
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
			|| !Ikaros.m_pEngine->GetPlayerInfo(index, &pinfo)
			|| Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber() == 0
			|| Ikaros.m_pMyPlayer->BaseEnt()->GetTeamNumber() == 1)

			continue;
		
		
		Vector hitboxpos = GetHitboxPOS(pBaseEntity, Ikaros.m_pCvars->iAimSpot);
		headPositions[index][cmd->command_number % 13] = BacktrackData{ cmd->tick_count, hitboxpos }; 

		Vector ViewDir = angle_vector(cmd->viewangles);
		
		float FOVDistance = distance_point_to_line(hitboxpos, Ikaros.m_pEntity->GetEyePos(pBaseEntity), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			iBestTarget = index;
		}


		if (iBestTarget != -1)
		{
			int bestTick = 0;
			float tempFOV = 9999;
			float bestFOV = 30;
			Vector lowestDistTicks(180, 180, 0);
			for (int t = 0; t < 12; ++t)
			{
				Vector ViewDir = angle_vector(cmd->viewangles);
			
				float tempFOV = distance_point_to_line(headPositions[iBestTarget][t].hitboxpos, Ikaros.m_pEntity->GetEyePos(pBaseEntity), ViewDir);
				if (bestFOV > tempFOV)
				{
					bestTick = t, bestFOV = tempFOV;
					backtrack_tick = t;
				}
			}

			if (cmd->buttons & IN_ATTACK)
				cmd->tick_count = headPositions[index][bestTick].tickcount;
			//pBaseEntity->SetupBones(headPositions[index][bestTick].matrix, 128, 256, Ikaros.m_pGlobals->curtime);
		}

	}

}
