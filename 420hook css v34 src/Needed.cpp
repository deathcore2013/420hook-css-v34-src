#include "Needed.h"

void Needed::FontInit(void)
{
	m_ESPFont = Ikaros.m_pMatSurface->CreateFont();
	Ikaros.m_pMatSurface->SetFontGlyphSet(m_ESPFont, "Tahoma", 13, 300, 0, 0, 0x200);
}

void Needed::ForceMaterial(float cRed, float cGreen, float cBlue, float cAlpha, IMaterial* material)
{
	if (material != NULL)
	{
		bool useColor = true;
		//bool forceMaterial = true;
		if (useColor)
		{
			float temp[3] =
			{
				cRed,
				cGreen,
				cBlue
			};

			float alpha = cAlpha;

			Ikaros.m_pRender->SetBlend(alpha);
			Ikaros.m_pRender->SetColorModulation(temp);
		}

		//if (forceMaterial)
		Ikaros.m_pModelRender->ForcedMaterialOverride(material);
	}
}

char * MAT_IGNOREZ(std::string type)
{
	char szBuffer[1024];
	sprintf(szBuffer, "\"%s", type.c_str());
	strcat(szBuffer, "\"\n{\n\t\"$basetexture\" \"VGUI/white_additive\"\n\t");
	strcat(szBuffer, "\"$envmap\" \"1\"\n\t");
	strcat(szBuffer, "\"$model\" \"1\"\n\t");
	strcat(szBuffer, "\"$flat\" \"1\"\n\t");
	strcat(szBuffer, "\"$nocull\" \"0\"\n\t");
	strcat(szBuffer, "\"$selfillum\" \"1\"\n\t");
	strcat(szBuffer, "\"$nofog\" \"0\"\n\t");
	strcat(szBuffer, "\"$ignorez\" \"1\"\n\t");
	strcat(szBuffer, "\"$znearer\" \"1\"\n\t");
	strcat(szBuffer, "\"$halflambert\" \"1\"\n}");

	return szBuffer;
}

char * MAT(std::string type)
{
	char szBuffer[1024];
	sprintf(szBuffer, "\"%s", type.c_str());
	strcat(szBuffer, "\"\n{\n\t\"$basetexture\" \"VGUI/white_additive\"\n\t");
	strcat(szBuffer, "\"$envmap\" \"1\"\n\t");
	strcat(szBuffer, "\"$model\" \"1\"\n\t");
	strcat(szBuffer, "\"$flat\" \"1\"\n\t");
	strcat(szBuffer, "\"$nocull\" \"0\"\n\t");
	strcat(szBuffer, "\"$selfillum\" \"1\"\n\t");
	strcat(szBuffer, "\"$nofog\" \"0\"\n\t");
	strcat(szBuffer, "\"$ignorez\" \"0\"\n\t");
	strcat(szBuffer, "\"$znearer\" \"1\"\n\t");
	strcat(szBuffer, "\"$halflambert\" \"1\"\n}");

	return szBuffer;
}

IMaterial* Needed::CreateMaterial(bool shouldIgnoreZ)
{
	static int created = 0;

	std::string type = /*(!Ikaros.m_pCvars->bGlassChams) ? */"UnlitGeneric"/* : "VertexLitGeneric"*/;

	char materialBuffer[2048];
	char* pLol;

	if (shouldIgnoreZ)
		pLol = MAT_IGNOREZ(type);
	else
		pLol = MAT(type);

	strcpy(materialBuffer, pLol);

	char materialName[512];
	sprintf(materialName, "custom_material_%i.vmt", created);
	++created;

	KeyValues* keyValues = new KeyValues(type.c_str());

	keyValues->LoadFromBuffer(materialName, materialBuffer);

	if (keyValues == NULL)
		return 0;

	IMaterial* createdMaterial = Ikaros.m_pMaterialSystem->CreateMaterial(materialName, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void Needed::Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();

	if(flLen == 0)
	{
		vOut.Init(0, 0, 1);
		return;
	}

	flLen = 1 / flLen;

	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

bool Needed::ScreenTransform( const Vector &point, Vector &screen )
{
	float w;
	const VMatrix &worldToScreen = Ikaros.m_pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w		 = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if( w < 0.001f )
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}
	return behind;
}

bool Needed::ScreenTransformRadar( const Vector &point, Vector &screen )
{
	float w;
	const VMatrix &worldToScreen = Ikaros.m_pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w		 = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;
	return behind;
}

bool Needed::WorldToScreenRadar( const Vector &vOrigin, Vector &vScreen )
{
	if(!ScreenTransformRadar( vOrigin, vScreen ))
	{
		int iScreenWidth, iScreenHeight;

		Ikaros.m_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );
		float x = iScreenWidth / 2;
		float y = iScreenHeight / 2;
		x += 0.5 * vScreen.x * iScreenWidth + 0.5;
		y -= 0.5 * vScreen.y * iScreenHeight + 0.5;
		vScreen.x = x;
		vScreen.y = y;
		return true;
	}
	return false;
}

