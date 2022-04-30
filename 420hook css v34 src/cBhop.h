#ifndef __CBHOP_H__
#define __CBHOP_H__

#include "Include.h"

class cBhop
{
public:
	void AutoStrafe(CUserCmd* cmd, int iFlags);
	void BunnyHop(CUserCmd* cmd);
};

#endif