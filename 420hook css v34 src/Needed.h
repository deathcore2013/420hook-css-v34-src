#ifndef __NEEDED_H__
#define __NEEDED_H__

#include "Include.h"

class Needed
{
public:
	void FontInit( void );
	void ForceMaterial(float cRed, float cGreen, float cBlue, float cAlpha, IMaterial* material);
	IMaterial* CreateMaterial(bool shouldIgnoreZ);
	void Normalize(Vector &vIn, Vector &vOut);
	bool ScreenTransform( const Vector &point, Vector &screen );
	bool WorldToScreen( const Vector &vOrigin, Vector &vScreen );
	bool ScreenTransformRadar( const Vector &point, Vector &screen );
	bool WorldToScreenRadar( const Vector &vOrigin, Vector &vScreen );
	void GetWorldSpaceCenter( C_BaseEntity* pBaseEnt, Vector& vWorldSpaceCenter );
	C_BaseCombatWeapon* Needed::GetBaseCombatActiveWeapon ( C_BaseEntity* pEntity );
	bool CrashCheck();
	bool IsKeyPressed(BYTE bKey);
	bool IsDefusing(C_BaseCombatWeapon* m_pWeapon, CUserCmd* cmd);
	bool IsCurWepKnife(C_BaseCombatWeapon* m_pWeapon);
	bool IsCurWepGrenade(C_BaseCombatWeapon* m_pWeapon);
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask);
	void ReadMemory(DWORD Address, LPVOID Buffer, SIZE_T Size);
	void WriteMemory(DWORD Address, LPVOID Buffer, SIZE_T Size);
	DWORD FindPattern(const char* szModule, const char* szSignature);
public:
	vgui::HFont m_ESPFont;
};

#endif
