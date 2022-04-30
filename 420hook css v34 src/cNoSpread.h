#ifndef _CNOSPREAD_H_ 
#define _CNOSPREAD_H_

#include "Include.h"
#include "public\mathlib.h"

class cNoSpread
{
	public:
		cNoSpread();
		int m_iLastWeaponID;
		int GetWeaponID(const char *szWeaponName);
		float GetVecSpread(float &flRange);
		void GetSpreadFix(UINT seed, QAngle &qIn, float *fOut);
};


#endif
