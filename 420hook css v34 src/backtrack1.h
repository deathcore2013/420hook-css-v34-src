#pragma once
#include "include.h"

namespace Backtracking
{
	struct BacktrackRecord
	{
		C_BaseEntity* entity;
		std::vector<Vector> bonePos;
		Vector headPos;
		Vector origin;
		matrix3x4_t Matrix3[128];
		matrix3x4_t BoneToWorldOut[128];
	};

	struct BacktrackTick
	{
		int tickcount;
		std::vector<BacktrackRecord> records;
	};

	void RegisterTick(CUserCmd* cmd);
	void Run(CUserCmd* cmd);

	//Hooks
	void CreateMove(CUserCmd* cmd);
};

extern std::vector<Backtracking::BacktrackTick> ticks;