bool Needed::WorldToScreen( const Vector &vOrigin, Vector &vScreen )
{
	if(!ScreenTransform( vOrigin, vScreen ))
	{
		int iScreenWidth, iScreenHeight;

		Ikaros.m_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );
		float x = iScreenWidth / 2;
		float y = iScreenHeight / 2;
		x += 0.5 * vScreen.x * iScreenWidth + 0.5;
		y -= 0.5 * vScreen.y * iScreenHeight + 0.5;
		vScreen.x = x;
		vScreen.y = y;
		return true;
	}
	return false;
}

void Needed::GetWorldSpaceCenter( C_BaseEntity* pBaseEnt, Vector& vWorldSpaceCenter )
{
	if ( pBaseEnt )
	{
		Vector vMin, vMax;
		pBaseEnt->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = pBaseEnt->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;
	}
}

C_BaseCombatWeapon* Needed::GetBaseCombatActiveWeapon ( C_BaseEntity* pEntity )
{
	C_BaseCombatWeapon* hResult = NULL;
	__asm
	{
		MOV  EAX, pEntity;
		MOV  EDI, EAX;
		MOV  EAX, DWORD PTR DS :[EDI];
		MOV  ECX, EDI;
		CALL DWORD PTR DS :[EAX+0x2C8]
		mov  hResult, EAX;
	}
	return hResult;
}

bool Needed::CrashCheck()
{
	if (!Ikaros.m_pEngine->IsInGame()
		|| Ikaros.m_pEngine->IsHLTV()
		|| !Ikaros.m_pEngine->IsConnected())
		return false;

	IClientEntity* ClientEntity = Ikaros.m_pEntList->GetClientEntity(Ikaros.m_pEngine->GetLocalPlayer());

	if (ClientEntity == NULL || ClientEntity->IsDormant())
		return false;

	C_BaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();

	if (pBaseEntity == NULL || pBaseEntity->IsDormant())
		return false;

	C_BaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon(pBaseEntity);

	if (!m_pWeapon)
		return false;

	return true;
}

bool Needed::IsKeyPressed(BYTE bKey)
{
	if (bKey == 0 && GetAsyncKeyState(0x04)
		|| bKey == 1 && GetAsyncKeyState(0x01)
		|| bKey == 2 && GetAsyncKeyState(0x02)
		|| bKey == 3 && GetAsyncKeyState(0x05)
		|| bKey == 4 && GetAsyncKeyState(0x06)
		|| bKey == 5 && GetAsyncKeyState(0x10)
		|| bKey == 6 && GetAsyncKeyState(0x11)
		|| bKey == 7 && GetAsyncKeyState(0x12)
		|| bKey == 8 && GetAsyncKeyState(0x51)
		|| bKey == 9 && GetAsyncKeyState(0x46)
		|| bKey == 10 && GetAsyncKeyState(0x45))
		return true;
	else
		return false;

	/*if (GetAsyncKeyState(bKey))
		return true;
	else
		return false;*/
}

bool Needed::IsDefusing(C_BaseCombatWeapon* m_pWeapon, CUserCmd* cmd)
{ 
	if (m_pWeapon == NULL)
		return false;

	int iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);
	if ((iWeaponID == WEAPON_C4)/* || (cmd->buttons & IN_USE)*/)
		return true;
	else
		return false;
}

bool Needed::IsCurWepKnife(C_BaseCombatWeapon* m_pWeapon)
{
	if (m_pWeapon == NULL)
		return false;

	int iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);
	if (iWeaponID == WEAPON_KNIFE)
		return true;
	else
		return false;
}

bool Needed::IsCurWepGrenade(C_BaseCombatWeapon* m_pWeapon)
{
	if (m_pWeapon == NULL)
		return false;

	int iWeaponID = Ikaros.m_pEntity->GetWeaponID(m_pWeapon);
	if ((iWeaponID == WEAPON_FLASHBANG) || (iWeaponID == WEAPON_HE_GRENADE) || (iWeaponID == WEAPON_SMOKE_GRENADE))
		return true;
	else
		return false;
}

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

DWORD Needed::dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

void Needed::ReadMemory(DWORD Address, LPVOID Buffer, SIZE_T Size)
{
	ReadProcessMemory(GetCurrentProcess(), (DWORD*)Address, Buffer, Size, NULL);
}

void Needed::WriteMemory(DWORD Address, LPVOID Buffer, SIZE_T Size)
{
	WriteProcessMemory(GetCurrentProcess(), (DWORD*)Address, Buffer, Size, NULL);
}

DWORD Needed::FindPattern(const char* szModule, const char* szSignature)
{
	//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;    //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}