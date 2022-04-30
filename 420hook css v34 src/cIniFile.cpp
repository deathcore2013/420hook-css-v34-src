#include "cIniFile.h"

char soundsdir[255];
char cPath[255];
char cPath1[255];
char cPath2[255];

bool bIsRage = false;

bool cIni::CreateCfgFile()
{
	GetModuleFileNameA(hMod, cPath, 255);
	for (int i = strlen(cPath); i > 0; i--)
	{
		if (cPath[i] == '\\')
		{
			cPath[i + 1] = 0;
			break;
		}
	}

	HANDLE hFile, hFile2, hFile3;
	CreateDirectoryA(cPath, 0);
	strcat(cPath, "\\420hook");
	CreateDirectoryA(cPath, 0);
	strcpy(cPath1, cPath);
	strcpy(cPath2, cPath);
	strcpy(soundsdir, cPath);
	strcat(soundsdir, "\\Sounds");
	CreateDirectoryA(soundsdir, 0);
	strcat(cPath,  "\\Legitv34.cfg");
	strcat(cPath1, "\\Ragev34.cfg");
	strcat(cPath2, "\\Menuv34.cfg");

	hFile = CreateFileA(cPath, GENERIC_ALL, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(hFile);

	hFile2 = CreateFileA(cPath1, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(hFile2);

	hFile3 = CreateFileA(cPath2, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile3 == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(hFile3);

	return true;
}

int cIni::ReadInteger(char* szSection, char* szKey, int iDefaultValue)
{
	int iResult;
	iResult = GetPrivateProfileIntA(szSection, szKey, iDefaultValue, bIsRage ? cPath1 : cPath);
	return iResult;
}

float cIni::ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf(szDefault, "%f",fltDefaultValue);
	GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, bIsRage ? cPath1 : cPath);
	fltResult =  atof(szResult);
	return fltResult;
}

char* cIni::ReadString(char* szSection, char* szKey, const char* szDefaultValue)
{
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileStringA(szSection, szKey, szDefaultValue, szResult, 255, bIsRage ? cPath1 : cPath);
	return szResult;
}

void cIni::WriteInteger(char* szSection, char* szKey, int iValue)
{
	char szValue[255];
	sprintf(szValue, "%d", iValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, bIsRage ? cPath1 : cPath);
}

void cIni::WriteFloat(char* szSection, char* szKey, float fltValue)
{
	char szValue[255];
	sprintf(szValue, "%f", fltValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, bIsRage ? cPath1 : cPath);
}

void cIni::WriteString(char* szSection, char* szKey, char* szValue)
{
	WritePrivateProfileStringA(szSection, szKey, szValue, bIsRage ? cPath1 : cPath);
}

int cIni::ReadMenuInteger(char* szSection, char* szKey, int iDefaultValue)
{
	int iResult;
	iResult = GetPrivateProfileIntA(szSection, szKey, iDefaultValue, cPath2);
	return iResult;
}

void cIni::WriteMenuInteger(char* szSection, char* szKey, int iValue)
{
	char szValue[255];
	sprintf(szValue, "%d", iValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, cPath2);
}

void cIni::LoadSettings()
{
	
	Ikaros.m_pCvars->FastWalk = ReadInteger("Settings", "circle", 1);
	Ikaros.m_pCvars->bESP = ReadInteger("Settings", "bt", 1);
	Ikaros.m_pCvars->bAimspotESP = ReadInteger("Settings", "circle", 1);
	Ikaros.m_pCvars->backtrack = ReadInteger("Settings", "bt", 1);
	Ikaros.m_pCvars->backtrack_tick = ReadInteger("Settings", "bt", 12);
	Ikaros.m_pCvars->DrawColor = ReadFloat("Settings", "colors.red", 0.f);
	Ikaros.m_pCvars->DrawColor2 = ReadFloat("Settings", "colors.green", 175.f);
	Ikaros.m_pCvars->DrawColor3 = ReadFloat("Settings", "colors.blue", 255.f);
	Ikaros.m_pCvars->DrawColor4 = ReadFloat("Settings", "colors.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor5 = ReadFloat("Settings", "colors1.red", 0.f);
	Ikaros.m_pCvars->DrawColor6 = ReadFloat("Settings", "colors1.green", 0.f);
	Ikaros.m_pCvars->DrawColor7 = ReadFloat("Settings", "colors1.blue", 0.f);
	Ikaros.m_pCvars->DrawColor8 = ReadFloat("Settings", "colors1.alpha", 120.f);
	Ikaros.m_pCvars->DrawColor9 = ReadFloat("Settings", "colors2.red", 0.f);
	Ikaros.m_pCvars->DrawColor10 = ReadFloat("Settings", "colors2.green", 175.f);
	Ikaros.m_pCvars->DrawColor11 = ReadFloat("Settings", "colors2.blue", 255.f);
	Ikaros.m_pCvars->DrawColor12 = ReadFloat("Settings", "colors2.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor13 = ReadFloat("Settings", "colors3.red", 0.f);
	Ikaros.m_pCvars->DrawColor14 = ReadFloat("Settings", "colors3.green", 175.f);
	Ikaros.m_pCvars->DrawColor15 = ReadFloat("Settings", "colors3.blue", 255.f);
	Ikaros.m_pCvars->DrawColor16 = ReadFloat("Settings", "colors3.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor17 = ReadFloat("Settings", "colors4.red", 0.f);
	Ikaros.m_pCvars->DrawColor18 = ReadFloat("Settings", "colors4.green", 175.f);
	Ikaros.m_pCvars->DrawColor19 = ReadFloat("Settings", "colors4.blue", 255.f);
	Ikaros.m_pCvars->DrawColor20 = ReadFloat("Settings", "colors4.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor21 = ReadFloat("Settings", "colors5.red", 0.f);
	Ikaros.m_pCvars->DrawColor22 = ReadFloat("Settings", "colors5.green", 200.f);
	Ikaros.m_pCvars->DrawColor23 = ReadFloat("Settings", "colors5.blue", 255.f);
	Ikaros.m_pCvars->DrawColor24 = ReadFloat("Settings", "colors5.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor25 = ReadFloat("Settings", "chamstcolor.red", 1.0f);
	Ikaros.m_pCvars->DrawColor26 = ReadFloat("Settings", "chamstcolor.green", 0.2f);
	Ikaros.m_pCvars->DrawColor27 = ReadFloat("Settings", "chamstcolor.blue", 0.2f);
	Ikaros.m_pCvars->DrawColor28 = ReadFloat("Settings", "chamsctcolor.red", 0.2f);
	Ikaros.m_pCvars->DrawColor29 = ReadFloat("Settings", "chamsctcolor.green", 0.2f);
	Ikaros.m_pCvars->DrawColor30 = ReadFloat("Settings", "chamsctcolor.blue", 1.0f);
	Ikaros.m_pCvars->DrawColor31 = ReadFloat("Settings", "tabslinecolor.red", 0.f);
	Ikaros.m_pCvars->DrawColor32 = ReadFloat("Settings", "tabslinecolor.green", 175.f);
	Ikaros.m_pCvars->DrawColor33 = ReadFloat("Settings", "tabslinecolor.blue", 255.f);
	Ikaros.m_pCvars->DrawColor34 = ReadFloat("Settings", "tabslinecolor.alpha", 255.f);
	Ikaros.m_pCvars->WindowsTextColor1 = ReadFloat("Settings", "windowstextcolor.red", 255.f);
	Ikaros.m_pCvars->WindowsTextColor2 = ReadFloat("Settings", "windowstextcolor.green", 255.f);
	Ikaros.m_pCvars->WindowsTextColor3 = ReadFloat("Settings", "windowstextcolor.blue", 255.f);
	Ikaros.m_pCvars->WindowsTextColor4 = ReadFloat("Settings", "windowstextcolor.alpha", 255.f);
	Ikaros.m_pCvars->PreviewColors = ReadInteger("Settings", "colors.previewcolors", 0.f);
	Ikaros.m_pCvars->GlassChamsColor = ReadFloat("Settings", "vis.glasschamscolor", 0.5f);
}

void cIni::SaveSettings()
{
	WriteInteger("Settings", "aim.active", Ikaros.m_pCvars->bAimActive);
	WriteInteger("Settings", "aim.mode", Ikaros.m_pCvars->iAim);
	WriteInteger("Settings", "aim.targetselection", Ikaros.m_pCvars->iAim_Mode);
	WriteInteger("Settings", "aim.spot", Ikaros.m_pCvars->iAimSpot);
	WriteInteger("Settings", "aim.autoaim", Ikaros.m_pCvars->bAutoAim);
	WriteInteger("Settings", "aim.autoshoot", Ikaros.m_pCvars->bAutoShoot);
	WriteInteger("Settings", "aim.key", Ikaros.m_pCvars->iAimKey);
	WriteFloat("Settings", "aim.height", Ikaros.m_pCvars->flAimHeight);
	WriteFloat("Settings", "aim.fov", Ikaros.m_pCvars->flFOV);
	WriteInteger("Settings", "aim.smooth.advanced", Ikaros.m_pCvars->bAdvancedSmooth);
	WriteFloat("Settings", "aim.smooth.x", Ikaros.m_pCvars->flSmoothX);
	WriteFloat("Settings", "aim.smooth.y", Ikaros.m_pCvars->flSmoothY);
	WriteInteger("Settings", "aim.silent", Ikaros.m_pCvars->bSilent);
	WriteInteger("Settings", "aim.psilent", Ikaros.m_pCvars->bPSilent);
	WriteInteger("Settings", "aim.team", Ikaros.m_pCvars->bTeamAim);
	WriteInteger("Settings", "aim.autowall", Ikaros.m_pCvars->bAutoWall);
	WriteInteger("Settings", "aim.bodyawp", Ikaros.m_pCvars->bBodyAWP);
	WriteInteger("Settings", "aim.dmfix", Ikaros.m_pCvars->bDM);
	WriteInteger("Settings", "aim.hitscan", Ikaros.m_pCvars->bHitscan);
	WriteInteger("Settings", "aim.multipoint", Ikaros.m_pCvars->bMultipoint);

	WriteInteger("Settings", "antiaim.active", Ikaros.m_pCvars->bAntiAim);
	WriteInteger("Settings", "antiaim.edge", Ikaros.m_pCvars->bEdgeAntiAim);
	WriteInteger("Settings", "antiaim.aimattargets", Ikaros.m_pCvars->bAimAtTargets);
	WriteInteger("Settings", "antiaim.typex", Ikaros.m_pCvars->iAntiAimTypeX);
	WriteInteger("Settings", "antiaim.typey", Ikaros.m_pCvars->iAntiAimTypeY);
	WriteFloat("Settings", "antiaim.x", Ikaros.m_pCvars->flAA_X);
	WriteFloat("Settings", "antiaim.y", Ikaros.m_pCvars->flAA_Y);

	WriteInteger("Settings", "esp.active", Ikaros.m_pCvars->bESP);
	WriteInteger("Settings", "esp.box", Ikaros.m_pCvars->i2DBox);
	WriteInteger("Settings", "esp.name", Ikaros.m_pCvars->bNameESP);
	WriteInteger("Settings", "esp.health", Ikaros.m_pCvars->iHealthESP);
	WriteInteger("Settings", "esp.bone", Ikaros.m_pCvars->bBoneESP);
	WriteInteger("Settings", "esp.enemyonly", Ikaros.m_pCvars->bEnemyOnlyESP);
	WriteInteger("Settings", "esp.aimspot", Ikaros.m_pCvars->bAimspotESP);
	WriteInteger("Settings", "esp.preview", Ikaros.m_pCvars->bPreview);

	WriteInteger("Settings", "misc.autopistol", Ikaros.m_pCvars->bAutoPistol);
	WriteInteger("Settings", "misc.autostrafe", Ikaros.m_pCvars->iAutoStrafe);
	WriteInteger("Settings", "misc.bhop", Ikaros.m_pCvars->bBunnyHop);
	WriteInteger("Settings", "misc.crosshair", Ikaros.m_pCvars->iCrosshair);
	WriteInteger("Settings", "misc.hitsound", Ikaros.m_pCvars->bHitSound);
	WriteInteger("Settings", "misc.speedhack", Ikaros.m_pCvars->bSpeed);
	WriteFloat("Settings", "misc.speedvalue", Ikaros.m_pCvars->iSpeedVal);
	WriteInteger("Settings", "misc.speedkey", Ikaros.m_pCvars->iSpeedKey);
	WriteInteger("Settings", "misc.fullbright", Ikaros.m_pCvars->bFullbright);
	WriteInteger("Settings", "misc.roundsay", Ikaros.m_pCvars->roundsay);
	WriteInteger("Settings", "misc.svcheats", Ikaros.m_pCvars->Cheats);
	WriteInteger("Settings", "misc.namestealer", Ikaros.m_pCvars->bNameStealer);
	WriteInteger("Settings", "misc.circlestrafe", Ikaros.m_pCvars->CircleStrafe);
	WriteInteger("Settings", "misc.benemycheck", Ikaros.m_pCvars->bEnemyCheck);
	WriteInteger("Settings", "misc.handsposition", Ikaros.m_pCvars->iHandsPosition);
	WriteInteger("Settings", "misc.killsay", Ikaros.m_pCvars->iKillSay);
	WriteInteger("Settings", "misc.knifeAA", Ikaros.m_pCvars->KnifeAA);
	WriteInteger("Settings", "misc.knifebot", Ikaros.m_pCvars->Knifebot);
	WriteInteger("Settings", "misc.radiokillinformer", Ikaros.m_pCvars->RadioKillInformer);
	WriteInteger("Settings", "misc.airstuck", Ikaros.m_pCvars->Airstuck);
	WriteInteger("Settings", "misc.copycat", Ikaros.m_pCvars->bChatRepeat);
	WriteInteger("Settings", "misc.fastwalk", Ikaros.m_pCvars->FastWalk);
	WriteFloat("Settings", "misc.chatcleartime", Ikaros.m_pCvars->ChatClearTime);
	WriteInteger("Settings", "misc.lagfix", Ikaros.m_pCvars->LagFix);
	WriteInteger("Settings", "misc.lagexploit", Ikaros.m_pCvars->LagExploit);
	WriteInteger("Settings", "misc.fakeduck", Ikaros.m_pCvars->FakeDuck);
	WriteInteger("Settings", "misc.flashspam", Ikaros.m_pCvars->bFlashSpam);
	WriteInteger("Settings", "misc.fakelag", Ikaros.m_pCvars->fakelag);
	WriteFloat("Settings", "misc.fakelagamount", Ikaros.m_pCvars->FakelagAmount);
	WriteFloat("Settings", "misc.viewmodelfov", Ikaros.m_pCvars->ViewmodelFOV);
	WriteFloat("Settings", "misc.overridefov", Ikaros.m_pCvars->OverrideFOV);

	WriteMenuInteger("Settings", "menu.playerlist", Ikaros.m_pCvars->bPlayerList);
	WriteMenuInteger("Settings", "menu.x", Ikaros.m_pCvars->iMenuX);
	WriteMenuInteger("Settings", "menu.y", Ikaros.m_pCvars->iMenuY);
	WriteInteger("Settings", "menu.watermark", Ikaros.m_pCvars->bWatermark);
	WriteInteger("Settings", "menu.watermarkcolor", Ikaros.m_pCvars->iWatermark);
	WriteInteger("Settings", "menu.rainbowoutline", Ikaros.m_pCvars->bRainbowOutline);
	WriteInteger("Settings", "menu.rainbowinline", Ikaros.m_pCvars->bRainbowInline);
	WriteFloat("Settings", "menu.blackout", Ikaros.m_pCvars->BlackOut);
	WriteInteger("Settings", "menu.closebuttons", Ikaros.m_pCvars->iDrawCloseButton);

	WriteInteger("Settings", "monitor.active", Ikaros.m_pCvars->bMonitorActive);
	WriteMenuInteger("Settings", "monitor.x", Ikaros.m_pCvars->iMonitorPosX);
	WriteMenuInteger("Settings", "monitor.y", Ikaros.m_pCvars->iMonitorPosY);

	WriteInteger("Settings", "radar.active", Ikaros.m_pCvars->bRadar2D);
	WriteInteger("Settings", "radar.window", Ikaros.m_pCvars->bRadarBG);
	WriteMenuInteger("Settings", "radar.x", Ikaros.m_pCvars->flRadarPos_x);
	WriteMenuInteger("Settings", "radar.y", Ikaros.m_pCvars->flRadarPos_y);
	WriteInteger("Settings", "radar.enemyonly", Ikaros.m_pCvars->bEnemyOnlyRadar);
	WriteInteger("Settings", "radar.names", Ikaros.m_pCvars->bRadarName);
	WriteMenuInteger("Settings", "radar.size", Ikaros.m_pCvars->bRadarSize);
	WriteMenuInteger("Settings", "radar.range", Ikaros.m_pCvars->bRadarRange);

	WriteInteger("Settings", "rem.asus", Ikaros.m_pCvars->bAsusWalls);
	WriteFloat("Settings", "rem.asusalpha", Ikaros.m_pCvars->flAsusAlpha);
	WriteInteger("Settings", "rem.flash", Ikaros.m_pCvars->bNoFlash);
	WriteInteger("Settings", "rem.hands", Ikaros.m_pCvars->bNoHands);
	WriteInteger("Settings", "rem.smoke", Ikaros.m_pCvars->bNoSmoke);
	WriteInteger("Settings", "rem.sky", Ikaros.m_pCvars->bNoSky);
	WriteInteger("Settings", "rem.rcs", Ikaros.m_pCvars->iRCS);
	WriteInteger("Settings", "rem.recoil", Ikaros.m_pCvars->iNoRecoil);
	WriteInteger("Settings", "rem.spread", Ikaros.m_pCvars->iNoSpread);
	WriteInteger("Settings", "rem.visrecoil", Ikaros.m_pCvars->bNoVisRecoil);
	WriteInteger("Settings", "rem.pspread", Ikaros.m_pCvars->iPNoSpread);
	WriteInteger("Settings", "rem.showshake", Ikaros.m_pCvars->bShowShake);
	WriteInteger("Settings", "rem.handswireframe", Ikaros.m_pCvars->bHandsWireframe);
	WriteInteger("Settings", "rem.nightmode", Ikaros.m_pCvars->bNightMode);
	WriteFloat("Settings", "rem.flnightmode", Ikaros.m_pCvars->flNightMode);
	WriteInteger("Settings", "rem.showvelocity", Ikaros.m_pCvars->bShowVelocity);

	WriteInteger("Settings", "trigger.active", Ikaros.m_pCvars->bTriggerActive);
	WriteInteger("Settings", "trigger.delay", Ikaros.m_pCvars->iTrigDelay);
	WriteInteger("Settings", "trigger.mode", Ikaros.m_pCvars->iTriggerbot);
	WriteInteger("Settings", "trigger.autoshoot", Ikaros.m_pCvars->bAutoTrigger);
	WriteInteger("Settings", "trigger.hitbox", Ikaros.m_pCvars->iTrigBone);
	WriteInteger("Settings", "trigger.multihitbox.head", Ikaros.m_pCvars->bTrigHead);
	WriteInteger("Settings", "trigger.multihitbox.chest", Ikaros.m_pCvars->bTrigChest);
	WriteInteger("Settings", "trigger.multihitbox.pelvis", Ikaros.m_pCvars->bTrigPelvis);
	WriteInteger("Settings", "trigger.multihitbox.arms", Ikaros.m_pCvars->bTrigArms);
	WriteInteger("Settings", "trigger.multihitbox.legs", Ikaros.m_pCvars->bTrigLegs);
	WriteInteger("Settings", "trigger.key", Ikaros.m_pCvars->iTrigKey);
	WriteInteger("Settings", "trigger.team", Ikaros.m_pCvars->bTrigTeam);

	WriteInteger("Settings", "vis.chams", Ikaros.m_pCvars->bChams);
	WriteInteger("Settings", "vis.weaponchams", Ikaros.m_pCvars->bWeaponChams);
	WriteInteger("Settings", "vis.glasschams", Ikaros.m_pCvars->bGlassChams);
	WriteInteger("Settings", "vis.xqz", Ikaros.m_pCvars->bXQZWalls);
	WriteInteger("Settings", "vis.playerchamsmaterial", Ikaros.m_pCvars->iPlayerChamsMaterial);
	WriteInteger("Settings", "vis.weaponchamsmaterial", Ikaros.m_pCvars->iWeaponChamsMaterial);
	WriteInteger("Settings", "vis.chamsthroughwalls", Ikaros.m_pCvars->bChamsThroughWalls);

	WriteFloat("Settings", "colors.red", Ikaros.m_pCvars->DrawColor);
	WriteFloat("Settings", "colors.green", Ikaros.m_pCvars->DrawColor2);
	WriteFloat("Settings", "colors.blue", Ikaros.m_pCvars->DrawColor3);
	WriteFloat("Settings", "colors.alpha", Ikaros.m_pCvars->DrawColor4);
	WriteFloat("Settings", "colors1.red", Ikaros.m_pCvars->DrawColor5);
	WriteFloat("Settings", "colors1.green", Ikaros.m_pCvars->DrawColor6);
	WriteFloat("Settings", "colors1.blue", Ikaros.m_pCvars->DrawColor7);
	WriteFloat("Settings", "colors1.alpha", Ikaros.m_pCvars->DrawColor8);
	WriteFloat("Settings", "colors2.red", Ikaros.m_pCvars->DrawColor9);
	WriteFloat("Settings", "colors2.green", Ikaros.m_pCvars->DrawColor10);
	WriteFloat("Settings", "colors2.blue", Ikaros.m_pCvars->DrawColor11);
	WriteFloat("Settings", "colors2.alpha", Ikaros.m_pCvars->DrawColor12);
	WriteFloat("Settings", "colors3.red", Ikaros.m_pCvars->DrawColor13);
	WriteFloat("Settings", "colors3.green", Ikaros.m_pCvars->DrawColor14);
	WriteFloat("Settings", "colors3.blue", Ikaros.m_pCvars->DrawColor15);
	WriteFloat("Settings", "colors3.alpha", Ikaros.m_pCvars->DrawColor16);
	WriteFloat("Settings", "colors4.red", Ikaros.m_pCvars->DrawColor17);
	WriteFloat("Settings", "colors4.green", Ikaros.m_pCvars->DrawColor18);
	WriteFloat("Settings", "colors4.blue", Ikaros.m_pCvars->DrawColor19);
	WriteFloat("Settings", "colors4.alpha", Ikaros.m_pCvars->DrawColor20);
	WriteFloat("Settings", "colors5.red", Ikaros.m_pCvars->DrawColor21);
	WriteFloat("Settings", "colors5.green", Ikaros.m_pCvars->DrawColor22);
	WriteFloat("Settings", "colors5.blue", Ikaros.m_pCvars->DrawColor23);
	WriteFloat("Settings", "colors5.alpha", Ikaros.m_pCvars->DrawColor24);
	WriteFloat("Settings", "chamstcolor.red", Ikaros.m_pCvars->DrawColor25);
	WriteFloat("Settings", "chamstcolor.green", Ikaros.m_pCvars->DrawColor26);
	WriteFloat("Settings", "chamstcolor.blue", Ikaros.m_pCvars->DrawColor27);
	WriteFloat("Settings", "chamsctcolor.red", Ikaros.m_pCvars->DrawColor28);
	WriteFloat("Settings", "chamsctcolor.green", Ikaros.m_pCvars->DrawColor29);
	WriteFloat("Settings", "chamsctcolor.blue", Ikaros.m_pCvars->DrawColor30);
	WriteFloat("Settings", "tabslinecolor.red", Ikaros.m_pCvars->DrawColor31);
	WriteFloat("Settings", "tabslinecolor.green", Ikaros.m_pCvars->DrawColor32);
	WriteFloat("Settings", "tabslinecolor.blue", Ikaros.m_pCvars->DrawColor33);
	WriteFloat("Settings", "tabslinecolor.alpha", Ikaros.m_pCvars->DrawColor34);
	WriteFloat("Settings", "windowstextcolor.red", Ikaros.m_pCvars->WindowsTextColor1);
	WriteFloat("Settings", "windowstextcolor.green", Ikaros.m_pCvars->WindowsTextColor2);
	WriteFloat("Settings", "windowstextcolor.blue", Ikaros.m_pCvars->WindowsTextColor3);
	WriteFloat("Settings", "windowstextcolor.alpha", Ikaros.m_pCvars->WindowsTextColor4);
	WriteInteger("Settings", "colors.previewcolors", Ikaros.m_pCvars->PreviewColors);
	WriteFloat("Settings", "vis.glasschamscolor", Ikaros.m_pCvars->GlassChamsColor);
}
void cIni::ResetColors()
{
	Ikaros.m_pCvars->DrawColor = ReadFloat("Settings", "colors.red", 0.f);
	Ikaros.m_pCvars->DrawColor2 = ReadFloat("Settings", "colors.green", 175.f);
	Ikaros.m_pCvars->DrawColor3 = ReadFloat("Settings", "colors.blue", 255.f);
	Ikaros.m_pCvars->DrawColor4 = ReadFloat("Settings", "colors.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor5 = ReadFloat("Settings", "colors1.red", 0.f);
	Ikaros.m_pCvars->DrawColor6 = ReadFloat("Settings", "colors1.green", 0.f);
	Ikaros.m_pCvars->DrawColor7 = ReadFloat("Settings", "colors1.blue", 0.f);
	Ikaros.m_pCvars->DrawColor8 = ReadFloat("Settings", "colors1.alpha", 120.f);
	Ikaros.m_pCvars->DrawColor9 = ReadFloat("Settings", "colors2.red", 0.f);
	Ikaros.m_pCvars->DrawColor10 = ReadFloat("Settings", "colors2.green", 175.f);
	Ikaros.m_pCvars->DrawColor11 = ReadFloat("Settings", "colors2.blue", 255.f);
	Ikaros.m_pCvars->DrawColor12 = ReadFloat("Settings", "colors2.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor13 = ReadFloat("Settings", "colors3.red", 0.f);
	Ikaros.m_pCvars->DrawColor14 = ReadFloat("Settings", "colors3.green", 175.f);
	Ikaros.m_pCvars->DrawColor15 = ReadFloat("Settings", "colors3.blue", 255.f);
	Ikaros.m_pCvars->DrawColor16 = ReadFloat("Settings", "colors3.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor17 = ReadFloat("Settings", "colors4.red", 0.f);
	Ikaros.m_pCvars->DrawColor18 = ReadFloat("Settings", "colors4.green", 175.f);
	Ikaros.m_pCvars->DrawColor19 = ReadFloat("Settings", "colors4.blue", 255.f);
	Ikaros.m_pCvars->DrawColor20 = ReadFloat("Settings", "colors4.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor21 = ReadFloat("Settings", "colors5.red", 0.f);
	Ikaros.m_pCvars->DrawColor22 = ReadFloat("Settings", "colors5.green", 200.f);
	Ikaros.m_pCvars->DrawColor23 = ReadFloat("Settings", "colors5.blue", 255.f);
	Ikaros.m_pCvars->DrawColor24 = ReadFloat("Settings", "colors5.alpha", 255.f);
	Ikaros.m_pCvars->DrawColor25 = ReadFloat("Settings", "chamstcolor.red", 1.0f);
	Ikaros.m_pCvars->DrawColor26 = ReadFloat("Settings", "chamstcolor.green", 0.2f);
	Ikaros.m_pCvars->DrawColor27 = ReadFloat("Settings", "chamstcolor.blue", 0.2f);
	Ikaros.m_pCvars->DrawColor28 = ReadFloat("Settings", "chamsctcolor.red", 0.2f);
	Ikaros.m_pCvars->DrawColor29 = ReadFloat("Settings", "chamsctcolor.green", 0.2f);
	Ikaros.m_pCvars->DrawColor30 = ReadFloat("Settings", "chamsctcolor.blue", 1.0f);
	Ikaros.m_pCvars->DrawColor31 = ReadFloat("Settings", "tabslinecolor.red", 0.f);
	Ikaros.m_pCvars->DrawColor32 = ReadFloat("Settings", "tabslinecolor.green", 175.f);
	Ikaros.m_pCvars->DrawColor33 = ReadFloat("Settings", "tabslinecolor.blue", 255.f);
	Ikaros.m_pCvars->DrawColor34 = ReadFloat("Settings", "tabslinecolor.alpha", 255.f);
	Ikaros.m_pCvars->WindowsTextColor1 = ReadFloat("Settings", "windowstextcolor.red", 255.f);
	Ikaros.m_pCvars->WindowsTextColor2 = ReadFloat("Settings", "windowstextcolor.green", 255.f);
	Ikaros.m_pCvars->WindowsTextColor3 = ReadFloat("Settings", "windowstextcolor.blue", 255.f);
	Ikaros.m_pCvars->WindowsTextColor4 = ReadFloat("Settings", "windowstextcolor.alpha", 255.f);
	//Ikaros.m_pCvars->PreviewColors = ReadInteger("Settings", "colors.previewcolors", 0.f);
	Ikaros.m_pCvars->GlassChamsColor = ReadFloat("Settings", "vis.glasschamscolor", 0.5f);
}