#pragma once
//#include "SDK.h"
#include "include.h"

struct BacktrackData
{
	int tickcount;
	Vector hitboxpos;
	matrix3x4_t matrix[128];
};

class CBacktracking
{
public:
	void Run(CUserCmd* cmd);

private:

};

extern BacktrackData headPositions[24][12]; 
extern CBacktracking gBacktracking;