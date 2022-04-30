#include "backtrack1.h"



//bool Settings::Backtracking::enabled = false;
//int Settings::Backtracking::backtracktickrate = 1;

std::vector<Backtracking::BacktrackTick> ticks;
C_BaseEntity* entity;

Vector prevOrig;

typedef void(__thiscall* DrawModelExecuteFn)(void*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

#define TICK_INTERVAL			(Ikaros.m_pGlobals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
Vector GetBonePos(int nIndex, int bone)
{

	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	IClientEntity* pLocal = Ikaros.m_pEntList->GetClientEntity(nIndex);
	if (pLocal->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
	}
	return Vector{};
}
inline Vector GetBonePosition(int boneIndex)
{
	matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

	if (!entity->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return Ikaros.m_pEntity->GetOrigin(entity);


	matrix3x4_t hitbox = BoneMatrix[boneIndex];

	return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	entity = nullptr;
}
void Backtracking::RegisterTick(CUserCmd* cmd)
{
	C_BaseEntity* localPlayer = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer())->GetBaseEntity();
	if (!localPlayer)
		return;

	ticks.insert(ticks.begin(), BacktrackTick{ cmd->tick_count });
	auto& cur = ticks[0];

	// remove old ticks
	while (ticks.size() > TIME_TO_TICKS(Ikaros.m_pCvars->bttiks / 1000.0f))
		ticks.pop_back();

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
			//player->GetIndex() == localPlayer->GetIndex())
			continue;

		std::vector<Vector> bones;
		static studiohdr_t* pStudioModel = Ikaros.m_pModelinfo->GetStudiomodel(pBaseEntity->GetModel());
		if (!pStudioModel)
			continue;

		for (int i = 0; i < pStudioModel->numbones; i++) {
			bones.emplace_back(GetBonePosition(i));
		}

		BacktrackRecord record = BacktrackRecord{ pBaseEntity, bones, GetBonePosition(Ikaros.m_pCvars->iAimSpot), Ikaros.m_pEntity->GetOrigin(pBaseEntity) };
		if (pBaseEntity->SetupBones(record.Matrix3, MAXSTUDIOBONES, 0x100, Ikaros.m_pGlobals->curtime && pBaseEntity->SetupBones(record.BoneToWorldOut, MAXSTUDIOBONES, 256, Ikaros.m_pGlobals->curtime)))
			cur.records.emplace_back(record);
	}
}
void MakeVector(QAngle angle, QAngle& vector)
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
void CalcAngle(Vector &src, Vector &dst, QAngle &angles)
{
	Vector tmp = dst - src;
	VectorAngles(tmp, angles);
}
#define PI 3.14159265358979323846f
#define M_PI 3.14159265358979323846
inline Vector angle_vector(QAngle meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}


Vector GetHitboxPOS_Ent2(CBaseEntity* Player, int HitboxID)
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

void Backtracking::Run(CUserCmd* cmd)
{
	//entity = nullptr;
	C_BaseEntity* localPlayer = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer())->GetBaseEntity();
	if (!localPlayer)
		return;
	float serverTime = (float)Ikaros.m_pEntity->GetTickBase(Ikaros.m_pMyPlayer->BaseEnt()) * Ikaros.m_pGlobals->interval_per_tick;
	
	C_BaseCombatWeapon* activeWeapon = Ikaros.m_pNeeded->GetBaseCombatActiveWeapon(Ikaros.m_pMyPlayer->BaseEnt());
	//C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)entityList->GetClientEntityFromHandle(
		//localPlayer->GetActiveWeapon());
	//if (!activeWeapon || activeWeapon->GetInReload())
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
				//player->GetIndex() == localPlayer->GetIndex())
				continue;
	//CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
	//if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE)
	//	return;

		if (cmd->buttons & IN_ATTACK && Ikaros.m_pEntity->GetNextPrimaryAttack(activeWeapon) < serverTime) {
		float fov = FLT_MAX;
		int tickcount = 0;
		bool hasTarget = false;
		Vector orig;

		for (auto& tick : ticks)
		{
			for (auto& record : tick.records)
			{
				//for (auto bonepos : record.bonePos) {
					Vector angle = Ikaros.m_pEntity->GetEyePos(localPlayer);
					//Vector angle = GetHitboxPOS_Ent2(localPlayer, Ikaros.m_pCvars->iAimSpot);
					Vector viewangles1 = angle_vector(cmd->viewangles);
					
					//Vector ViewDir = Ikaros.m_pEntity->GetEyePos(Ikaros.m_pMyPlayer->BaseEnt());
					//float tmpFOV = distance_point_to_line(bonepos, Ikaros.m_pEntity->GetEyePos(localPlayer), viewangles1);
					
					//float tmpFOV = GetFov1(viewangles1, angle);
					//if (tmpFOV < fov) {
						//fov = tmpFOV;
						tickcount = tick.tickcount;
						hasTarget = true;
						pBaseEntity = record.entity;
						orig = record.origin;
					}
				}
			}
		}

		//if (pBaseEntity && hasTarget) {
		//	cmd->tick_count = tickcount;
			prevOrig = Ikaros.m_pEntity->GetOrigin(Ikaros.m_pMyPlayer->BaseEnt());
			//Ikaros.m_pEntity->GetOrigin(pBaseEntity) = orig;
			//entity->GetVecOrigin() = orig;
		}
	//}

		//if (pBaseEntity)
			//Ikaros.m_pEntity->GetOrigin(pBaseEntity) = prevOrig;

		//pBaseEntity = nullptr;
//}
//}
void Backtracking::CreateMove(CUserCmd* cmd)
{
	if (!Ikaros.m_pEngine->IsInGame())
		return;

	if (!Ikaros.m_pCvars->btt)
		return;

	if (cmd->command_number) {
		RegisterTick(cmd);
	}

	Run(cmd);
}
